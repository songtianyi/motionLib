
/**
 * CVector3f.h
 *
 * Copyright (c) 2013, Dalian Nationalities University. All Rights  Reserved.
 * Tianyi Song <songtianyi630@163.com>
 *
 * You can use this library in your project, but do not redistribute it and/or modify
 * it.
 *
 */


#ifndef CVECTOR3F_H
#define CVECTOR3F_H

#include <math.h>
#include <stdio.h>
#include "H3DMath.h"
#include "def.h"

#define pitch x //[-90,90]
#define heading y//[-180 180]
#define bank z//[-180 180]


#define OBJECTTOINERTIA 1
#define INERTIATOOBJECT 2


#ifdef MS_BUILD_ENV
#pragma warning(disable:4244)
#endif

class CQuaternion;

/*
 *@brief 3 dimension vector operations
 */
class CVector3f
{
public:
        CVector3f(void);
        CVector3f(const FLOAT x,const FLOAT y,const FLOAT z);
        ~CVector3f(void);

        FLOAT			operator[]( const int index ) const;
        FLOAT &			operator[]( const int index );
        CVector3f		operator-() const;
        CVector3f &		operator=( const CVector3f &a );		// required because of a msvc 6 & 7 bug
        //FLOAT			operator*( const CVector3f &a ) const; //avoid confusion between dot and multiply
        CVector3f		operator*( const FLOAT a ) const;
        CVector3f		operator/( const FLOAT a ) const;
        CVector3f		operator+( const CVector3f &a ) const;
        CVector3f		operator-( const CVector3f &a ) const;
        CVector3f &		operator+=( const CVector3f &a );
        CVector3f &		operator-=( const CVector3f &a );
        CVector3f &		operator/=( const CVector3f &a );
        CVector3f &		operator/=( const FLOAT a );
        CVector3f &		operator*=( const FLOAT a );

        friend CVector3f	operator*( const FLOAT a, const CVector3f b );
        friend FLOAT        dotpdut(const CVector3f &a,const CVector3f &b);
        FLOAT               dotpdut(const CVector3f &a);
        friend CVector3f    cropdut(const CVector3f &a,const CVector3f &b);
        CVector3f &		    cropdut( const CVector3f &a, const CVector3f &b );
        friend double       manhattnDist(const CVector3f &a, const CVector3f &b);//self defined manhattn distance
		friend double		euclideanDist(const CVector3f &a, const CVector3f &b);//euclidean distance


		//set
		void setXYZ(const FLOAT x,const FLOAT y,const FLOAT z);

        /*vector3f as euler angle*/
        ///////////////////////////////
        CQuaternion     toQuat(const int type);//transform to quaternion

        CVector3f       canonized(void)const;//not be changed
        void            canonize(void);//
        ///////////////////////////////

		CVector3f normalized(void)const;
		void normalize(void);//changed

		void print(void) const;


public:
        FLOAT x,y,z;//if this is euler angle,pitch = x heading=y bank=z,FLOAT in degree
};

//inline FLOAT CVector3f::operator*( const CVector3f &a ) const {
//	return x * a.x + y * a.y + z * a.z;
//}

inline FLOAT euclideanDist(const CVector3f &a, const CVector3f &b)
{
        return ((a.x -b.x)*(a.x -b.x) +  (a.y - b.y)*(a.y - b.y) + (a.z-b.z)*(a.z-b.z) );
        //return sqrt ((a.x -b.x)*(a.x -b.x) +  (a.y - b.y)*(a.y - b.y) + (a.z-b.z)*(a.z-b.z) );
}
inline FLOAT manhattanDist(const CVector3f &a, const CVector3f &b)
{
    return ( fabs(a.x -b.x) + fabs(a.y - b.y) + fabs(a.z-b.z) );
}
inline FLOAT dotpdut(const CVector3f &a,const CVector3f &b)
{
    return a.x*b.x + a.y*b.y + a.z*b.z;
}
inline FLOAT CVector3f::dotpdut(const CVector3f &a)
{
    return (a.x*x + a.y*y + a.z*z);
}
/*a X b*/
inline CVector3f cropdut(const CVector3f &a,const CVector3f &b)
{
	return CVector3f(a.y * b.z - a.z * b.y,a.z * b.x - a.x * b.z,a.x * b.y - a.y * b.x);
}
inline CVector3f &CVector3f::cropdut( const CVector3f &a, const CVector3f &b ) {
	x = a.y * b.z - a.z * b.y;
	y = a.z * b.x - a.x * b.z;
	z = a.x * b.y - a.y * b.x;
	return *this;
}

inline CVector3f operator*( const FLOAT a, const CVector3f b ) {
	return CVector3f( b.x * a, b.y * a, b.z * a );
}
inline CVector3f CVector3f::operator-( const CVector3f &a ) const {
	return CVector3f( (FLOAT)(x - a.x), (FLOAT)(y - a.y), (FLOAT)(z - a.z) );
}
inline FLOAT CVector3f::operator[]( const int index ) const {
	return ( &x )[ index ];
}
inline FLOAT &CVector3f::operator[]( const int index ) {
	return ( &x )[ index ];
}
inline CVector3f CVector3f::operator-() const {
	return CVector3f( -x, -y, -z );
}
inline CVector3f &CVector3f::operator=( const CVector3f &a ) {
	x = a.x;
	y = a.y;
	z = a.z;
	return *this;
}

inline CVector3f CVector3f::operator*( const FLOAT a ) const {
	return CVector3f( x * a, y * a, z * a );
}
inline CVector3f CVector3f::operator/( const FLOAT a ) const {
	FLOAT inva = 1.0 / a;
	return CVector3f( x * inva, y * inva, z * inva );
}

inline CVector3f CVector3f::operator+( const CVector3f &a ) const {
	return CVector3f( x + a.x, y + a.y, z + a.z );
}
inline CVector3f &CVector3f::operator+=( const CVector3f &a ) {
	x += a.x;
	y += a.y;
	z += a.z;

	return *this;
}
inline CVector3f &CVector3f::operator/=( const CVector3f &a ) {
	x /= a.x;
	y /= a.y;
	z /= a.z;

	return *this;
}
inline CVector3f &CVector3f::operator/=( const FLOAT a ) {
	FLOAT inva = 1.0 / a;
	x *= inva;
	y *= inva;
	z *= inva;

	return *this;
}
inline CVector3f &CVector3f::operator-=( const CVector3f &a ) {
	x -= a.x;
	y -= a.y;
	z -= a.z;

	return *this;
}

inline CVector3f &CVector3f::operator*=( const FLOAT a ) {
	x *= a;
	y *= a;
	z *= a;

	return *this;
}

#endif // CVECTOR3F_H
