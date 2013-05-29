
/**
 * func.h
 *
 * Copyright (c) 2013, Dalian Nationalities University. All Rights  Reserved.
 * Tianyi Song <songtianyi630@163.com>
 *
 * You can use this library in your project, but do not redistribute it and/or modify
 * it.
 *
 */


#ifndef FUNC_H
#define FUNC_H

#include <string.h>
#include "CVector3f.h"
#include "CQuaternion.h"

/*
 *@brief matrix multiplication
 *@input matrix a, row = X col = Y
 *@input matrix b, row = Y col = Z
 *@output matrix res, row = X col = Z
 **/
void	__matrixMultiXYZ(FLOAT *res,const FLOAT *a,const FLOAT *b,const int X,const int Y,const int Z);


/*
 *
 */
void	Pitch(CVector3f tarVec,CVector3f curVec,FLOAT &pitch);//x

void 	Yaw(CVector3f tarVec,CVector3f curVec,FLOAT &yaw);//y

void 	Roll(CVector3f tarVec,CVector3f curVec,FLOAT &roll);//z



/*
 *@brief Cyclic-Coordinate Descent ,calculate a quaternion
 ,which can rotate the current vec to target vec
 *
 */
CQuaternion CCD(CVector3f root,CVector3f curEnd,CVector3f desireEnd);


#endif
