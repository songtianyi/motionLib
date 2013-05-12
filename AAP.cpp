#include "AAP.h"

CAAP::CAAP(void)
{
}
CAAP::CAAP(const float theta,const float x,const float y,const float z)
{
	this->angle = theta;
	this->x =x;
	this->y =y;
	this->z =z;
}


CAAP::~CAAP()
{
}

CQuaternion CAAP::toQuat() const {
	float a, s, c;
	a = angle * ( H3DMath::M_DEG2RAD * 0.5f );
	s = sin(a); c = cos(a);
	return CQuaternion(x*s,y*s,z*s,c);
}
CQuaternion CAAP::exp()const
{
	return this->toQuat();
}