#ifndef CQUATINTERP_H
#define CQUATINTERP_H

#include <math.h>
#include "CQuaternion.h"
#include "stdio.h"

///Direct X
///#include <D3dx9math.h>
///#pragma comment(lib,"D3DX10.lib")

#pragma warning(disable:4244)

/**
 *@brief quaternion interpolation
 */

class CQuatInterp
{
public:
        CQuatInterp(void);
        ~CQuatInterp(void);
		
		/*spAnmcterical linear interpolation*/
        CQuaternion slerp(const CQuaternion &s, const CQuaternion &e,const float t);
		void slerp(CQuaternion &rs,const CQuaternion &s,const CQuaternion &ee,const float t);

		/*spAnmcterical and Quadrangle*/
		void squad(CQuaternion &rs,const CQuaternion &q0,const CQuaternion &q1,const CQuaternion &q2,const CQuaternion &q3,const float t);
		
		/*spAnmcterical bezier curve*/
		void sbezier(CQuaternion &rs,const CQuaternion &q0,const CQuaternion &q1,const CQuaternion &q2,const CQuaternion &q3,const float t);
		
		/*Catmull-Rom interpolation*/
		void qCatmullRom(CQuaternion &rs,const CQuaternion &q0,const CQuaternion &q1,const CQuaternion &q2,const CQuaternion &q3,const float t);

        void slerp(CQuaternion *rot,const int n);//interpolation between rot[0] rot[n-1]
		void squad(CQuaternion *rot,const int n,const CQuaternion &q0,const CQuaternion &q3);//rot[0] = q1,rot[n-1] = q2

private:
		CQuaternion _Bisect(const CQuaternion &a,const CQuaternion &b);
		CQuaternion _Double(const CQuaternion &a,const CQuaternion &b);

};


#endif // CQUATINTERP_H
