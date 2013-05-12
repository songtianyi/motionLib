
#include "CVector3f.h"
#include "CQuaternion.h"


CVector3f::CVector3f()
{
    //ctor
    x = y = z = 0.0f;
}
CVector3f::CVector3f(const float x,const float y,const float z)
{
    this->x = x; this->y = y; this->z = z;
}
CVector3f::~CVector3f()
{
    //dtor
}
void CVector3f::print() const
{
	printf("(%f %f %f)",x,y,z);
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
    float tmp1 = pitch,tmp2 = heading,tmp3 = bank;

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
    double squmrt = sqrt((double)x*x+(double)y*y+(double)z*z);
    if( squmrt )
        return CVector3f( (double)x/squmrt,(double)y/squmrt,(double)z/squmrt);
    return *this;
}
void CVector3f::normalize()
{
    double squmrt = sqrt((double)x*x+(double)y*y+(double)z*z);
    if(  squmrt )
	{
        x = (double)x/squmrt; y = (double)y/squmrt; z = (double)z/squmrt;
	}
}
CQuaternion CVector3f::toQuat(const int type)
{
    //INERTIATOOBJECT
    CQuaternion rs;
    double p = x*H3DMath::M_DEG2RAD,h = y*H3DMath::M_DEG2RAD,b = z*H3DMath::M_DEG2RAD;
    rs.w = (float)( cos(h/2)*cos(p/2)*cos(b/2) + sin(h/2)*sin(p/2)*sin(b/2));
    rs.x = (float)(-cos(h/2)*sin(p/2)*cos(b/2) - sin(h/2)*cos(p/2)*sin(b/2));
    rs.y = (float)( cos(h/2)*sin(p/2)*sin(b/2) - sin(h/2)*cos(p/2)*cos(b/2));
    rs.z = (float)( sin(h/2)*sin(p/2)*cos(b/2) - cos(h/2)*cos(p/2)*sin(b/2));
    if(type == OBJECTTOINERTIA)
    {
        rs = rs.conjugated();
    }
    return rs;
}

