#include "Bezier.h"

const double Bezier::CUBIC_FITTING_ERROR  = 0.000099;

Bezier::Bezier(void)
{
    //ctor
}

Bezier::~Bezier(void)
{
    //dtor
}


void Bezier::cubicBezierLeastsquare(const CVector3f *q,const int n,CVector3f &p1,CVector3f &p2)
{
    assert(n > 3);
    CVector3f p0 = q[0],p3 = q[n-1];

    double A1 = 0,A2 = 0, A12 = 0;
    CVector3f C1(0,0,0),C2(0,0,0);//0 0 0

    for(int i = 0;i < n;i++)
    {
        double t = i*1.0/(n-1);
        double t2   = t*t , t3 = t2*t , t4   = t3*t;

        double _t   = 1-t , _t2  = _t*_t , _t3  = _t2*_t, _t4  = _t3*_t;

        A1 += t2 * _t4;
        A2 += t4 * _t2;
        A12+= t3 * _t3;

        CVector3f tmp;
        tmp = (q[i] - _t3*p0 - t3*p3);
        C1 += ( 3*t*_t2*tmp );
        C2 += ( 3*t2*_t*tmp );
    }

    //P1 = (A2C1-A12C2)/(A1A2-A12A12)
    //P2 = (A1C2-A12C1)/(A1A2-A12A12)
    A1 *= 9; A2 *= 9; A12 *= 9;
    double base = A1*A2 - A12*A12;
    assert(base != 0);
    p1 = (A2*C1 - A12*C2)/base;
    p2 = (A1*C2 - A12*C1)/base;
}

void Bezier::cubicBezierLeastsquare(const double *q,const int n,const int dim,double *p1,double *p2)
{
    assert(n > 3);
    const double *p0 = &q[0*dim],*p3 = &q[(n-1)*dim];

    double A1 = 0,A2 = 0, A12 = 0;
	double *C1 = new double[dim]; memset(C1,0,sizeof(double)*dim);
	double *C2 = new double[dim]; memset(C2,0,sizeof(double)*dim);

    for(int i = 0;i < n;i++)
    {
        double t = i*1.0/(n-1);
        double t2   = t*t , t3 = t2*t , t4   = t3*t;
        double _t   = 1-t;
		double _t2  = _t*_t , _t3  = _t2*_t, _t4  = _t3*_t;

        A1 += t2 * _t4;
        A2 += t4 * _t2;
        A12+= t3 * _t3;
        
		for(int j = 0;j < dim;j++)
		{
			double tmp = (q[i*dim+j] - _t3*p0[j] - t3*p3[j]);
			C1[j] += ( 3*t*_t2*tmp );
			C2[j] += ( 3*t2*_t*tmp );
		}
    }

    //P1 = (A2C1-A12C2)/(A1A2-A12A12)
    //P2 = (A1C2-A12C1)/(A1A2-A12A12)
    A1 *= 9; A2 *= 9; A12 *= 9;
    double base = A1*A2 - A12*A12;
    assert(base != 0);
	for(int j = 0;j < dim;j++)
	{
		p1[j] = (A2*C1[j] - A12*C2[j])/base;
		p2[j] = (A1*C2[j] - A12*C1[j])/base;
	}

	delete [] C1; C1 = 0;
	delete [] C2; C2 = 0;
	p0 = p3 = 0;
}

