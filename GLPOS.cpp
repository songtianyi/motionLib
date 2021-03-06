#include "GLPOS.h"

GLPOS::GLPOS(void)
{
}

GLPOS::~GLPOS(void)
{
}


void GLPOS::__getParentNodesID(int *father,const ActionHeader *pAct,const AnmHeader *pAnm)
{
		father[0] = -1;
		for(int j = 0;j < pAct->m_boneAnmCount;j++)
		{
			int flag = 0;
			for(int k = j-1;k >= 0;k--)
			{
				if( strcmp(pAnm[j].m_nodeParentName,pAnm[k].m_nodeName) == 0 )
				{
					father[j] = k;
					flag = 1;
					break;
				}
			}
			if(j == 0) flag = 1;
			assert(flag == 1);
		}
}

void GLPOS::__getOffsetMat(FLOAT rs[4][4],int i,int p,const AnmHeader *pAnm)
{
	//left hand
	//ith bone jth frame
	HMat44 self(
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		pAnm[i].TM_ROW3[0],pAnm[i].TM_ROW3[1],pAnm[i].TM_ROW3[2],1
	);
	HMat44 father(
		pAnm[ p ].TM_ROW0[0],pAnm[ p ].TM_ROW0[1],pAnm[ p ].TM_ROW0[2],0,
		pAnm[ p ].TM_ROW1[0],pAnm[ p ].TM_ROW1[1],pAnm[ p ].TM_ROW1[2],0,
		pAnm[ p ].TM_ROW2[0],pAnm[ p ].TM_ROW2[1],pAnm[ p ].TM_ROW2[2],0,
		pAnm[ p ].TM_ROW3[0],pAnm[ p ].TM_ROW3[1],pAnm[ p ].TM_ROW3[2],1
	);

	father.InverseFastSelf();
	__matrixMultiXYZ((FLOAT *)rs,(const FLOAT *)self.mat,(const FLOAT *)father.mat,4,4,4);
}
void GLPOS::__getOffsetMat(FLOAT rs[4],int i,int p,const AnmHeader *pAnm)
{
	FLOAT rs1[4][4];
	__getOffsetMat(rs1,i,p,pAnm);
	rs[0] = rs1[3][0];
	rs[1] = rs1[3][1];
	rs[2] = rs1[3][2];
	rs[3] = 1;
}
void GLPOS::__44Transpose(FLOAT mat[4][4])
{
	for(int i = 0;i < 3;i++)
	{
		for(int j = i+1;j < 4;j++)
		{
			FLOAT t = mat[i][j];
			mat[i][j] = mat[j][i];
			mat[j][i] = t;
		}
	}
}

