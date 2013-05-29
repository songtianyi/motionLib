#include "CQuaternion.h"
#include "CVector3f.h"
#include "HMat44.h"
#include "HMat33.h"
#include "AAP.h"

CQuaternion::CQuaternion()
{
    //ctor
}
void CQuaternion::print() const
{
    #ifdef FLOAT_32
	printf("(w = %f,%f %f %f)",w,x,y,z);
	#endif

	#ifdef FLOAT_64
	printf("(w = %0.9f,%0.9f %0.9f %f)",w,x,y,z);
	#endif // FLOAT_64
}
CQuaternion::CQuaternion(const FLOAT x,const FLOAT y,const FLOAT z,const FLOAT w)
{
    this->w = w; this->x = x; this->y = y; this->z = z;
}

CQuaternion::~CQuaternion()
{
    //dtor
}
CQuaternion CQuaternion::conjugated() const
{
    return CQuaternion(-x,-y,-z,w);
}
void CQuaternion::conjugate()
{
    x = -x; y = -y; z = -z;
}
CQuaternion CQuaternion::log()
{
	FLOAT a = static_cast<FLOAT>(acos(w));
	FLOAT sina = static_cast<FLOAT>(sin(a));
	CQuaternion ret;
	ret.w = 0;
	if (sina > 0)
	{
		ret.x = a*x/sina;
		ret.y = a*y/sina;
		ret.z = a*z/sina;
	}
	else
	{
		ret.x=ret.y=ret.z=0;
	}
	return ret;
}
CQuaternion CQuaternion::exp()
{
	FLOAT a = static_cast<FLOAT>(sqrt(x*x + y*y + z*z));
	FLOAT sina = static_cast<FLOAT>(sin(a));
	FLOAT cosa = static_cast<FLOAT>(cos(a));
	CQuaternion ret;

	ret.w = cosa;
	if(a > 0)
	{
		ret.x = sina * x / a;
		ret.y = sina * y / a;
		ret.z = sina * z / a;
	}
	else
	{
		ret.x = ret.y = ret.z = 0;
	}

	return ret;
}


CQuaternion CQuaternion::normalized() const
{
    FLOAT squmrt = sqrt(w*w+x*x+y*y+z*z);
    if( squmrt )
        return CQuaternion(x/squmrt,y/squmrt,z/squmrt,w/squmrt);
    return *this;
}
void CQuaternion::normalize()
{
    FLOAT squmrt = sqrt(w*w+x*x+y*y+z*z);
	if(  squmrt )
	{
        w = w/squmrt; x = x/squmrt; y = y/squmrt; z = z/squmrt;
	}
}
CVector3f CQuaternion::toEulerAngle(const int type)
{
    //
    CQuaternion r = *this;
    if(type == INERTIATOOBJECT)
    {
        r = this->conjugated();
    }
    //object to Inertia
    FLOAT sp = -2.0 * (r.y*r.z - r.w*r.x);
    FLOAT p,h,b;//x,y,z;
    if(fabs(sp) > 0.999999)
    {
        p = 1.5707963267948965 * sp;
        h = atan2((FLOAT) (-r.x*r.z + r.w*r.y),(FLOAT)(0.5-r.y*r.y-r.z*r.z));
        b = 0.0;
    }
    else{
        p = H3DMath::ASin64(sp);
        h = atan2((FLOAT)r.x*r.z + r.w*r.y,(FLOAT)0.5 - r.x*r.x - r.y*r.y);
        b = atan2((FLOAT)r.x*r.y + r.w*r.z,(FLOAT)0.5 - r.x*r.x - r.z*r.z);
    }
    return CVector3f(p,h,b);
}

HMat33 CQuaternion::toHMat33( void ) const {
	HMat33	mat;
	FLOAT	wx, wy, wz;
	FLOAT	xx, yy, yz;
	FLOAT	xy, xz, zz;
	FLOAT	x2, y2, z2;

	x2 = x + x;
	y2 = y + y;
	z2 = z + z;

	xx = x * x2;//x*2*x
	xy = x * y2;//x*2*y
	xz = x * z2;//x*2*z

	yy = y * y2;//y*2*y
	yz = y * z2;//y*2*z
	zz = z * z2;

	wx = w * x2;
	wy = w * y2;
	wz = w * z2;

	mat[ 0 ][ 0 ] = 1.0f - ( yy + zz );
	mat[ 0 ][ 1 ] = xy - wz;
	mat[ 0 ][ 2 ] = xz + wy;

	mat[ 1 ][ 0 ] = xy + wz;
	mat[ 1 ][ 1 ] = 1.0f - ( xx + zz );
	mat[ 1 ][ 2 ] = yz - wx;

	mat[ 2 ][ 0 ] = xz - wy;
	mat[ 2 ][ 1 ] = yz + wx;
	mat[ 2 ][ 2 ] = 1.0f - ( xx + yy );

	return mat;
}

HMat44 CQuaternion::toHMat44( void ) const {
	HMat44	t;
	FLOAT	wx, wy, wz;
	FLOAT	xx, yy, yz;
	FLOAT	xy, xz, zz;
	FLOAT	x2, y2, z2;

	x2 = x + x;
	y2 = y + y;
	z2 = z + z;

	xx = x * x2;//x*2*x
	xy = x * y2;//x*2*y
	xz = x * z2;//x*2*z

	yy = y * y2;//y*2*y
	yz = y * z2;//y*2*z
	zz = z * z2;

	wx = w * x2;
	wy = w * y2;
	wz = w * z2;

	t.mat[ 0 ][ 0 ] = 1.0f - ( yy + zz );
	t.mat[ 1 ][ 0 ] = xy - wz;
	t.mat[ 2 ][ 0 ] = xz + wy;
	t.mat[ 3 ][ 0 ] = 0;

	t.mat[ 0 ][ 1 ] = xy + wz;
	t.mat[ 1 ][ 1 ] = 1.0f - ( xx + zz );
	t.mat[ 2 ][ 1 ] = yz - wx;
	t.mat[ 3 ][ 1 ] = 0;

	t.mat[ 0 ][ 2 ] = xz - wy;
	t.mat[ 1 ][ 2 ] = yz + wx;
	t.mat[ 2 ][ 2 ] = 1.0f - ( xx + yy );
	t.mat[ 3 ][ 2 ] = 0;


	t.mat[ 0 ][ 3 ] = 0;
	t.mat[ 1 ][ 3 ] = 0;
	t.mat[ 2 ][ 3 ] = 0;
	t.mat[ 3 ][ 3 ] = 1;



	return t;
}
