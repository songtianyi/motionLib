#ifndef __PCA_H__
#define __PCA_H__

/**
 *PCA
 */

#include <vector>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <cstdio>
#include <cassert>
using namespace std;

#include "H3DMath.h"

#define PCA_EPS            0.00000001    //PCA calucation accuracy
#define PCA_ITERATION      60          //iteration times
#define PCA_SIG_EXP		  -21          //


class PCA
{
public:
    PCA();
    ~PCA();
	void dimReduc(double *mat,const int row,const int col,int &dim,double *P);//reduce dim
	void dimIncrs(double *mat,const int row,const int dim,const int col,double *P);//increase dim
	void getCovaMat(const double *mat,const int row,const int col,double *covMat);

private:
	
	
	//calculate Eigenvector
    int _calEigenVector(double CovMatrix[], int n, double Eigen[], double EigenVector[]);
    void _Householder_Tri_Symetry_Diagonal(double a[], int n, double q[], double b[], double c[]);
    int _Tri_Symmetry_Diagonal_Eigenvector(int n, double b[], double c[], double q[], double eps, int l);
	
	//sort
	void _mySwap(int *a,int *b);
    void _quickSort(int left,int right,double array[],int id[]);
	
	//matrix operation
	void _matrixMulti(double *rs,const double *a,const double *b,const int r,const int s,const int t);
	void _matrixTransposition(double *m,const int row,const int col);
};

#endif