void GLPOS::getGLPos(CVector3f *glPos,\
					 const ActionHeader *pAct,const AnmHeader *pAnm,\
					 const CQuaternion *rotkey,const CVector3f *poskey)
{
		int *parent_of = new int[pAct->m_boneAnmCount];
		__getParentNodesID(parent_of,pAct,pAnm);

		//
		FLOAT *M = new FLOAT[pAct->m_boneAnmCount*16];
		int nClipNum = pAct->m_lastFrame - pAct->m_firstFrame + 1;

		for(int i = 0;i < nClipNum;i++)
		{
				int posCnt = 0,rotCnt = 0;

				for(int j = 0;j < pAct->m_boneAnmCount;j++)
				{
					//cal trans mat
					FLOAT offset[4][4];
					FLOAT trans[4][4];
					{
						if( pAnm[j].m_numPoskey != 0 )
						{
							FLOAT tmp[4][4] = {
								{1,0,0,poskey[posCnt*nClipNum + i].x},
								{0,1,0,poskey[posCnt*nClipNum + i].y},
								{0,0,1,poskey[posCnt*nClipNum + i].z},
								{0,0,0,1}//right
							};
							memcpy(offset,tmp,sizeof(tmp));
						}
						else
						{
							__getOffsetMat(offset,j,parent_of[j],pAnm);
							__44Transpose(offset);//to right hand

							offset[0][0] = 1; offset[0][1] = 0; offset[0][2] = 0;
							offset[1][0] = 0; offset[1][1] = 1; offset[1][2] = 0;
							offset[2][0] = 0; offset[2][1] = 0; offset[2][2] = 1;
						}

						HMat33 htmp;
						if(pAnm[j].m_numRotkey != 0)
						{
							htmp = rotkey[rotCnt*nClipNum+i].toHMat33();
						}
						else
						{
							CQuaternion qt(pAnm[j].m_initRot[1],pAnm[j].m_initRot[2],pAnm[j].m_initRot[3],pAnm[j].m_initRot[0]);
							htmp = qt.toHMat33();
						}
						FLOAT ftmp[4][4] =
						{
							{ htmp[0].x,htmp[0].y,htmp[0].z,0 },
							{ htmp[1].x,htmp[1].y,htmp[1].z,0 },
							{ htmp[2].x,htmp[2].y,htmp[2].z,0 },
							{ 0,0,0,1 }
						};

						__matrixMultiXYZ((FLOAT *)trans,(const FLOAT *)offset,(const FLOAT *)ftmp,4,4,4);//T*R

						if(j == 0)
						{
							memcpy(&M[ j*16 ],trans,sizeof(trans));
						}
						else
						{
							__matrixMultiXYZ((FLOAT *)&M[ j*16 ],(const FLOAT *)&M[ parent_of[j]*16 ],(const FLOAT *)trans,4,4,4);
						}
					}

					//cal glpos
					if(j == 0)
					{
						FLOAT E[4] = {0,0,0,1};
						__matrixMultiXYZ(E,(const FLOAT *)&M[j*16],(const FLOAT *)E,4,4,1);
						memcpy(&glPos[j*nClipNum+i],E,3*sizeof(FLOAT));
					}
					else
					{
						FLOAT E[4] = {0,0,0,1};
						 __matrixMultiXYZ(E,(const FLOAT *)& M[ j*16 ],(const FLOAT *)E,4,4,1);
						memcpy(&glPos[j*nClipNum+i],E,3*sizeof(FLOAT));
					}

					if(pAnm[j].m_numRotkey != 0)
					{
						rotCnt++;
					}
					if(pAnm[j].m_numPoskey != 0)
					{
						posCnt++;
					}
			}

		}
		delete []M; M = 0;
		delete [] parent_of; parent_of = 0;
}

