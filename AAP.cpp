#include "AAP.h"

CAAP::CAAP(void)
{
}
CAAP::CAAP(const FLOAT theta,const FLOAT x,const FLOAT y,const FLOAT z)
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
	FLOAT a, s, c;
	a = angle * ( H3DMath::M_DEG2RAD * 0.5f );
	s = sin( a ); c = cos( a );
	return CQuaternion( ( FLOAT )x*s,( FLOAT )y*s,( FLOAT )z*s,c);
}
CQuaternion CAAP::exp()const
{
	return this->toQuat();
}
