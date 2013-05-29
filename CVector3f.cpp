
#include "CVector3f.h"
#include "CQuaternion.h"


CVector3f::CVector3f()
{
    //ctor
    x = y = z = 0.0f;
}
void CVector3f::setXYZ(const FLOAT x,const FLOAT y,const FLOAT z)
{
	this->x = x; this->y = y; this->z = z;
}
CVector3f::CVector3f(const FLOAT x,const FLOAT y,const FLOAT z)
{
    this->x = x; this->y = y; this->z = z;
}
CVector3f::~CVector3f()
{
    //dtor
}
void CVector3f::print() const
{
    #ifdef FLOAT_32
	printf("(%f %f %f)",x,y,z);
	#endif

	#ifdef FLOAT_64
	printf("(%0.9f %0.9f %0.9f)",x,y,z);
	#endif // FLOAT_64
}
void CVector3f::canonize()
{
    pitch   *= H3DMath::M_DEG2RAD;
    heading *= H3DMath::M_DEG2RAD;
    bank    *= H3DMath::M_DEG2RAD;

    pitch = WRAPPI(pitch);

    if(pitch < -H3DMath::HALF_PI)
    {
        pitch   = -(H3DMath::PI+pitch);
        heading += H3DMath::PI;
        bank    += H3DMath::PI;
    }
    else if(pitch > H3DMath::HALF_PI)
    {
        pitch   =  H3DMath::PI - pitch;
        heading += H3DMath::PI;
        bank    += H3DMath::PI;
    }
    //check gimbal lock
    if(fabs(pitch) > (H3DMath::HALF_PI - 1e-4))
    {
        heading += bank;
        bank = 0.0;
    }
    else
    {
        bank = WRAPPI(bank);
    }
    heading = WRAPPI(heading);

    assert(pitch    >= -H3DMath::HALF_PI    && pitch    <= H3DMath::HALF_PI);
    assert(heading  >= -H3DMath::PI         && heading  <= H3DMath::PI);
    assert(bank     >= -H3DMath::PI         && bank     <= H3DMath::PI);

    pitch   *= H3DMath::M_RAD2DEG;
    heading *= H3DMath::M_RAD2DEG;
    bank    *= H3DMath::M_RAD2DEG;
}

CVector3f CVector3f::canonized()const
{
    FLOAT tmp1 = pitch,tmp2 = heading,tmp3 = bank;

    tmp1    *= H3DMath::M_DEG2RAD;
    tmp2    *= H3DMath::M_DEG2RAD;
    tmp3    *= H3DMath::M_DEG2RAD;

    tmp1 = WRAPPI(tmp1);

    if(tmp1 < -H3DMath::HALF_PI)
    {
        tmp1    = -(H3DMath::PI+tmp1);
        tmp2    += H3DMath::PI;
        tmp3    += H3DMath::PI;
    }
    else if(tmp1 > H3DMath::HALF_PI)
    {
        tmp1    =  H3DMath::PI - tmp1;
        tmp2    += H3DMath::PI;
        tmp3    += H3DMath::PI;
    }
    //check gimbal lock
    if(fabs(tmp1) > (H3DMath::HALF_PI - 1e-4))
    {
        tmp2 += tmp3;
        tmp3 = 0.0;
    }
    else
    {
        tmp3 = WRAPPI(tmp3);
    }
    tmp2 = WRAPPI(tmp2);

    assert(tmp1 >= -H3DMath::HALF_PI    && tmp1 <= H3DMath::HALF_PI);
    assert(tmp2 >= -H3DMath::PI         && tmp2 <= H3DMath::PI);
    assert(tmp3 >= -H3DMath::PI         && tmp3 <= H3DMath::PI);

    tmp1 *= H3DMath::M_RAD2DEG;
    tmp2 *= H3DMath::M_RAD2DEG;
    tmp3 *= H3DMath::M_RAD2DEG;
    return CVector3f(tmp1,tmp2,tmp3);
}
CVector3f CVector3f::normalized() const
{
    FLOAT squmrt = sqrt((FLOAT)x*x+(FLOAT)y*y+(FLOAT)z*z);
    if( squmrt )
        return CVector3f( (FLOAT)x/squmrt,(FLOAT)y/squmrt,(FLOAT)z/squmrt);
    return *this;
}
void CVector3f::normalize()
{
    FLOAT squmrt = sqrt((FLOAT)x*x+(FLOAT)y*y+(FLOAT)z*z);
    if(  squmrt )
	{
        x = (FLOAT)x/squmrt; y = (FLOAT)y/squmrt; z = (FLOAT)z/squmrt;
	}
}
CQuaternion CVector3f::toQuat(const int type)
{
    //INERTIATOOBJECT
    CQuaternion rs;
    FLOAT p = x*H3DMath::M_DEG2RAD,h = y*H3DMath::M_DEG2RAD,b = z*H3DMath::M_DEG2RAD;
    rs.w = (FLOAT)( cos(h/2)*cos(p/2)*cos(b/2) + sin(h/2)*sin(p/2)*sin(b/2));
    rs.x = (FLOAT)(-cos(h/2)*sin(p/2)*cos(b/2) - sin(h/2)*cos(p/2)*sin(b/2));
    rs.y = (FLOAT)( cos(h/2)*sin(p/2)*sin(b/2) - sin(h/2)*cos(p/2)*cos(b/2));
    rs.z = (FLOAT)( sin(h/2)*sin(p/2)*cos(b/2) - cos(h/2)*cos(p/2)*sin(b/2));
    if(type == OBJECTTOINERTIA)
    {
        rs = rs.conjugated();
    }
    return rs;
}