void GLPOS::getGLPos(CVector3f *glPos,\
			  const ActionHeader *pAct,const AnmHeader *pAnm,\
			  const CQuaternion *rotkey,const CVector3f *poskey,const int boneIndex)
{
		int *parent_of = new int[pAct->m_boneAnmCount];
		__getParentNodesID(parent_of,pAct,pAnm);

		int *node_stack = new int[pAct->m_boneAnmCount];
		int stack_index = 0;
		int curr_bone = boneIndex;
		while(curr_bone != -1)
		{
			node_stack[stack_index++] =curr_bone;
			curr_bone = parent_of[curr_bone];
		}

		FLOAT *M = new FLOAT[16];
		int nClipNum = pAct->m_lastFrame - pAct->m_firstFrame + 1;

		for(int i = 0;i < nClipNum;i++)
		{
				for(int j = stack_index-1;j >= 0;j--)
				{
					//cal trans mat
					FLOAT offset[4][4];
					FLOAT trans[4][4];
					{
						if( pAnm[ node_stack[j] ].m_numPoskey != 0 )
						{
							int posCnt = pAct->m_bonePosIndex[ node_stack[j] ];
							FLOAT tmp[4][4] = {
								{1,0,0,poskey[posCnt*nClipNum + i].x},
								{0,1,0,poskey[posCnt*nClipNum + i].y},
								{0,0,1,poskey[posCnt*nClipNum + i].z},
								{0,0,0,1}
							};
							memcpy(offset,tmp,sizeof(tmp));
						}
						else
						{
							__getOffsetMat(offset,node_stack[j],parent_of[node_stack[j]],pAnm);
							__44Transpose(offset);//to right hand

							offset[0][0] = 1; offset[0][1] = 0; offset[0][2] = 0;
							offset[1][0] = 0; offset[1][1] = 1; offset[1][2] = 0;
							offset[2][0] = 0; offset[2][1] = 0; offset[2][2] = 1;
						}

						HMat33 htmp;
						if(pAnm[node_stack[j]].m_numRotkey != 0)
						{
							int rotCnt = pAct->m_boneRotIndex[node_stack[j]];
							htmp = rotkey[rotCnt*nClipNum+i].toHMat33();
						}
						else
						{
							CQuaternion qt(pAnm[node_stack[j]].m_initRot[1],pAnm[node_stack[j]].m_initRot[2],pAnm[node_stack[j]].m_initRot[3],pAnm[node_stack[j]].m_initRot[0]);
							htmp = qt.toHMat33();
						}
						FLOAT ftmp[4][4] =
						{
							{ htmp[0].x,htmp[0].y,htmp[0].z,0 },
							{ htmp[1].x,htmp[1].y,htmp[1].z,0 },
							{ htmp[2].x,htmp[2].y,htmp[2].z,0 },
							{ 0,0,0,1 }
						};

						__matrixMultiXYZ((FLOAT *)trans,(const FLOAT *)offset,(const FLOAT *)ftmp,4,4,4);//T*R
						if(node_stack[j] == 0)
							memcpy(M,trans,sizeof(trans));
						else
						{
							__matrixMultiXYZ((FLOAT *)M,(const FLOAT *)M,(const FLOAT *)trans,4,4,4);
						}
					}
			}

			//cal glpos
			{
				FLOAT E[4] = {0,0,0,1};
				__matrixMultiXYZ(E,(const FLOAT *)M,(const FLOAT *)E,4,4,1);
				memcpy(&glPos[i],E,3*sizeof(FLOAT));
			}
		}
		delete []M; M = 0;
		delete [] parent_of; parent_of = 0;
		delete [] node_stack; node_stack = 0;
}

