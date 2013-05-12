#include "SplineInterp.h"

SplineInterp::SplineInterp()
{
}
SplineInterp::~SplineInterp()
{
}
void SplineInterp::thomas(float *a,float *b,float *c,CVector3f *x,CVector3f *d,const int n)//[0,n-1]
{
	//a[1,n-1]
	//b[0,n-1]
	//c[0,n-2]
	for (int i = 1; i < n; i++)
	{
		float m = a[i]/b[i-1];
		b[i] = b[i] - m * c[i - 1];
		d[i] = d[i] - m*d[i-1];
	}
	 x[n-1] = d[n-1]/b[n-1];
	 
	for (int i = n - 2; i >= 0; --i)
		x[i] = (d[i] - c[i] * x[i+1]) / b[i];
}
void SplineInterp::getCubicSplinePolynomial(const float *x,const CVector3f *y,CVector3f *coef,const int n,const CVector3f &der0,const CVector3f &dern)//n个点, [0,n-1]
{
	float *h,*hs,*mu,*lambda;
	CVector3f *r,*d,*M;

	h = 0; hs = 0; mu = 0; lambda = 0;

	h = new float[n-1];
	hs= new float[n-1];
	mu = new float[n];
	lambda = new float[n];

	r = 0; d = 0;M = 0;

	r = new CVector3f[n-1];
	d = new CVector3f[n];
	M= new CVector3f[n];

	for(int i = 0;i <= n-2;i++)
	{
		h[i] = x[i+1] - x[i];//left
		r[i] = (y[i+1] - y[i])/h[i];//left
	}
	for(int i =1;i <= n-2;i++)
	{
		hs[i] = h[i-1] + h[i];//right
	}

	//lambda[n-1] = 1; mu[0] = 1;
	mu[0] = 1;
	for(int i = 1;i <= n-2;i++)
	{
		mu[i] = h[i]/hs[i];//right
		lambda[i] = 1- mu[i];
	}
	lambda[n-1] = 1;
	
	d[0] = 6*( r[0] - der0 )/h[0];
	for(int i = 1;i < n-1;i++)
	{
		d[i] = 6*(r[i] -r[i-1])/hs[i];
	}
	d[n-1] = 6*(dern - r[n-2])/h[n-2];
	
	
	// 2					mu[0]																|	| M[0]	|		|d[0]		|
	// lambda[1]		2					mu[1]											|	| M[1]	|		|d[1]		|
	// 					lambda[2]		2				mu[2]								|	| M[2]	|		|d[2]		|
	// 										lambda[3]	2			mu[3]					| *	| M[3]	| =	|d[3]		|
	//-	-	-	-	-	-	-		--	-	-	-	-	-	-	-	-	-	--	-				|	| ----		|		|----		|
	//															lambda[n-2]	2	mu[n-2]|	|M[n-2]	|		|d[n-2]	|
	//																	lambda[n-1]		2	|	|M[n-1]	|		|d[n-1]	|
	
	float *b = new float [n];
	for(int i = 0;i < n;i++)
		b[i] = 2;
	thomas(lambda,b,mu,M,d,n);
	delete [] b; b = 0;
	
	
	for(int i = 0;i < n-1;i++)
	{
		coef[i*4] = M[i]/(6*h[i]);
		coef[i*4 + 1] = M[i+1]/(6*h[i]);
		coef[i*4 + 2] = y[i]/h[i] - h[i]*M[i]/6;
		coef[i*4 + 3] = y[i+1]/h[i] - h[i]*M[i+1]/6;
	}
	

	delete [] h; h = 0;
	delete [] hs; hs = 0;
	delete [] mu; mu = 0;
	delete [] lambda; lambda = 0;

	delete [] r;  r = 0;
	delete []d; d = 0;
	delete []M;M= 0;
	assert(h == 0 && hs == 0 && mu == 0 && lambda == 0);
	assert(r == 0 && d == 0 && M == 0);
}

void SplineInterp::getCubicSplineInterpValue(const CVector3f *coef,const int i,const float p_x,const float *x,CVector3f &res_y)
{
	//S(x) = coef[*4 + 0] * (x[i+1] - x)^3 + coef[i*4 + 1] * (x - x[i])^3 + coef[i*4 + 2] * (x[i+1] - x) + coef[i*4 + 3] * (x - x[i])
	float diff_left = p_x - x[i];
	float diff_right=x[i+1] - p_x;
	res_y = (coef[i*4 + 0]*(diff_right*diff_right) +  coef[i*4 + 2])*(diff_right)+ (coef[i*4 + 1] * (diff_left*diff_left) + coef[i*4 + 3])*(diff_left);
}

