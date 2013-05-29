
/**
 * PCA.h
 *
 * Copyright (c) 2013, Dalian Nationalities University. All Rights  Reserved.
 * Tianyi Song <songtianyi630@163.com>
 *
 * You can use this library in your project, but do not redistribute it and/or modify
 * it.
 *
 */

#ifndef PCA_H
#define PCA_H



#include <vector>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <cstdio>
#include <cassert>
using namespace std;

#include "H3DMath.h"
#include "def.h"

#define PCA_EPS            0.00000001    //PCA calucation accuracy
#define PCA_ITERATION      60          //iteration times
#define PCA_SIG_EXP		  -21          //


class PCA
{
public:
    PCA();
    ~PCA();
	void dimReduc(FLOAT *mat,const int row,const int col,int &dim,FLOAT *P);//reduce dim
	void dimIncrs(FLOAT *mat,const int row,const int dim,const int col,FLOAT *P);//increase dim
	void getCovaMat(const FLOAT *mat,const int row,const int col,FLOAT *covMat);

private:


	//calculate Eigenvector
    int _calEigenVector(FLOAT CovMatrix[], int n, FLOAT Eigen[], FLOAT EigenVector[]);
    void _Householder_Tri_Symetry_Diagonal(FLOAT a[], int n, FLOAT q[], FLOAT b[], FLOAT c[]);
    int _Tri_Symmetry_Diagonal_Eigenvector(int n, FLOAT b[], FLOAT c[], FLOAT q[], FLOAT eps, int l);

	//sort
	void _mySwap(int *a,int *b);
    void _quickSort(int left,int right,FLOAT array[],int id[]);

	//matrix operation
	void _matrixMulti(FLOAT *rs,const FLOAT *a,const FLOAT *b,const int r,const int s,const int t);
	void _matrixTransposition(FLOAT *m,const int row,const int col);
};

#endif
