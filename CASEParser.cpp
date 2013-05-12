#include "CASEParser.h"

CASEParser::CASEParser(void)
{
    //ctor
    CALLED = false;
}

CASEParser::~CASEParser(void)
{
    //dtor
}
void CASEParser::initActionHeader(ActionHeader *pActHead)
{
    pActHead->m_firstFrame = 0;
    pActHead->m_lastFrame = 0;
    pActHead->m_boneAnmCount = 0;
}

void CASEParser::getActionHeader(const char *dir,ActionHeader *pActHead)
{
    initActionHeader(pActHead);
    FILE *r = fopen(dir,"r");
    if(r== NULL)
    {
        printf("[open file error] %s\n",dir);
        return;
    }
    char str[256];
	int nIntV;
	memset(pActHead->BACKGROUND_STATIC,0,sizeof(pActHead->BACKGROUND_STATIC));
	memset(pActHead->AMBIENT_STATIC,0,sizeof(pActHead->AMBIENT_STATIC));
    fscanf(r,"*3DSMAX_ASCIIEXPORT %d\n*COMMENT \"%[^\"]\"\n",&nIntV,pActHead->m_comment);
	fscanf(r,"*SCENE {\n");
	fscanf(r,"\t*SCENE_FILENAME \"%[^\"]\"\n",pActHead->m_fileName);
	fscanf(r,"\t*SCENE_FIRSTFRAME %d\n",&pActHead->m_firstFrame);
	fscanf(r,"\t*SCENE_LASTFRAME %d\n",&pActHead->m_lastFrame);
	fscanf(r,"\t*SCENE_FRAMESPEED %d\n",&pActHead->m_frameSpeed);
	fscanf(r,"\t*SCENE_TICKSPERFRAME %d\n",&pActHead->m_tpf);
	fscanf(r,"\t*SCENE_BACKGROUND_STATIC %f %f %f\n",\
		&pActHead->BACKGROUND_STATIC[0],\
		&pActHead->BACKGROUND_STATIC[1],\
		&pActHead->BACKGROUND_STATIC[2]);
	fscanf(r,"\t*SCENE_AMBIENT_STATIC %f %f %f\n}\n",\
		&pActHead->AMBIENT_STATIC[0],\
		&pActHead->AMBIENT_STATIC[1],\
		&pActHead->AMBIENT_STATIC[2]);

    pActHead->m_boneAnmCount = pActHead->m_boneRotCount	= pActHead->m_bonePosCount	= 0;
	pActHead->m_helpAnmCount  = pActHead->m_helpRotCount	= pActHead->m_helpPosCount	= 0;
    int pos_flag = 0,rot_flag = 0;
	int objectCnt = 0;


	memset(pActHead->m_bonePosIndex,-1,sizeof(pActHead->m_bonePosIndex));
	memset(pActHead->m_boneRotIndex,-1,sizeof(pActHead->m_boneRotIndex));
	memset(pActHead->m_helpPosIndex,-1,sizeof(pActHead->m_helpPosIndex));
	memset(pActHead->m_helpRotIndex,-1,sizeof(pActHead->m_helpRotIndex));

	while(fscanf(r,"%s",str) != EOF)
    {
        if(strstr(str,"*CONTROL_ROT_TRACK") != NULL && rot_flag)
        {
			assert(rot_flag == 1 || rot_flag == -1);
			if(rot_flag == 1)
			{
				pActHead->m_boneRotIndex[pActHead->m_boneAnmCount-1] = pActHead->m_boneRotCount;
				pActHead->m_boneRotCount++;
			}
			else//rot_flag = -1
			{
				pActHead->m_helpRotIndex[pActHead->m_helpAnmCount-1] = pActHead->m_helpRotCount;
				pActHead->m_helpRotCount++;
			}
            rot_flag = 0;
        }
        else if(strstr(str,"*CONTROL_POS_TRACK") != NULL && pos_flag)
        {
			assert(pos_flag == 1 || pos_flag == -1);
			if(pos_flag == 1)
			{
				pActHead->m_bonePosIndex[pActHead->m_boneAnmCount-1] = pActHead->m_bonePosCount;
				pActHead->m_bonePosCount++;
			}
			else
			{
				pActHead->m_helpPosIndex[pActHead->m_boneAnmCount-1] = pActHead->m_helpPosCount;
				pActHead->m_helpPosCount++;
			}
            pos_flag = 0;
        }
        else if(strstr(str,"*GEOMOBJECT") != NULL )
        {
            pActHead->m_boneAnmCount++;
            pos_flag = rot_flag = 1;
			pActHead->m_isHelperObject[objectCnt++] = 0;
        }
        else if(strstr(str,"*HELPEROBJECT") != NULL)
        {
			pActHead->m_helpAnmCount++;
            pos_flag = rot_flag = -1;
			pActHead->m_isHelperObject[objectCnt++] = 1;
        }
    }
	assert(objectCnt < 200);
	assert(pActHead->m_boneAnmCount < MAX_OBJECT);
	assert(pActHead->m_helpAnmCount  < MAX_HELPER_OBJECT);

    fclose(r); r = NULL;

	CALLED = true;
    return;
}

