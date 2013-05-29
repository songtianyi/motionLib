
/**
 * CQuaternion.h
 *
 * Copyright (c) 2013, Dalian Nationalities University. All Rights  Reserved.
 * Tianyi Song <songtianyi630@163.com>
 *
 * You can use this library in your project, but do not redistribute it and/or modify
 * it.
 *
 */

#ifndef CQUATERNION_H
#define CQUATERNION_H

#include <math.h>
#include "H3DMath.h"
#include "def.h"


#define OBJECTTOINERTIA 1
#define INERTIATOOBJECT 2


class CVector3f;
class HMat33;
class HMat44;
class CAAP;

/**
 *@brief quaternion operations
 *		result quaternions are not normalized!
 */
class CQuaternion
{
public:
        CQuaternion(void);
        CQuaternion(const FLOAT x,const FLOAT y,const FLOAT z,const FLOAT w);
        ~CQuaternion(void);

        FLOAT				operator[]( const int index ) const;
        FLOAT &				operator[]( const int index );
		CQuaternion			operator-() const;
		CQuaternion &		operator=( const CQuaternion &a );
		CQuaternion			operator+( const CQuaternion &a ) const;
		CQuaternion &		operator+=( const CQuaternion &a );
		CQuaternion			operator-( const CQuaternion &a ) const;//(x-a,y-a,z-a,w-a)
		CQuaternion &		operator-=( const CQuaternion &a );
		CQuaternion			operator*( const CQuaternion &a ) const;//cross product
		CQuaternion			operator*( const FLOAT a ) const;//(x*a,y*a,z*a,w*a)
		CQuaternion			operator/(const FLOAT a)const;//(x/a,y/a,z/a,w/a)
		CQuaternion &		operator*=( const CQuaternion &a );
		CQuaternion &		operator*=( const FLOAT a );


		friend CQuaternion	operator*( const FLOAT a, const CQuaternion &b );
		friend CQuaternion	diff(const CQuaternion &a,const CQuaternion &b);//if b*diff = a, calculate diff
		friend FLOAT		dotpdut(const CQuaternion &a,const CQuaternion &b);//dot product



        CVector3f toEulerAngle(const int type);// to euler angle
        HMat33 toHMat33(void) const;
		HMat44 toHMat44(void) const;

        CQuaternion conjugated(void) const;
        CQuaternion normalized(void) const;//not changed

		CQuaternion log(void);//keep itself as it was and return a new quaternion
		CQuaternion exp(void);//keep itself as it was and return a new quaternion

        void normalize(void);//quaternion changed
        void conjugate(void);//quaternion changed

		void print(void) const;

public:
        FLOAT w,x,y,z;

};

inline FLOAT CQuaternion::operator[]( const int index ) const {
	assert( ( index > -1 ) && ( index < 4 ) );
	return ( &x )[ index ];
}
inline FLOAT &CQuaternion::operator[]( const int index ) {
	assert( ( index > -1 ) && ( index < 4 ) );
	return ( &x )[ index ];
}

inline CQuaternion CQuaternion::operator-() const {
	return CQuaternion( -x, -y, -z, -w );
}

inline CQuaternion &CQuaternion::operator=( const CQuaternion &a ) {
	x = a.x;
	y = a.y;
	z = a.z;
	w = a.w;

	return (*this);
}

inline CQuaternion CQuaternion::operator+( const CQuaternion &a ) const {
	return CQuaternion( x + a.x, y + a.y, z + a.z, w + a.w );
}

inline CQuaternion& CQuaternion::operator+=( const CQuaternion &a ) {
	x += a.x;
	y += a.y;
	z += a.z;
	w += a.w;

	return (*this);
}

inline CQuaternion CQuaternion::operator-( const CQuaternion &a ) const {
	return CQuaternion( x - a.x, y - a.y, z - a.z, w - a.w );
}

inline CQuaternion& CQuaternion::operator-=( const CQuaternion &a ) {
	x -= a.x;
	y -= a.y;
	z -= a.z;
	w -= a.w;

	return (*this);
}

inline CQuaternion CQuaternion::operator*( const CQuaternion &a ) const {
	return CQuaternion(	w*a.x + x*a.w + y*a.z - z*a.y,
					w*a.y + y*a.w + z*a.x - x*a.z,
					w*a.z + z*a.w + x*a.y - y*a.x,
					w*a.w - x*a.x - y*a.y - z*a.z );
}

inline CQuaternion CQuaternion::operator*( FLOAT a ) const {
	return CQuaternion( x * a, y * a, z * a, w * a );
}
inline CQuaternion CQuaternion::operator/( FLOAT a ) const {
	assert(a != 0);
	return CQuaternion( x/a, y/a, z/a, w/a );
}
inline CQuaternion operator*( const FLOAT a, const CQuaternion &b ) {
	return b * a;
}

inline CQuaternion& CQuaternion::operator*=( const CQuaternion &a ) {
	(*this) = (*this) * a;
	return (*this);
}


inline CQuaternion& CQuaternion::operator*=( FLOAT a ) {
	x *= a;
	y *= a;
	z *= a;
	w *= a;

	return (*this);
}
inline FLOAT dotpdut(const CQuaternion &a,const CQuaternion &b)
{
	return (a.w*b.w + a.x*b.x + a.y*b.y + a.z*b.z);
}
inline CQuaternion	diff(const CQuaternion &a,const CQuaternion &b)
{
	//b*d = a
	CQuaternion d = (b.conjugated())*a;
	return d;
}
#endif // CQUATERNION_H