int Bezier::cubicBezierFitting(const CVector3f *q,const int n,const CVector3f &P1,const CVector3f &P2)
{
    CVector3f P0 = q[0],P3 = q[n-1];
    //p(t) = P0 * (1-t)^3 + P1 * 3*t(1-t)^2 + P2 * 3*t^2*(1-t) + P3 * t^3
    CVector3f a,b,c,p;
    c.x = 3 * (P1.x - P0.x);
    c.y = 3 * (P1.y - P0.y);
    c.z = 3 * (P1.z - P0.z);
    b.x = 3 * (P2.x - P1.x) - c.x;
    b.y = 3 * (P2.y - P1.y) - c.y;
    b.z = 3 * (P2.z - P1.z) - c.z;
    a.x = P3.x - P0.x - c.x - b.x;
    a.y = P3.y - P0.y - c.y - b.y;
    a.z = P3.z - P0.z - c.z - b.z;
    double max_error = 0; int max_id = -1;

    for(int i = 1;i < n-1;i++)//zero-based
    {
        double t = i*1.0/(n-1);
        p.x = a.x * t * t * t + b.x * t * t + c.x * t + P0.x;
        p.y = a.y * t * t * t + b.y * t * t + c.y * t + P0.y;
        p.z = a.z * t * t * t + b.z * t * t + c.z * t + P0.z;

        double dist = manhattanDist(q[i],p)/3;//sum / 3

        if(H3DMath::complf(dist - max_error) > 0)
        {
            max_error = dist;
            max_id = i;
        }
    }
    if(H3DMath::complf(max_error - Bezier::CUBIC_FITTING_ERROR) > 0)
    {
        return max_id;
    }
    return -1;
}
int Bezier::cubicBezierFitting(const double *q,const int n,const int dim,const double *P1,const double *P2)
{
	const double *P0 = &q[0*dim],*P3 = &q[(n-1)*dim];
    //p(t) = P0 * (1-t)^3 + P1 * 3*t(1-t)^2 + P2 * 3*t^2*(1-t) + P3 * t^3
    double *a = new double[dim];
	double *b = new double[dim];
	double *c = new double[dim];
	double p;
	for(int j = 0;j < dim;j++)
	{
		c[j] = 3 * (P1[j] - P0[j]);
		b[j] = 3 * (P2[j] - P1[j]) - c[j];
		a[j] = P3[j] - P0[j] - c[j] - b[j];
	}

    double max_error = 0; int max_id = -1;

    for(int i = 1;i < n-1;i++)//zero-based
    {
        double t = i*1.0/(n-1);
		int index = i*dim;
		for(int j = 0;j < dim;j++)
		{
			p = a[j] * t * t * t + b[j] * t * t + c[j] * t + P0[j];
			p = fabs(q[index+j] - p);
			if(H3DMath::complf(p - max_error) > 0)
			{
				max_error = p;
				max_id = i;
			}
		}
    }
	delete [] a;a = 0;
	delete [] b;b = 0;
	delete [] c;c = 0;
    if(H3DMath::complf(max_error - Bezier::CUBIC_FITTING_ERROR) > 0)
    {
        return max_id;
    }
    return -1;
}
void Bezier::cubicBezierInterp(CVector3f *q,const int n,const CVector3f &P1,const CVector3f &P2)
{
    CVector3f P0 = q[0],P3 = q[n-1];

    CVector3f a,b,c,p;
    c.x = 3 * (P1.x - P0.x);
    c.y = 3 * (P1.y - P0.y);
    c.z = 3 * (P1.z - P0.z);
    b.x = 3 * (P2.x - P1.x) - c.x;
    b.y = 3 * (P2.y - P1.y) - c.y;
    b.z = 3 * (P2.z - P1.z) - c.z;
    a.x = P3.x - P0.x - c.x - b.x;
    a.y = P3.y - P0.y - c.y - b.y;
    a.z = P3.z - P0.z - c.z - b.z;

    for(int i = 1;i < n-1;i++)
    {
        double t = i*1.0/(n-1);
        q[i].x = a.x * t * t * t + b.x * t * t + c.x * t + P0.x;
        q[i].y = a.y * t * t * t + b.y * t * t + c.y * t + P0.y;
        q[i].z = a.z * t * t * t + b.z * t * t + c.z * t + P0.z;
    }
}
void Bezier::cubicBezierInterp(double *q,const int n,const int dim,const double *P1,const double *P2)
{
	const double *P0 = &q[0*dim],*P3 = &q[(n-1)*dim];
    //p(t) = P0 * (1-t)^3 + P1 * 3*t(1-t)^2 + P2 * 3*t^2*(1-t) + P3 * t^3
    double *a = new double[dim];
	double *b = new double[dim];
	double *c = new double[dim];

	for(int j = 0;j < dim;j++)
	{
		c[j] = 3 * (P1[j] - P0[j]);
		b[j] = 3 * (P2[j] - P1[j]) - c[j];
		a[j] = P3[j] - P0[j] - c[j] - b[j];
	}

    for(int i = 1;i < n-1;i++)//zero-based
    {
        double t = i*1.0/(n-1);
		int index = i*dim;
		for(int j = 0;j < dim;j++)
		{
			q[index+j] = a[j] * t * t * t + b[j] * t * t + c[j] * t + P0[j];
		}
    }
	delete [] a;a = 0;
	delete [] b;b = 0;
	delete [] c;c = 0;
}