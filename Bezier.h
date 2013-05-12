#ifndef BEZIER_H
#define BEZIER_H

#include "CVector3f.h"
#include <string.h>

class Bezier
{
public:
        Bezier(void);
        ~Bezier(void);

        /*calcualte cubic bezier control points with least square*/
        /*start point :q[0], end point q[n-1]*/
        void cubicBezierLeastsquare(const CVector3f *q,const int n,CVector3f &p1,CVector3f &p2);
		void cubicBezierLeastsquare(const double *q,const int n,const int dim,double *p1,double *p2);//high dimensional

        /*interpolation between q[0] and q[n-1],and measure error between q[i] and interpolation value*/
        int cubicBezierFitting(const CVector3f *q,const int n,const CVector3f &p1,const CVector3f &p2);
		int cubicBezierFitting(const double *q,const int n,const int dim,const double *P1,const double *P2);//high dimensional

        /*interpolate between q[0] and q[n-1]*/
        void cubicBezierInterp(CVector3f *q,const int n,const CVector3f &P1,const CVector3f &P2);
		void cubicBezierInterp(double *q,const int n,const int dim,const double *P1,const double *P2);

private:
        static const double CUBIC_FITTING_ERROR;
};



#endif // BEZIER_H
