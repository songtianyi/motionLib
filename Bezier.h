
/**
 * Bezier.h
 *
 * Copyright (c) 2013, Dalian Nationalities University. All Rights  Reserved.
 * Tianyi Song <songtianyi630@163.com>
 *
 * You can use this library in your project, but do not redistribute it and/or modify
 * it.
 *
 */


#ifndef BEZIER_H
#define BEZIER_H


/**
 *@brief cubic bezier
 */


#include "CVector3f.h"
#include "def.h"
#include <string.h>

class Bezier
{
public:
        Bezier(void);
        ~Bezier(void);

        /*calcualte cubic bezier control points with least square*/
        /*start point :q[0], end point q[n-1]*/
        void cubicBezierLeastsquare(const CVector3f *q,const int n,CVector3f &p1,CVector3f &p2);
		void cubicBezierLeastsquare(const FLOAT *q,const int n,const int dim,FLOAT *p1,FLOAT *p2);//high dimensional

        /*interpolation between q[0] and q[n-1],and measure error between q[i] and interpolation value*/
        int cubicBezierFitting(const CVector3f *q,const int n,const CVector3f &p1,const CVector3f &p2);
		int cubicBezierFitting(const FLOAT *q,const int n,const int dim,const FLOAT *P1,const FLOAT *P2);//high dimensional

        /*interpolate between q[0] and q[n-1]*/
        void cubicBezierInterp(CVector3f *q,const int n,const CVector3f &P1,const CVector3f &P2);
		void cubicBezierInterp(FLOAT *q,const int n,const int dim,const FLOAT *P1,const FLOAT *P2);

private:
        static const FLOAT CUBIC_FITTING_ERROR;
};



#endif // BEZIER_H
