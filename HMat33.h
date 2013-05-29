
/**
 * HMat33.h
 *
 * Copyright (c) 2013, www.horizon3d.com.cn/‎Dalian Nationalities University. All Rights  Reserved.
 * Tianyi Song <songtianyi630@163.com>
 *
 * You can use this library in your project, but do not redistribute it and/or modify
 * it.
 *
 */

#ifndef HMat33_H
#define HMat33_H
#include "CVector3f.h"


#include <string.h>
#include <iostream>

class CQuaternion;
class HMat44;


#define ID_INLINE inline


class HMat33
{
public:
	HMat33( void );
	explicit HMat33( const CVector3f &x, const CVector3f &y, const CVector3f &z );
	explicit HMat33( const FLOAT xx, const FLOAT xy, const FLOAT xz, const FLOAT yx, const FLOAT yy, const FLOAT yz, const FLOAT zx, const FLOAT zy, const FLOAT zz );
	explicit HMat33( const FLOAT src[ 3 ][ 3 ] );

	const CVector3f &	operator[]( int index ) const;
	CVector3f &		operator[]( int index );
	HMat33			operator-() const;
	HMat33			operator*( const FLOAT a ) const;
	HMat33			operator*( const HMat33 &a ) const;
	HMat33			operator+( const HMat33 &a ) const;
	HMat33			operator-( const HMat33 &a ) const;
	HMat33 &		operator*=( const FLOAT a );
	HMat33 &		operator*=( const HMat33 &a );
	HMat33 &		operator+=( const HMat33 &a );
	HMat33 &		operator-=( const HMat33 &a );

	friend HMat33	operator*( const FLOAT a, const HMat33 &mat );

    HMat33          Transpose( void );
    void            TransposeSelf( void );

	CVector3f		toAngles( void ) const;//transform to euler angles
	CQuaternion		toQuat( void ) const;//transform to quaternion


private:
	CVector3f			mat[ 3 ];
};

ID_INLINE HMat33::HMat33( void ) {
}

ID_INLINE HMat33::HMat33( const CVector3f &x, const CVector3f &y, const CVector3f &z ) {
	mat[ 0 ].x = x.x; mat[ 0 ].y = x.y; mat[ 0 ].z = x.z;
	mat[ 1 ].x = y.x; mat[ 1 ].y = y.y; mat[ 1 ].z = y.z;
	mat[ 2 ].x = z.x; mat[ 2 ].y = z.y; mat[ 2 ].z = z.z;
}

ID_INLINE HMat33::HMat33( const FLOAT xx, const FLOAT xy, const FLOAT xz, const FLOAT yx, const FLOAT yy, const FLOAT yz, const FLOAT zx, const FLOAT zy, const FLOAT zz ) {
	mat[ 0 ].x = xx; mat[ 0 ].y = xy; mat[ 0 ].z = xz;
	mat[ 1 ].x = yx; mat[ 1 ].y = yy; mat[ 1 ].z = yz;
	mat[ 2 ].x = zx; mat[ 2 ].y = zy; mat[ 2 ].z = zz;
}

ID_INLINE HMat33::HMat33( const FLOAT src[ 3 ][ 3 ] ) {
	memcpy( mat, src, 3 * 3 * sizeof( FLOAT ) );
}

ID_INLINE const CVector3f &HMat33::operator[]( int index ) const {
	assert( ( index > -1 ) && ( index < 3 ) );
	return mat[ index ];
}

