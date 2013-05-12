#ifndef FUNC_H
#define FUNC_H

#include <string.h>

/*
 *@brief matrix multiplication
 *@input: matrix a,row = X col = Y
 *@input: matrix b,row = Y col = Z
 *@output: res, row = X col = Z
 **/
void __matrixMultiXYZ(float *res,const float *a,const float *b,const int X,const int Y,const int Z);

/*
 *
 */
void Pitch(CVector3f tarVec,CVector3f curVec,float &pitch)//x

void 	Yaw(CVector3f tarVec,CVector3f curVec,float &yaw)//y

void 	Roll(CVector3f tarVec,CVector3f curVec,float &roll)//z
#endif