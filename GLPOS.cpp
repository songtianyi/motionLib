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

void GLPOS::__getOffsetMat(float rs[4][4],int i,int p,const AnmHeader *pAnm)
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
	__matrixMultiXYZ((float *)rs,(const float *)self.mat,(const float *)father.mat,4,4,4);
}
void GLPOS::__getOffsetMat(float rs[4],int i,int p,const AnmHeader *pAnm)
{
	float rs1[4][4];
	__getOffsetMat(rs1,i,p,pAnm);
	rs[0] = rs1[3][0];
	rs[1] = rs1[3][1];
	rs[2] = rs1[3][2];
	rs[3] = 1;
}
void GLPOS::__44Transpose(float mat[4][4])
{
	for(int i = 0;i < 3;i++)
	{
		for(int j = i+1;j < 4;j++)
		{
			float t = mat[i][j];
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
		float *M = new float[pAct->m_boneAnmCount*16];
		int nClipNum = pAct->m_lastFrame - pAct->m_firstFrame + 1;
		
		for(int i = 0;i < nClipNum;i++)
		{
				int posCnt = 0,rotCnt = 0;
				
				for(int j = 0;j < pAct->m_boneAnmCount;j++)
				{
					//cal trans mat
					float offset[4][4];
					float trans[4][4];
					{
						if( pAnm[j].m_numPoskey != 0 )
						{
							float tmp[4][4] = {
								{1,0,0,poskey[posCnt*nClipNum + i].x},
								{0,1,0,poskey[posCnt*nClipNum + i].y},
								{0,0,1,poskey[posCnt*nClipNum + i].z},
								{0,0,0,1}
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
						float ftmp[4][4] =
						{
							{ htmp[0].x,htmp[0].y,htmp[0].z,0 },
							{ htmp[1].x,htmp[1].y,htmp[1].z,0 },
							{ htmp[2].x,htmp[2].y,htmp[2].z,0 },
							{ 0,0,0,1 }
						};

						__matrixMultiXYZ((float *)trans,(const float *)offset,(const float *)ftmp,4,4,4);//T*R
						
						if(j == 0)
						{
							memcpy(&M[ j*16 ],trans,sizeof(trans));
						}
						else
						{
							__matrixMultiXYZ((float *)&M[ j*16 ],(const float *)&M[ parent_of[j]*16 ],(const float *)trans,4,4,4);
						}
					}

					//cal glpos
					if(j == 0)
					{
						float E[4] = {0,0,0,1};
						__matrixMultiXYZ(E,(const float *)&M[j*16],(const float *)E,4,4,1);
						memcpy(&glPos[j*nClipNum+i],E,3*sizeof(float));
					}
					else
					{
						//float E[4] = {offset[0][3],offset[1][3],offset[2][3],1};
						//__matrixMulti441(E,&M [ parent_of[j]*16 ],E );
						float E[4] = {0,0,0,1};
						 __matrixMultiXYZ(E,(const float *)& M[ j*16 ],(const float *)E,4,4,1);
						memcpy(&glPos[j*nClipNum+i],E,3*sizeof(float));
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

		float *M = new float[16];
		int nClipNum = pAct->m_lastFrame - pAct->m_firstFrame + 1;
		
		for(int i = 0;i < nClipNum;i++)
		{
				for(int j = stack_index-1;j >= 0;j--)
				{
					//cal trans mat
					float offset[4][4];
					float trans[4][4];
					{
						if( pAnm[ node_stack[j] ].m_numPoskey != 0 )
						{
							int posCnt = pAct->m_bonePosIndex[ node_stack[j] ];
							float tmp[4][4] = {
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
						float ftmp[4][4] =
						{
							{ htmp[0].x,htmp[0].y,htmp[0].z,0 },
							{ htmp[1].x,htmp[1].y,htmp[1].z,0 },
							{ htmp[2].x,htmp[2].y,htmp[2].z,0 },
							{ 0,0,0,1 }
						};

						__matrixMultiXYZ((float *)trans,(const float *)offset,(const float *)ftmp,4,4,4);//T*R
						if(node_stack[j] == 0)
							memcpy(M,trans,sizeof(trans));
						else
						{
							__matrixMultiXYZ((float *)M,(const float *)M,(const float *)trans,4,4,4);
						}
					}
			}

			//cal glpos
			{
				float E[4] = {0,0,0,1};
				__matrixMultiXYZ(E,(const float *)M,(const float *)E,4,4,1);
				memcpy(&glPos[i],E,3*sizeof(float));
			}
		}
		delete []M; M = 0;
		delete [] parent_of; parent_of = 0;
		delete [] node_stack; node_stack = 0;
}