void CASEParser::parse(const char *dir,\
					   CVector3f *poskey,CVector3f *hposkey,\
					   CQuaternion *rotkey,CQuaternion *hrotkey,\
					   CAAP *aapkey,CAAP *haapkey,\
					   AnmHeader *pAnmHead,HAnmHeader *pHAnmHead,\
					   const ActionHeader *pActHead)
{
	assert( CALLED == true );
    FILE *r = fopen(dir,"r");
    if(r== NULL)
    {
        printf("[open file error] %s\n",dir);
        return;
    }
    char str[256] = "nothing";
	int nBoneCnt = -1,nRotCnt = -1,nPosCnt = -1;
	int nHBoneCnt = -1,nHRotCnt = -1,nHPosCnt = -1;
	int nClipNum = (pActHead->m_lastFrame-pActHead->m_firstFrame+1);
    bool rootCfrm = 0;
	int nIntV;
	float fFloatV;
	fscanf(r,"*3DSMAX_ASCIIEXPORT %d\n*COMMENT \"%[^\"]\"\n",&nIntV,pActHead->m_comment);
	fscanf(r,"*SCENE {\n");
	fscanf(r,"\t*SCENE_FILENAME \"%[^\"]\"\n",pActHead->m_fileName);
	fscanf(r,"\t*SCENE_FIRSTFRAME %d\n",&nIntV);
	fscanf(r,"\t*SCENE_LASTFRAME %d\n",&nIntV);
	fscanf(r,"\t*SCENE_FRAMESPEED %d\n",&nIntV);
	fscanf(r,"\t*SCENE_TICKSPERFRAME %d\n",&nIntV);
	fscanf(r,"\t*SCENE_BACKGROUND_STATIC %f %f %f\n",&fFloatV,&fFloatV,&fFloatV);
	fscanf(r,"\t*SCENE_AMBIENT_STATIC %f %f %f\n}\n",&fFloatV,&fFloatV,&fFloatV);
	while(fscanf(r,"%s {\n",str) != EOF)
	{
		if( strcmp(str,"*GEOMOBJECT") == 0)
		{
			nBoneCnt++;
			fscanf(r,"\t*NODE_NAME \"%[^\"]\"\n",pAnmHead[nBoneCnt].m_nodeName);

			if(rootCfrm)
			{
				fscanf(r,"\t*NODE_PARENT \"%[^\"]\"\n",pAnmHead[nBoneCnt].m_nodeParentName);
			}
			else
			{
				rootCfrm = 1;
			}
			fscanf(r,"\t*NODE_TM {\n");
			fscanf(r,"\t\t*NODE_NAME \"%[^\"]\"\n",str);
			fscanf(r,"\t\t*INHERIT_POS %f %f %f\n",\
				&pAnmHead[nBoneCnt].INHERIT_POS[0],\
				&pAnmHead[nBoneCnt].INHERIT_POS[1],\
				&pAnmHead[nBoneCnt].INHERIT_POS[2]);
			fscanf(r,"\t\t*INHERIT_ROT %f %f %f\n",\
				&pAnmHead[nBoneCnt].INHERIT_ROT[0],\
				&pAnmHead[nBoneCnt].INHERIT_ROT[1],\
				&pAnmHead[nBoneCnt].INHERIT_ROT[2]);
			fscanf(r,"\t\t*INHERIT_SCL %f %f %f\n",\
				&pAnmHead[nBoneCnt].INHERIT_SCL[0],\
				&pAnmHead[nBoneCnt].INHERIT_SCL[1],\
				&pAnmHead[nBoneCnt].INHERIT_SCL[2]);
			fscanf(r,"\t\t*TM_ROW0 %f %f %f\n",&pAnmHead[nBoneCnt].TM_ROW0[0],&pAnmHead[nBoneCnt].TM_ROW0[1],&pAnmHead[nBoneCnt].TM_ROW0[2]);
			fscanf(r,"\t\t*TM_ROW1 %f %f %f\n",&pAnmHead[nBoneCnt].TM_ROW1[0],&pAnmHead[nBoneCnt].TM_ROW1[1],&pAnmHead[nBoneCnt].TM_ROW1[2]);
			fscanf(r,"\t\t*TM_ROW2 %f %f %f\n",&pAnmHead[nBoneCnt].TM_ROW2[0],&pAnmHead[nBoneCnt].TM_ROW2[1],&pAnmHead[nBoneCnt].TM_ROW2[2]);
			fscanf(r,"\t\t*TM_ROW3 %f %f %f\n",&pAnmHead[nBoneCnt].TM_ROW3[0],&pAnmHead[nBoneCnt].TM_ROW3[1],&pAnmHead[nBoneCnt].TM_ROW3[2]);
			fscanf(r,"\t\t*TM_POS %f %f %f\n",\
				&pAnmHead[nBoneCnt].m_initPos[0],\
				&pAnmHead[nBoneCnt].m_initPos[1],\
				&pAnmHead[nBoneCnt].m_initPos[2]);
			fscanf(r,"\t\t*TM_ROTAXIS %f %f %f\n",\
				&pAnmHead[nBoneCnt].TM_ROTAXIS[0],\
				&pAnmHead[nBoneCnt].TM_ROTAXIS[1],\
				&pAnmHead[nBoneCnt].TM_ROTAXIS[2]);
			fscanf(r,"\t\t*TM_ROTANGLE %f\n",&pAnmHead[nBoneCnt].TM_ROTANGLE);
			fscanf(r,"\t\t*TM_QUATERNION w=%f (%f %f %f)\n",\
				&pAnmHead[nBoneCnt].m_initRot[0],\
				&pAnmHead[nBoneCnt].m_initRot[1],\
				&pAnmHead[nBoneCnt].m_initRot[2],\
				&pAnmHead[nBoneCnt].m_initRot[3]);
			fscanf(r,"\t\t*TM_SCALE %f %f %f\n",\
				&pAnmHead[nBoneCnt].m_initScale[0],\
				&pAnmHead[nBoneCnt].m_initScale[1],\
				&pAnmHead[nBoneCnt].m_initScale[2]);


			//specfied
			//assert scale == 1

			fscanf(r,"\t\t*TM_SCALEAXIS %f %f %f\n",\
				&pAnmHead[nBoneCnt].TM_SCALEAXIS[0],\
				&pAnmHead[nBoneCnt].TM_SCALEAXIS[1],\
				&pAnmHead[nBoneCnt].TM_SCALEAXIS[2]);
			fscanf(r,"\t\t*TM_SCALEAXISANG %f\n\t}\n",&pAnmHead[nBoneCnt].TM_SCALEAXISANG);
			fscanf(r,"\t*PROP_MOTIONBLUR %d\n",&pAnmHead[nBoneCnt].PROP_MOTIONBLUR);
			fscanf(r,"\t*PROP_CASTSHADOW %d\n",&pAnmHead[nBoneCnt].PROP_CASTSHADOW);
			fscanf(r,"\t*PROP_RECVSHADOW %d\n",&pAnmHead[nBoneCnt].PROP_RECVSHADOW);

			pAnmHead[nBoneCnt].m_numPoskey = pAnmHead[nBoneCnt].m_numRotkey = 0;
			fscanf(r,"\t%s {\n",str);
			if( strcmp(str,"*TM_ANIMATION") != 0 )
			{
				fscanf(r,"}\n");
				continue;
			}
			fscanf(r,"\t\t*NODE_NAME \"%[^\"]\"\n",str);
			fscanf(r,"\t\t%s {\n",str);

			if( strcmp(str,"*CONTROL_POS_TRACK") == 0 )
			{
				nPosCnt++;
				pAnmHead[nBoneCnt].m_numPoskey = nClipNum;
				//pos data
				int index = nPosCnt*nClipNum;
				for(int i = 0;i < nClipNum;i++)
				{
					int t = fscanf(r,"\t\t\t*CONTROL_POS_SAMPLE %d %f %f %f\n",&nIntV,&poskey[index+i].x,&poskey[index+i].y,&poskey[index+i].z);
					assert(t == 4);
				}

				fscanf(r,"\t\t}\n");
			}
			else if( strcmp(str,"*CONTROL_ROT_TRACK") == 0 )
			{
				nRotCnt++;
				pAnmHead[nBoneCnt].m_numRotkey = nClipNum;
				//rot data
				int index = nRotCnt*nClipNum;
				for(int i = 0;i < nClipNum;i++)
				{
					fscanf(r,"\t\t\t*CONTROL_ROT_SAMPLE %d %f %f %f %f\n",&nIntV,&aapkey[index+i].x,&aapkey[index+i].y,&aapkey[index+i].z,&aapkey[index+i].angle);
					fscanf(r,"\t\t\t*CONTROL_ROT_QUATERNION %d\tw=%f (%f %f %f)\n",&nIntV,&rotkey[index+i].w,&rotkey[index+i].x,&rotkey[index+i].y,&rotkey[index+i].z);
				}
				fscanf(r,"\t\t}\n");
			}
			fscanf(r,"\t\t%s {\n",str);
			if( strcmp(str,"*CONTROL_ROT_TRACK") != 0)
			{
                fscanf(r,"\t}\n}\n");
				continue;
			}
			assert( strcmp(str,"*CONTROL_ROT_TRACK") == 0 );
			{
				nRotCnt++;
				pAnmHead[nBoneCnt].m_numRotkey = nClipNum;
				//rot data
				int index = nRotCnt*nClipNum;
				for(int i = 0;i < nClipNum;i++)
				{
					fscanf(r,"\t\t\t*CONTROL_ROT_SAMPLE %d %f %f %f %f\n",&nIntV,&aapkey[index+i].x,&aapkey[index+i].y,&aapkey[index+i].z,&aapkey[index+i].angle);
					fscanf(r,"\t\t\t*CONTROL_ROT_QUATERNION %d\tw=%f (%f %f %f)\n",&nIntV,&rotkey[index+i].w,&rotkey[index+i].x,&rotkey[index+i].y,&rotkey[index+i].z);
				}
				fscanf(r,"\t\t}\n");
			}
			fscanf(r,"\t}\n}\n");
		}
		else if( strcmp(str,"*HELPEROBJECT") == 0 )
		{
			nHBoneCnt++;
			fscanf(r,"\t*NODE_NAME \"%[^\"]\"\n",pHAnmHead[nHBoneCnt].m_nodeName);
			if(rootCfrm)
			{
				fscanf(r,"\t*NODE_PARENT \"%[^\"]\"\n",pHAnmHead[nHBoneCnt].m_nodeParentName);
			}
			else
			{
				rootCfrm = 1;
			}
			fscanf(r,"\t*HELPER_CLASS \"%[^\"]\"\n",pHAnmHead[nHBoneCnt].m_className);
			fscanf(r,"\t*NODE_TM {\n");
			fscanf(r,"\t\t*NODE_NAME \"%[^\"]\"\n",str);
			fscanf(r,"\t\t*INHERIT_POS %f %f %f\n",\
				&pHAnmHead[nHBoneCnt].INHERIT_POS[0],\
				&pHAnmHead[nHBoneCnt].INHERIT_POS[1],\
				&pHAnmHead[nHBoneCnt].INHERIT_POS[2]);
			fscanf(r,"\t\t*INHERIT_ROT %f %f %f\n",\
				&pHAnmHead[nHBoneCnt].INHERIT_ROT[0],\
				&pHAnmHead[nHBoneCnt].INHERIT_ROT[1],\
				&pHAnmHead[nHBoneCnt].INHERIT_ROT[2]);
			fscanf(r,"\t\t*INHERIT_SCL %f %f %f\n",\
				&pHAnmHead[nHBoneCnt].INHERIT_SCL[0],\
				&pHAnmHead[nHBoneCnt].INHERIT_SCL[1],\
				&pHAnmHead[nHBoneCnt].INHERIT_SCL[2]);
			fscanf(r,"\t\t*TM_ROW0 %f %f %f\n",&pHAnmHead[nHBoneCnt].TM_ROW0[0],&pHAnmHead[nHBoneCnt].TM_ROW0[1],&pHAnmHead[nHBoneCnt].TM_ROW0[2]);
			fscanf(r,"\t\t*TM_ROW1 %f %f %f\n",&pHAnmHead[nHBoneCnt].TM_ROW1[0],&pHAnmHead[nHBoneCnt].TM_ROW1[1],&pHAnmHead[nHBoneCnt].TM_ROW1[2]);
			fscanf(r,"\t\t*TM_ROW2 %f %f %f\n",&pHAnmHead[nHBoneCnt].TM_ROW2[0],&pHAnmHead[nHBoneCnt].TM_ROW2[1],&pHAnmHead[nHBoneCnt].TM_ROW2[2]);
			fscanf(r,"\t\t*TM_ROW3 %f %f %f\n",&pHAnmHead[nHBoneCnt].TM_ROW3[0],&pHAnmHead[nHBoneCnt].TM_ROW3[1],&pHAnmHead[nHBoneCnt].TM_ROW3[2]);
			fscanf(r,"\t\t*TM_POS %f %f %f\n",\
				&pHAnmHead[nHBoneCnt].m_initPos[0],\
				&pHAnmHead[nHBoneCnt].m_initPos[1],\
				&pHAnmHead[nHBoneCnt].m_initPos[2]);
			fscanf(r,"\t\t*TM_ROTAXIS %f %f %f\n",\
				&pHAnmHead[nHBoneCnt].TM_ROTAXIS[0],\
				&pHAnmHead[nHBoneCnt].TM_ROTAXIS[1],\
				&pHAnmHead[nHBoneCnt].TM_ROTAXIS[2]);
			fscanf(r,"\t\t*TM_ROTANGLE %f\n",&pHAnmHead[nHBoneCnt].TM_ROTANGLE);
			fscanf(r,"\t\t*TM_QUATERNION w=%f (%f %f %f)\n",\
				&pHAnmHead[nHBoneCnt].m_initRot[0],\
				&pHAnmHead[nHBoneCnt].m_initRot[1],\
				&pHAnmHead[nHBoneCnt].m_initRot[2],\
				&pHAnmHead[nHBoneCnt].m_initRot[3]);
			fscanf(r,"\t\t*TM_SCALE %f %f %f\n",\
				&pHAnmHead[nHBoneCnt].m_initScale[0],\
				&pHAnmHead[nHBoneCnt].m_initScale[1],\
				&pHAnmHead[nHBoneCnt].m_initScale[2]);

			fscanf(r,"\t\t*TM_SCALEAXIS %f %f %f\n",\
				&pHAnmHead[nHBoneCnt].TM_SCALEAXIS[0],\
				&pHAnmHead[nHBoneCnt].TM_SCALEAXIS[1],\
				&pHAnmHead[nHBoneCnt].TM_SCALEAXIS[2]);
			fscanf(r,"\t\t*TM_SCALEAXISANG %f\n\t}\n",&pHAnmHead[nHBoneCnt].TM_SCALEAXISANG);
			fscanf(r,"\t*BOUNDINGBOX_MIN %f %f %f\n",\
				&pHAnmHead[nHBoneCnt].BOUNDINGBOX_MIN[0],\
				&pHAnmHead[nHBoneCnt].BOUNDINGBOX_MIN[1],\
				&pHAnmHead[nHBoneCnt].BOUNDINGBOX_MIN[2]);
			fscanf(r,"\t*BOUNDINGBOX_MAX %f %f %f\n",\
				&pHAnmHead[nHBoneCnt].BOUNDINGBOX_MAX[0],\
				&pHAnmHead[nHBoneCnt].BOUNDINGBOX_MAX[1],\
				&pHAnmHead[nHBoneCnt].BOUNDINGBOX_MAX[2]);

			pHAnmHead[nHBoneCnt].m_numPoskey = pHAnmHead[nHBoneCnt].m_numRotkey = 0;
			fscanf(r,"\t%s {\n",str);
			if( strcmp(str,"*TM_ANIMATION") != 0 )
			{
				fscanf(r,"}\n");
				continue;
			}
			fscanf(r,"\t\t*NODE_NAME \"%[^\"]\"\n",str);
			fscanf(r,"\t\t%s {\n",str);
			if( strcmp(str,"*CONTROL_POS_TRACK") == 0 )
			{
				nHPosCnt++;
				pHAnmHead[nHBoneCnt].m_numPoskey = nClipNum;
				//pos data
				int index = nHPosCnt*nClipNum;
				for(int i = 0;i < nClipNum;i++)
				{
					int t = fscanf(r,"\t\t\t*CONTROL_POS_SAMPLE %d %f %f %f\n",&nIntV,&hposkey[index+i].x,&hposkey[index+i].y,&hposkey[index+i].z);
					assert(t == 4);
				}

				fscanf(r,"\t\t}\n");
			}
			else if( strcmp(str,"*CONTROL_ROT_TRACK") == 0 )
			{
				nHRotCnt++;
				pHAnmHead[nHBoneCnt].m_numRotkey = nClipNum;
				//rot data
				int index = nHRotCnt*nClipNum;
				for(int i = 0;i < nClipNum;i++)
				{
					fscanf(r,"\t\t\t*CONTROL_ROT_SAMPLE %d %f %f %f %f\n",&nIntV,&haapkey[index+i].x,&haapkey[index+i].y,&haapkey[index+i].z,&haapkey[index+i].angle);
					fscanf(r,"\t\t\t*CONTROL_ROT_QUATERNION %d\tw=%f (%f %f %f)\n",&nIntV,&hrotkey[index+i].w,&hrotkey[index+i].x,&hrotkey[index+i].y,&hrotkey[index+i].z);
				}
				fscanf(r,"\t\t}\n");
			}
			fscanf(r,"\t\t%s {\n",str);
			if( strcmp(str,"*CONTROL_ROT_TRACK") != 0)
			{
                fscanf(r,"\t}\n}\n");
				continue;
			}
			assert( strcmp(str,"*CONTROL_ROT_TRACK") == 0 );
			{
				nHRotCnt++;
				pHAnmHead[nHBoneCnt].m_numRotkey = nClipNum;
				//rot data
				int index = nHRotCnt*nClipNum;
				for(int i = 0;i < nClipNum;i++)
				{
					fscanf(r,"\t\t\t*CONTROL_ROT_SAMPLE %d %f %f %f %f\n",&nIntV,&haapkey[index+i].x,&haapkey[index+i].y,&haapkey[index+i].z,&haapkey[index+i].angle);
					fscanf(r,"\t\t\t*CONTROL_ROT_QUATERNION %d\tw=%f (%f %f %f)\n",&nIntV,&hrotkey[index+i].w,&hrotkey[index+i].x,&hrotkey[index+i].y,&hrotkey[index+i].z);
				}
				fscanf(r,"\t\t}\n");
			}
			fscanf(r,"\t}\n}\n");
		}
	}
    assert(nRotCnt == (signed)pActHead->m_boneRotCount-1);
    assert(nPosCnt == (signed)pActHead->m_bonePosCount-1);
    assert(nBoneCnt == (signed)pActHead->m_boneAnmCount-1);
	assert(nHRotCnt == (signed)pActHead->m_helpRotCount-1);
    assert(nHPosCnt == (signed)pActHead->m_helpPosCount-1);
    assert(nHBoneCnt == (signed)pActHead->m_helpAnmCount-1);
    fclose(r); r = 0;
}


