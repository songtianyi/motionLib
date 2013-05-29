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

/**
 *@brief class CAAP, Euler axis and angle
 */
#ifndef AAP_H
#define AAP_H

#include "CVector3f.h"
#include "CQuaternion.h"
#include "def.h"


class CAAP
{
public:
	CAAP(void);
	CAAP(const FLOAT theta,const FLOAT x,const FLOAT y,const FLOAT z);  /*constructor*/
	~CAAP(void);
	CQuaternion				toQuat  ( void ) const;                 /*transform euler axis and angle pair to quaternion*/
	CQuaternion				exp     ( void ) const;                 /*exponential,defined in <3D math primer for*/
                                                                    /*graphics and game development> page 170*/
public:
	FLOAT angle,x,y,z;                                              /*rotation angle in degree, rotation vector(x,y,z)*/
};



#endif