ID_INLINE CVector3f &HMat33::operator[]( int index ) {
	assert( ( index > -1 ) && ( index < 3 ) );
	return mat[ index ];
}
ID_INLINE HMat33 HMat33::operator-() const {
	return HMat33(	-mat[0][0], -mat[0][1], -mat[0][2],
					-mat[1][0], -mat[1][1], -mat[1][2],
					-mat[2][0], -mat[2][1], -mat[2][2] );
}
ID_INLINE HMat33 HMat33::operator*( const FLOAT a ) const {
	return HMat33(
		mat[0].x * a, mat[0].y * a, mat[0].z * a,
		mat[1].x * a, mat[1].y * a, mat[1].z * a,
		mat[2].x * a, mat[2].y * a, mat[2].z * a );
}
ID_INLINE HMat33 HMat33::operator*( const HMat33 &a ) const {
	int i, j;
	const FLOAT *m1Ptr, *m2Ptr;
	FLOAT *dstPtr;
	HMat33 dst;

	m1Ptr = reinterpret_cast<const FLOAT *>(this);
	m2Ptr = reinterpret_cast<const FLOAT *>(&a);
	dstPtr = reinterpret_cast<FLOAT *>(&dst);

	for ( i = 0; i < 3; i++ ) {
		for ( j = 0; j < 3; j++ ) {
			*dstPtr = m1Ptr[0] * m2Ptr[ 0 * 3 + j ]
					+ m1Ptr[1] * m2Ptr[ 1 * 3 + j ]
					+ m1Ptr[2] * m2Ptr[ 2 * 3 + j ];
			dstPtr++;
		}
		m1Ptr += 3;
	}
	return dst;
}
ID_INLINE HMat33 HMat33::operator+( const HMat33 &a ) const {
	return HMat33(
		mat[0].x + a[0].x, mat[0].y + a[0].y, mat[0].z + a[0].z,
		mat[1].x + a[1].x, mat[1].y + a[1].y, mat[1].z + a[1].z,
		mat[2].x + a[2].x, mat[2].y + a[2].y, mat[2].z + a[2].z );
}
ID_INLINE HMat33 HMat33::operator-( const HMat33 &a ) const {
	return HMat33(
		mat[0].x - a[0].x, mat[0].y - a[0].y, mat[0].z - a[0].z,
		mat[1].x - a[1].x, mat[1].y - a[1].y, mat[1].z - a[1].z,
		mat[2].x - a[2].x, mat[2].y - a[2].y, mat[2].z - a[2].z );
}
ID_INLINE HMat33 &HMat33::operator*=( const FLOAT a ) {
	mat[0].x *= a; mat[0].y *= a; mat[0].z *= a;
	mat[1].x *= a; mat[1].y *= a; mat[1].z *= a;
	mat[2].x *= a; mat[2].y *= a; mat[2].z *= a;

    return *this;
}
ID_INLINE HMat33 &HMat33::operator*=( const HMat33 &a ) {
	int i, j;
	const FLOAT *m2Ptr;
	FLOAT *m1Ptr, dst[3];

	m1Ptr = reinterpret_cast<FLOAT *>(this);
	m2Ptr = reinterpret_cast<const FLOAT *>(&a);

	for ( i = 0; i < 3; i++ ) {
		for ( j = 0; j < 3; j++ ) {
			dst[j]  = m1Ptr[0] * m2Ptr[ 0 * 3 + j ]
					+ m1Ptr[1] * m2Ptr[ 1 * 3 + j ]
					+ m1Ptr[2] * m2Ptr[ 2 * 3 + j ];
		}
		m1Ptr[0] = dst[0]; m1Ptr[1] = dst[1]; m1Ptr[2] = dst[2];
		m1Ptr += 3;
	}
	return *this;
}
ID_INLINE HMat33 &HMat33::operator+=( const HMat33 &a ) {
	mat[0].x += a[0].x; mat[0].y += a[0].y; mat[0].z += a[0].z;
	mat[1].x += a[1].x; mat[1].y += a[1].y; mat[1].z += a[1].z;
	mat[2].x += a[2].x; mat[2].y += a[2].y; mat[2].z += a[2].z;

    return *this;
}
ID_INLINE HMat33 &HMat33::operator-=( const HMat33 &a ) {
	mat[0].x -= a[0].x; mat[0].y -= a[0].y; mat[0].z -= a[0].z;
	mat[1].x -= a[1].x; mat[1].y -= a[1].y; mat[1].z -= a[1].z;
	mat[2].x -= a[2].x; mat[2].y -= a[2].y; mat[2].z -= a[2].z;

    return *this;
}
ID_INLINE HMat33 operator*( const FLOAT a, const HMat33 &mat ) {
	return mat * a;
}
ID_INLINE HMat33 HMat33::Transpose()
{
    HMat33 t = *this;
    t.TransposeSelf();
    return t;
}
#endif
