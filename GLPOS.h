#ifndef GLPOS_H
#define GLPOS_H

#include "CASEParser.h"
#include "HMat44.h"
#include "HMat33.h"
#include "func.h"

/**
 *@brief calculate global position of ASE file objects
 */
class GLPOS
{
public:
	GLPOS(void);
	~GLPOS(void);
	/*get global position, exclude helper object*/
	void getGLPos(CVector3f *glPos,const ActionHeader *pAct,const AnmHeader *pAnm,const CQuaternion *rotkey,const CVector3f *poskey);
	
	/*calculate all frames of certain object*/
	/*input all poskey and rotkey*/
	void getGLPos(CVector3f *glPos,const ActionHeader *pAct,const AnmHeader *pAnm,const CQuaternion *rotkey,const CVector3f *poskey,const int boneIndex);

	void __getParentNodesID(int *father,const ActionHeader *pAct,const AnmHeader *pAnm);
	void __getOffsetMat(float rs[4][4],int i,int p,const AnmHeader *pAnm);
	void __getOffsetMat(float rs[4],int i,int p,const AnmHeader *pAnm);
	void __44Transpose(float mat[4][4]);
};
#endif
