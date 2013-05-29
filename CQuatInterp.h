
/**
 * AAP.h
 *
 * Copyright (c) 2013, Dalian Nationalities University. All Rights  Reserved.
 * Tianyi Song <songtianyi630@163.com>
 *
 * You can use this library in your project, but do not redistribute it and/or modify
 * it.
 *
 */


#ifndef CQUATINTERP_H
#define CQUATINTERP_H

#include <math.h>
#include "CQuaternion.h"
#include "stdio.h"

///Direct X
#ifdef DIRECTX_ENABLE
#include <D3dx9math.h>
#pragma comment(lib,"D3DX10.lib")
#endif

#ifdef MS_BUILD_ENV
#pragma warning(disable:4244)
#endif

/**
 *@brief quaternion interpolation
 */

class CQuatInterp
{
public:
        CQuatInterp(void);
        ~CQuatInterp(void);

		/*spherical linear interpolation*/
        CQuaternion slerp(const CQuaternion &s, const CQuaternion &e,const FLOAT t);
		void slerp(CQuaternion &rs,const CQuaternion &s,const CQuaternion &ee,const FLOAT t);

		/*spAnmcterical and Quadrangle*/
		void squad(CQuaternion &rs,const CQuaternion &q0,const CQuaternion &q1,const CQuaternion &q2,const CQuaternion &q3,const FLOAT t);

		/*spAnmcterical bezier curve*/
		void sbezier(CQuaternion &rs,const CQuaternion &q0,const CQuaternion &q1,const CQuaternion &q2,const CQuaternion &q3,const FLOAT t);

		/*Catmull-Rom interpolation*/
		void qCatmullRom(CQuaternion &rs,const CQuaternion &q0,const CQuaternion &q1,const CQuaternion &q2,const CQuaternion &q3,const FLOAT t);

        void slerp(CQuaternion *rot,const int n);//interpolation between rot[0] rot[n-1]
		void squad(CQuaternion *rot,const int n,const CQuaternion &q0,const CQuaternion &q3);//rot[0] = q1,rot[n-1] = q2

private:
		CQuaternion _Bisect(const CQuaternion &a,const CQuaternion &b);
		CQuaternion _Double(const CQuaternion &a,const CQuaternion &b);

};


#endif // CQUATINTERP_H