void CASEParser::restore(const char *dir,\
					   const CVector3f *poskey,const CVector3f *hposkey,\
					   const CQuaternion *rotkey,const CQuaternion *hrotkey,\
					   const CAAP *aapkey,const CAAP *haapkey,\
					   const AnmHeader *pAnmHead,const HAnmHeader *pHAnmHead,\
					   const ActionHeader *pActHead)
{
	FILE *r = fopen(dir,"w");
    if(r== NULL)
    {
        printf("[open file error] %s\n",dir);
        return;
    }
	int nBoneCnt = -1,nRotCnt = -1,nPosCnt = -1;
	int nHBoneCnt = -1,nHRotCnt = -1,nHPosCnt = -1;
	int nClipNum = (pActHead->m_lastFrame-pActHead->m_firstFrame+1);
    bool rootCfrm = 0;
	fprintf(r,"*3DSMAX_ASCIIEXPORT %d\n*COMMENT \"%s\"\n",200,pActHead->m_comment);
	fprintf(r,"*SCENE {\n");
	fprintf(r,"\t*SCENE_FILENAME \"%s\"\n",pActHead->m_fileName);
	fprintf(r,"\t*SCENE_FIRSTFRAME %d\n",pActHead->m_firstFrame);
	fprintf(r,"\t*SCENE_LASTFRAME %d\n",pActHead->m_lastFrame);
	fprintf(r,"\t*SCENE_FRAMESPEED %d\n",pActHead->m_frameSpeed);
	fprintf(r,"\t*SCENE_TICKSPERFRAME %d\n",pActHead->m_tpf);
	fprintf(r,"\t*SCENE_BACKGROUND_STATIC %0.5f %0.5f %0.5f\n",pActHead->BACKGROUND_STATIC[0],pActHead->BACKGROUND_STATIC[1],pActHead->BACKGROUND_STATIC[2]);
	fprintf(r,"\t*SCENE_AMBIENT_STATIC %0.5f %0.5f %0.5f\n}\n",pActHead->AMBIENT_STATIC[0],pActHead->AMBIENT_STATIC[1],pActHead->AMBIENT_STATIC[2]);
	int nTotalObject = pActHead->m_boneAnmCount+pActHead->m_helpAnmCount;
	for(int obj = 0;obj < nTotalObject;obj++)
	{
		if( pActHead->m_isHelperObject[obj] == 0)
		{
			fprintf(r,"%s {\n","*GEOMOBJECT");
			nBoneCnt++;
			fprintf(r,"\t*NODE_NAME \"%s\"\n",pAnmHead[nBoneCnt].m_nodeName);
			if(rootCfrm)
			{
				fprintf(r,"\t*NODE_PARENT \"%s\"\n",pAnmHead[nBoneCnt].m_nodeParentName);
			}
			else
			{
				rootCfrm = 1;
			}
			fprintf(r,"\t*NODE_TM {\n");
			fprintf(r,"\t\t*NODE_NAME \"%s\"\n",pAnmHead[nBoneCnt].m_nodeName);
			fprintf(r,"\t\t*INHERIT_POS %g %g %g\n",\
				pAnmHead[nBoneCnt].INHERIT_POS[0],\
				pAnmHead[nBoneCnt].INHERIT_POS[1],\
				pAnmHead[nBoneCnt].INHERIT_POS[2]);
			fprintf(r,"\t\t*INHERIT_ROT %g %g %g\n",\
				pAnmHead[nBoneCnt].INHERIT_ROT[0],\
				pAnmHead[nBoneCnt].INHERIT_ROT[1],\
				pAnmHead[nBoneCnt].INHERIT_ROT[2]);
			fprintf(r,"\t\t*INHERIT_SCL %g %g %g\n",\
				pAnmHead[nBoneCnt].INHERIT_SCL[0],\
				pAnmHead[nBoneCnt].INHERIT_SCL[1],\
				pAnmHead[nBoneCnt].INHERIT_SCL[2]);
			fprintf(r,"\t\t*TM_ROW0 %f %f %f\n",pAnmHead[nBoneCnt].TM_ROW0[0],pAnmHead[nBoneCnt].TM_ROW0[1],pAnmHead[nBoneCnt].TM_ROW0[2]);
			fprintf(r,"\t\t*TM_ROW1 %f %f %f\n",pAnmHead[nBoneCnt].TM_ROW1[0],pAnmHead[nBoneCnt].TM_ROW1[1],pAnmHead[nBoneCnt].TM_ROW1[2]);
			fprintf(r,"\t\t*TM_ROW2 %f %f %f\n",pAnmHead[nBoneCnt].TM_ROW2[0],pAnmHead[nBoneCnt].TM_ROW2[1],pAnmHead[nBoneCnt].TM_ROW2[2]);
			fprintf(r,"\t\t*TM_ROW3 %f %f %f\n",pAnmHead[nBoneCnt].TM_ROW3[0],pAnmHead[nBoneCnt].TM_ROW3[1],pAnmHead[nBoneCnt].TM_ROW3[2]);
			fprintf(r,"\t\t*TM_POS %f %f %f\n",\
				pAnmHead[nBoneCnt].m_initPos[0],\
				pAnmHead[nBoneCnt].m_initPos[1],\
				pAnmHead[nBoneCnt].m_initPos[2]);
			fprintf(r,"\t\t*TM_ROTAXIS %f %f %f\n",\
				pAnmHead[nBoneCnt].TM_ROTAXIS[0],\
				pAnmHead[nBoneCnt].TM_ROTAXIS[1],\
				pAnmHead[nBoneCnt].TM_ROTAXIS[2]);
			fprintf(r,"\t\t*TM_ROTANGLE %0.5f\n",pAnmHead[nBoneCnt].TM_ROTANGLE);
			fprintf(r,"\t\t*TM_QUATERNION w=%f (%f %f %f)\n",\
				pAnmHead[nBoneCnt].m_initRot[0],\
				pAnmHead[nBoneCnt].m_initRot[1],\
				pAnmHead[nBoneCnt].m_initRot[2],\
				pAnmHead[nBoneCnt].m_initRot[3]);
			fprintf(r,"\t\t*TM_SCALE %f %f %f\n",\
				pAnmHead[nBoneCnt].m_initScale[0],\
				pAnmHead[nBoneCnt].m_initScale[1],\
				pAnmHead[nBoneCnt].m_initScale[2]);

			fprintf(r,"\t\t*TM_SCALEAXIS %f %f %f\n",\
				pAnmHead[nBoneCnt].TM_SCALEAXIS[0],\
				pAnmHead[nBoneCnt].TM_SCALEAXIS[1],\
				pAnmHead[nBoneCnt].TM_SCALEAXIS[2]);
			fprintf(r,"\t\t*TM_SCALEAXISANG %0.5f\n\t}\n",pAnmHead[nBoneCnt].TM_SCALEAXISANG);
			fprintf(r,"\t*PROP_MOTIONBLUR %d\n",pAnmHead[nBoneCnt].PROP_MOTIONBLUR);
			fprintf(r,"\t*PROP_CASTSHADOW %d\n",pAnmHead[nBoneCnt].PROP_CASTSHADOW);
			fprintf(r,"\t*PROP_RECVSHADOW %d\n",pAnmHead[nBoneCnt].PROP_RECVSHADOW);

			if(pAnmHead[nBoneCnt].m_numPoskey == 0 && pAnmHead[nBoneCnt].m_numRotkey == 0)
			{
				fprintf(r,"}\n");
				continue;
			}
			else
			{
				fprintf(r,"\t%s {\n","*TM_ANIMATION");
			}
			fprintf(r,"\t\t*NODE_NAME \"%s\"\n",pAnmHead[nBoneCnt].m_nodeName);
			if(pAnmHead[nBoneCnt].m_numPoskey != 0)
			{
				fprintf(r,"\t\t%s {\n","*CONTROL_POS_TRACK");
				nPosCnt++;
				//pos data
				int index = nPosCnt*nClipNum;
				for(int i = 0;i < nClipNum;i++)
				{
					fprintf(r,"\t\t\t*CONTROL_POS_SAMPLE %d %f %f %f\n",(pActHead->m_tpf)*(pActHead->m_firstFrame+i),poskey[index+i].x,poskey[index+i].y,poskey[index+i].z);
				}
				fprintf(r,"\t\t}\n");
			}
			if(pAnmHead[nBoneCnt].m_numRotkey != 0)
			{
				fprintf(r,"\t\t%s {\n","*CONTROL_ROT_TRACK");
				nRotCnt++;
				//rot data
				int index = nRotCnt*nClipNum;
				for(int i = 0;i < nClipNum;i++)
				{
					fprintf(r,"\t\t\t*CONTROL_ROT_SAMPLE %d %0.5f %0.5f %0.5f %0.5f\n",\
						(pActHead->m_tpf)*(pActHead->m_firstFrame+i),\
						aapkey[index+i].x,aapkey[index+i].y,aapkey[index+i].z,aapkey[index+i].angle);
					fprintf(r,"\t\t\t*CONTROL_ROT_QUATERNION %d\tw=%f (%f %f %f)\n",\
						(pActHead->m_tpf)*(pActHead->m_firstFrame+i),\
						rotkey[index+i].w,rotkey[index+i].x,rotkey[index+i].y,rotkey[index+i].z);
				}
				fprintf(r,"\t\t}\n");
			}
			fprintf(r,"\t}\n}\n");
		}
		else if( pActHead->m_isHelperObject[obj] == 1 )
		{
			fprintf(r,"%s {\n","*HELPEROBJECT");
			nHBoneCnt++;
			fprintf(r,"\t*NODE_NAME \"%s\"\n",pHAnmHead[nHBoneCnt].m_nodeName);
			if(rootCfrm)
			{
				fprintf(r,"\t*NODE_PARENT \"%s\"\n",pHAnmHead[nHBoneCnt].m_nodeParentName);
			}
			else
			{
				rootCfrm = 1;
			}
			fprintf(r,"\t*HELPER_CLASS \"%s\"\n",pHAnmHead[nHBoneCnt].m_className);
			fprintf(r,"\t*NODE_TM {\n");
			fprintf(r,"\t\t*NODE_NAME \"%s\"\n",pHAnmHead[nHBoneCnt].m_nodeName);
			fprintf(r,"\t\t*INHERIT_POS %g %g %g\n",\
				pHAnmHead[nHBoneCnt].INHERIT_POS[0],\
				pHAnmHead[nHBoneCnt].INHERIT_POS[1],\
				pHAnmHead[nHBoneCnt].INHERIT_POS[2]);
			fprintf(r,"\t\t*INHERIT_ROT %g %g %g\n",\
				pHAnmHead[nHBoneCnt].INHERIT_ROT[0],\
				pHAnmHead[nHBoneCnt].INHERIT_ROT[1],\
				pHAnmHead[nHBoneCnt].INHERIT_ROT[2]);
			fprintf(r,"\t\t*INHERIT_SCL %g %g %g\n",\
				pHAnmHead[nHBoneCnt].INHERIT_SCL[0],\
				pHAnmHead[nHBoneCnt].INHERIT_SCL[1],\
				pHAnmHead[nHBoneCnt].INHERIT_SCL[2]);
			fprintf(r,"\t\t*TM_ROW0 %f %f %f\n",pHAnmHead[nHBoneCnt].TM_ROW0[0],pHAnmHead[nHBoneCnt].TM_ROW0[1],pHAnmHead[nHBoneCnt].TM_ROW0[2]);
			fprintf(r,"\t\t*TM_ROW1 %f %f %f\n",pHAnmHead[nHBoneCnt].TM_ROW1[0],pHAnmHead[nHBoneCnt].TM_ROW1[1],pHAnmHead[nHBoneCnt].TM_ROW1[2]);
			fprintf(r,"\t\t*TM_ROW2 %f %f %f\n",pHAnmHead[nHBoneCnt].TM_ROW2[0],pHAnmHead[nHBoneCnt].TM_ROW2[1],pHAnmHead[nHBoneCnt].TM_ROW2[2]);
			fprintf(r,"\t\t*TM_ROW3 %f %f %f\n",pHAnmHead[nHBoneCnt].TM_ROW3[0],pHAnmHead[nHBoneCnt].TM_ROW3[1],pHAnmHead[nHBoneCnt].TM_ROW3[2]);
			fprintf(r,"\t\t*TM_POS %f %f %f\n",\
				pHAnmHead[nHBoneCnt].m_initPos[0],\
				pHAnmHead[nHBoneCnt].m_initPos[1],\
				pHAnmHead[nHBoneCnt].m_initPos[2]);
			fprintf(r,"\t\t*TM_ROTAXIS %f %f %f\n",\
				pHAnmHead[nHBoneCnt].TM_ROTAXIS[0],\
				pHAnmHead[nHBoneCnt].TM_ROTAXIS[1],\
				pHAnmHead[nHBoneCnt].TM_ROTAXIS[2]);
			fprintf(r,"\t\t*TM_ROTANGLE %0.5f\n",pHAnmHead[nHBoneCnt].TM_ROTANGLE);
			fprintf(r,"\t\t*TM_QUATERNION w=%f (%f %f %f)\n",\
				pHAnmHead[nHBoneCnt].m_initRot[0],\
				pHAnmHead[nHBoneCnt].m_initRot[1],\
				pHAnmHead[nHBoneCnt].m_initRot[2],\
				pHAnmHead[nHBoneCnt].m_initRot[3]);
			fprintf(r,"\t\t*TM_SCALE %f %f %f\n",\
				pHAnmHead[nHBoneCnt].m_initScale[0],\
				pHAnmHead[nHBoneCnt].m_initScale[1],\
				pHAnmHead[nHBoneCnt].m_initScale[2]);

			fprintf(r,"\t\t*TM_SCALEAXIS %f %f %f\n",\
				pHAnmHead[nHBoneCnt].TM_SCALEAXIS[0],\
				pHAnmHead[nHBoneCnt].TM_SCALEAXIS[1],\
				pHAnmHead[nHBoneCnt].TM_SCALEAXIS[2]);
			fprintf(r,"\t\t*TM_SCALEAXISANG %0.5f\n\t}\n",pHAnmHead[nHBoneCnt].TM_SCALEAXISANG);
			fprintf(r,"\t*BOUNDINGBOX_MIN %f %f %f\n",\
				pHAnmHead[nHBoneCnt].BOUNDINGBOX_MIN[0],\
				pHAnmHead[nHBoneCnt].BOUNDINGBOX_MIN[1],\
				pHAnmHead[nHBoneCnt].BOUNDINGBOX_MIN[2]);
			fprintf(r,"\t*BOUNDINGBOX_MAX %f %f %f\n",\
				pHAnmHead[nHBoneCnt].BOUNDINGBOX_MAX[0],\
				pHAnmHead[nHBoneCnt].BOUNDINGBOX_MAX[1],\
				pHAnmHead[nHBoneCnt].BOUNDINGBOX_MAX[2]);


			if( pHAnmHead[nHBoneCnt].m_numPoskey == 0 && pHAnmHead[nHBoneCnt].m_numRotkey == 0 )
			{
				fprintf(r,"}\n");
				continue;
			}
			else
			{
				fprintf(r,"\t%s {\n","*TM_ANIMATION");
			}
			fprintf(r,"\t\t*NODE_NAME \"%s\"\n",pHAnmHead[nHBoneCnt].m_nodeName);

			if( pHAnmHead[nHBoneCnt].m_numPoskey != 0 )
			{
				fprintf(r,"\t\t%s {\n","*CONTROL_POS_TRACK");
				nHPosCnt++;
				//pos data
				int index = nHPosCnt*nClipNum;
				for(int i = 0;i < nClipNum;i++)
				{
					fprintf(r,"\t\t\t*CONTROL_POS_SAMPLE %d %f %f %f\n",\
						(pActHead->m_tpf)*(pActHead->m_firstFrame+i),\
						hposkey[index+i].x,hposkey[index+i].y,hposkey[index+i].z);
				}

				fprintf(r,"\t\t}\n");
			}
			if( pHAnmHead[nHBoneCnt].m_numRotkey != 0 )
			{
				fprintf(r,"\t\t%s {\n","*CONTROL_ROT_TRACK");
				nHRotCnt++;
				//rot data
				int index = nHRotCnt*nClipNum;
				for(int i = 0;i < nClipNum;i++)
				{
					fprintf(r,"\t\t\t*CONTROL_ROT_SAMPLE %d %0.5f %0.5f %0.5f %0.5f\n",\
						(pActHead->m_tpf)*(pActHead->m_firstFrame+i),\
						haapkey[index+i].x,haapkey[index+i].y,haapkey[index+i].z,haapkey[index+i].angle);
					fprintf(r,"\t\t\t*CONTROL_ROT_QUATERNION %d\tw=%f (%f %f %f)\n",\
						(pActHead->m_tpf)*(pActHead->m_firstFrame+i),\
						hrotkey[index+i].w,hrotkey[index+i].x,hrotkey[index+i].y,hrotkey[index+i].z);
				}
				fprintf(r,"\t\t}\n");
			}
			fprintf(r,"\t}\n}\n");
		}
	}
    assert(nRotCnt == (signed)pActHead->m_boneRotCount-1);
    assert(nPosCnt == (signed)pActHead->m_bonePosCount-1);
    assert(nBoneCnt == (signed)pActHead->m_boneAnmCount-1);
	assert(nHRotCnt == (signed)pActHead->m_helpRotCount-1);
    assert(nHPosCnt == (signed)pActHead->m_helpPosCount-1);
    assert(nHBoneCnt == (signed)pActHead->m_helpAnmCount-1);
    fclose(r); r = 0;
}