void GLPOS::getGLPosOneFrameAllBone(CVector3f *glPos,\
					 const ActionHeader *pAct,const AnmHeader *pAnm,\
					 const CQuaternion *rotkey,const CVector3f *poskey,const int frameIndex)
{
		int *parent_of = new int[pAct->m_boneAnmCount];
		__getParentNodesID(parent_of,pAct,pAnm);

		//
		FLOAT *M = new FLOAT[pAct->m_boneAnmCount*16];
		int nClipNum = pAct->m_lastFrame - pAct->m_firstFrame + 1;

		for(int i = frameIndex;i < frameIndex+1;i++)
		{
				int posCnt = 0,rotCnt = 0;

				for(int j = 0;j < pAct->m_boneAnmCount;j++)
				{
					//cal trans mat
					FLOAT offset[4][4];
					FLOAT trans[4][4];
					{
						if( pAnm[j].m_numPoskey != 0 )
						{
							FLOAT tmp[4][4] = {
								{1,0,0,poskey[posCnt*nClipNum + i].x},
								{0,1,0,poskey[posCnt*nClipNum + i].y},
								{0,0,1,poskey[posCnt*nClipNum + i].z},
								{0,0,0,1}//right
							};
							memcpy(offset,tmp,sizeof(tmp));
						}
						else
						{
							__getOffsetMat(offset,j,parent_of[j],pAnm);
							__44Transpose(offset);//to right hand

							offset[0][0] = 1; offset[0][1] = 0; offset[0][2] = 0;
							offset[1][0] = 0; offset[1][1] = 1; offset[1][2] = 0;
							offset[2][0] = 0; offset[2][1] = 0; offset[2][2] = 1;
						}

						HMat33 htmp;
						if(pAnm[j].m_numRotkey != 0)
						{
							htmp = rotkey[rotCnt*nClipNum+i].toHMat33();
						}
						else
						{
							CQuaternion qt(pAnm[j].m_initRot[1],pAnm[j].m_initRot[2],pAnm[j].m_initRot[3],pAnm[j].m_initRot[0]);
							htmp = qt.toHMat33();
						}
						FLOAT ftmp[4][4] =
						{
							{ htmp[0].x,htmp[0].y,htmp[0].z,0 },
							{ htmp[1].x,htmp[1].y,htmp[1].z,0 },
							{ htmp[2].x,htmp[2].y,htmp[2].z,0 },
							{ 0,0,0,1 }
						};

						__matrixMultiXYZ((FLOAT *)trans,(const FLOAT *)offset,(const FLOAT *)ftmp,4,4,4);//T*R

						if(j == 0)
						{
							memcpy(&M[ j*16 ],trans,sizeof(trans));
						}
						else
						{
							__matrixMultiXYZ((FLOAT *)&M[ j*16 ],(const FLOAT *)&M[ parent_of[j]*16 ],(const FLOAT *)trans,4,4,4);
						}
					}

					//cal glpos
					if(j == 0)
					{
						FLOAT E[4] = {0,0,0,1};
						__matrixMultiXYZ(E,(const FLOAT *)&M[j*16],(const FLOAT *)E,4,4,1);
						memcpy(&glPos[j*nClipNum+i],E,3*sizeof(FLOAT));
					}
					else
					{
						FLOAT E[4] = {0,0,0,1};
						 __matrixMultiXYZ(E,(const FLOAT *)& M[ j*16 ],(const FLOAT *)E,4,4,1);
						memcpy(&glPos[j*nClipNum+i],E,3*sizeof(FLOAT));
					}

					if(pAnm[j].m_numRotkey != 0)
					{
						rotCnt++;
					}
					if(pAnm[j].m_numPoskey != 0)
					{
						posCnt++;
					}
			}

		}
		delete []M; M = 0;
		delete [] parent_of; parent_of = 0;
}

void GLPOS::_getTransMatrix(FLOAT rs[][4],int f,int j,int end_num,\
                     const HBVHHead *pHead,\
                     const HBVHJoint *pJoints,\
                     const CVector3f *mat)
{
    ///T*R

    //rs = T
    FLOAT T[4][4] = {
        {1,0,0,pJoints[j].m_offset[0]},
        {0,1,0,pJoints[j].m_offset[1]},
        {0,0,1,pJoints[j].m_offset[2]},
        {0,0,0,1}};
    memcpy(rs,T,sizeof(T));

    //rs = rs*R
    FLOAT vec[3];
    vec[0] = mat[f*pHead->m_columNum/3].x;
    vec[1] = mat[f*pHead->m_columNum/3].y;
    vec[2] = mat[f*pHead->m_columNum/3].z;
    int i = 0;
    if(pJoints[j].m_channelNum == 6)
    {
        //root joint
        for(;i < 3;i++)
        {
            //position
            if(pJoints[j].m_channels[i] == 0)
            {
                //Xposition
                FLOAT t[4][4] = {
                    {1,0,0,vec[i]},
                    {0,1,0,0},
                    {0,0,1,0},
                    {0,0,0,1} };
                __matrixMultiXYZ((FLOAT *)rs,(const FLOAT *)rs,(const FLOAT *)t,4,4,4);
            }
            else if(pJoints[j].m_channels[i] == 1)
            {
                //Yposition
                FLOAT t[4][4] = {
                    {1,0,0,0},
                    {0,1,0,vec[i]},
                    {0,0,1,0},
                    {0,0,0,1} };
                __matrixMultiXYZ((FLOAT *)rs,(const FLOAT *)rs,(const FLOAT *)t,4,4,4);
            }
            else{
                //Zposition
                FLOAT t[4][4] = {
                    {1,0,0,0},
                    {0,1,0,0},
                    {0,0,1,vec[i]},
                    {0,0,0,1} };
                __matrixMultiXYZ((FLOAT *)rs,(const FLOAT *)rs,(const FLOAT *)t,4,4,4);
            }
        }
    }

    int off_col = (j+1-end_num);
    vec[0] = mat[f*pHead->m_columNum/3 +  off_col].x;
    vec[1] = mat[f*pHead->m_columNum/3 +  off_col].y;
    vec[2] = mat[f*pHead->m_columNum/3 +  off_col].z;

    for(int k = 0;k < 3;i++,k++)
    {
        //rotation
        FLOAT x = vec[k]*H3DMath::M_DEG2RAD;
        if(pJoints[j].m_channels[i] == 0)
        {
            //Xrotation
            FLOAT t[4][4] = { {1,0,0,0},{0,cos(x),-sin(x),0},{0,sin(x),cos(x),0},{0,0,0,1} };
            __matrixMultiXYZ((FLOAT *)rs,(const FLOAT *)rs,(const FLOAT *)t,4,4,4);
        }
        else if(pJoints[j].m_channels[i] == 1)
        {
            //Yrotation
            FLOAT t[4][4] = { {cos(x),0,sin(x),0},{0,1,0,0},{-sin(x),0,cos(x),0},{0,0,0,1} };
            __matrixMultiXYZ((FLOAT *)rs,(const FLOAT *)rs,(const FLOAT *)t,4,4,4);
        }
        else{
            //Zrotation
            FLOAT t[4][4] = { {cos(x),-sin(x),0,0},{sin(x),cos(x),0,0},{0,0,1,0},{0,0,0,1} };
            __matrixMultiXYZ((FLOAT *)rs,(const FLOAT *)rs,(const FLOAT *)t,4,4,4);
        }
    }
}


