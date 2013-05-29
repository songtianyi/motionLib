
/**
 * GLPOS.h
 *
 * Copyright (c) 2013, Dalian Nationalities University. All Rights  Reserved.
 * Tianyi Song <songtianyi630@163.com>
 *
 * You can use this library in your project, but do not redistribute it and/or modify
 * it.
 *
 */


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

    void getGLPosOneFrameAllBone(CVector3f *glPos,\
					 const ActionHeader *pAct,const AnmHeader *pAnm,\
					 const CQuaternion *rotkey,const CVector3f *poskey,const int frameIndex);
	void __getParentNodesID(int *father,const ActionHeader *pAct,const AnmHeader *pAnm);
	void __getOffsetMat(FLOAT rs[4][4],int i,int p,const AnmHeader *pAnm);
	void __getOffsetMat(FLOAT rs[4],int i,int p,const AnmHeader *pAnm);
	void __44Transpose(FLOAT mat[4][4]);
};
#endif
