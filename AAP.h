#ifndef AAP_H
#define AAP_H

#include "CVector3f.h"
#include "CQuaternion.h"
class CAAP//axis angle pair
{
public:
	CAAP(void);
	CAAP(const float theta,const float x,const float y,const float z);
	~CAAP(void);
	CQuaternion				toQuat( void ) const;
	CQuaternion				exp(void)const;
public:
	float angle,x,y,z;//degree , rotation axis
};



#endif