void GLPOS::getGLPos(CVector3f *glpos,const HBVHHead *pHead,const HBVHJoint *pJoints,const CVector3f *mat)
{
    const FLOAT unitM[4][4] = { {1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1} };//unit matrix
    FLOAT *M = new FLOAT[pHead->m_jointNum * 16];
    memcpy(&M[0*16],unitM,sizeof(unitM));
    for(int i = 0;i < pHead->m_frameNum;i++)
    {
        int end_num = 0;
        for(int j = 0;j < pHead->m_jointNum;j++)
        {
            if(pHead->m_isEndSite[j] == false)
            {
                //M[parent_of[j]]*R(channels[0])*R()...
                FLOAT trans[4][4];
                _getTransMatrix(trans,i,j,end_num,pHead,pJoints,mat);
                if(pHead->m_parentOf[j] == -1)
                {
                    memcpy(&M[j*16],trans,sizeof(trans));
                }
                else
                    __matrixMultiXYZ(&M[j*16], (const FLOAT *)&M[pHead->m_parentOf[j]*16], (const FLOAT *)trans,4,4,4);
            }
            else end_num++;

            //S[4][1] = {0,0,0,1}
            //E[4][1] = {offset[0],offset[1],offset[2],1};
            //E = M[parent_of[j]]*E  S = M[parent_of[j]]*S
            if(j == 0)
            {
                FLOAT S[4] = {0,0,0,1};
                __matrixMultiXYZ((FLOAT *)&S,(const FLOAT *)&M[0*16],(const FLOAT *)S,4,4,1);
                memcpy(&glpos[i*pHead->m_jointNum + 0],S,3*sizeof(FLOAT));
            }
            else{

                FLOAT E[4] = {pJoints[j].m_offset[0],pJoints[j].m_offset[1],pJoints[j].m_offset[2],1};
                __matrixMultiXYZ((FLOAT *)E,(const FLOAT *)&M[ pHead->m_parentOf[j]*16 ],(const FLOAT *)E,4,4,1);
                memcpy(&glpos[i*pHead->m_jointNum + j],E,3*sizeof(FLOAT));
            }
        }
    }
}
