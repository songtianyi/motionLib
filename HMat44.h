
/**
 * HMat44.h
 *
 * Copyright (c) 2013, www.horizon3d.com.cn/Dalian Nationalities University. All Rights  Reserved.
 * Tianyi Song <songtianyi630@163.com>
 *
 * You can use this library in your project, but do not redistribute it and/or modify
 * it.
 *
 */
#ifndef HMat44_H
#define HMat44_H

#include <string.h>
#include <assert.h>
#include <math.h>
#include <iostream>
#include "def.h"


#define ID_INLINE inline
#define MATRIX_INVERSE_EPSILON		1e-14

class HMat44
{
public:
			HMat44( void );
			explicit HMat44(const FLOAT xx, const FLOAT xy, const FLOAT xz, const FLOAT xw,
							const FLOAT yx, const FLOAT yy, const FLOAT yz, const FLOAT yw,
							const FLOAT zx, const FLOAT zy, const FLOAT zz, const FLOAT zw,
							const FLOAT wx, const FLOAT wy, const FLOAT wz, const FLOAT ww );
			explicit HMat44( const FLOAT src[ 4 ][ 4 ] );

			HMat44			operator*( const FLOAT a ) const;
			HMat44			operator*( const HMat44 &a ) const;
			HMat44			operator+( const HMat44 &a ) const;
			HMat44			operator-( const HMat44 &a ) const;
			HMat44 &		operator*=( const FLOAT a );
			HMat44 &		operator*=( const HMat44 &a );
			HMat44 &		operator+=( const HMat44 &a );
			HMat44 &		operator-=( const HMat44 &a );

			friend HMat44	operator*( const FLOAT a, const HMat44 &mat );

			HMat44			InverseFast( void ) const;	// returns the inverse ( m * m.Inverse() = identity )
			bool			InverseFastSelf( void );	// returns false if determinant is zero

			HMat44          Transpose( void );
			void            TransposeSelf( void );
public:
	FLOAT mat[4][4];
};
ID_INLINE HMat44::HMat44( void ) {
}
ID_INLINE HMat44::HMat44( const FLOAT xx, const FLOAT xy, const FLOAT xz, const FLOAT xw,
							const FLOAT yx, const FLOAT yy, const FLOAT yz, const FLOAT yw,
							const FLOAT zx, const FLOAT zy, const FLOAT zz, const FLOAT zw,
							const FLOAT wx, const FLOAT wy, const FLOAT wz, const FLOAT ww ) {
	mat[0][0] = xx; mat[0][1] = xy; mat[0][2] = xz; mat[0][3] = xw;
	mat[1][0] = yx; mat[1][1] = yy; mat[1][2] = yz; mat[1][3] = yw;
	mat[2][0] = zx; mat[2][1] = zy; mat[2][2] = zz; mat[2][3] = zw;
	mat[3][0] = wx; mat[3][1] = wy; mat[3][2] = wz; mat[3][3] = ww;
}

ID_INLINE HMat44::HMat44( const FLOAT src[ 4 ][ 4 ] ) {
	memcpy( mat, src, 4 * 4 * sizeof( FLOAT ) );
}
ID_INLINE HMat44 HMat44::operator*( const FLOAT a ) const {
	return HMat44(
		mat[0][0] * a, mat[0][1] * a, mat[0][2] * a, mat[0][3] * a,
		mat[1][0] * a, mat[1][1] * a, mat[1][2] * a, mat[1][3] * a,
		mat[2][0] * a, mat[2][1] * a, mat[2][2] * a, mat[2][3] * a,
		mat[3][0] * a, mat[3][1] * a, mat[3][2] * a, mat[3][3] * a );
}
ID_INLINE HMat44 HMat44::operator*( const HMat44 &a ) const {
	int i, j;
	const FLOAT *m1Ptr, *m2Ptr;
	FLOAT *dstPtr;
	HMat44 dst;

	m1Ptr = reinterpret_cast<const FLOAT *>(this);
	m2Ptr = reinterpret_cast<const FLOAT *>(&a);
	dstPtr = reinterpret_cast<FLOAT *>(&dst);

	for ( i = 0; i < 4; i++ ) {
		for ( j = 0; j < 4; j++ ) {
			*dstPtr = m1Ptr[0] * m2Ptr[ 0 * 4 + j ]
					+ m1Ptr[1] * m2Ptr[ 1 * 4 + j ]
					+ m1Ptr[2] * m2Ptr[ 2 * 4 + j ]
					+ m1Ptr[3] * m2Ptr[ 3 * 4 + j ];
			dstPtr++;
		}
		m1Ptr += 4;
	}
	return dst;
}
ID_INLINE HMat44 HMat44::operator+( const HMat44 &a ) const {
	return HMat44(
		mat[0][0] + a.mat[0][0], mat[0][1] + a.mat[0][1], mat[0][2] + a.mat[0][2], mat[0][3] + a.mat[0][3],
		mat[1][0] + a.mat[1][0], mat[1][1] + a.mat[1][1], mat[1][2] + a.mat[1][2], mat[1][3] + a.mat[1][3],
		mat[2][0] + a.mat[2][0], mat[2][1] + a.mat[2][1], mat[2][2] + a.mat[2][2], mat[2][3] + a.mat[2][3],
		mat[3][0]+ a.mat[3][0], mat[3][1] + a.mat[3][1], mat[3][2] + a.mat[3][2], mat[3][3] + a.mat[3][3] );
}

ID_INLINE HMat44 HMat44::operator-( const HMat44 &a ) const {
	return HMat44(
		mat[0][0] - a.mat[0][0], mat[0][1] - a.mat[0][1], mat[0][2] - a.mat[0][2], mat[0][3] - a.mat[0][3],
		mat[1][0] - a.mat[1][0], mat[1][1] - a.mat[1][1], mat[1][2] - a.mat[1][2], mat[1][3] - a.mat[1][3],
		mat[2][0] - a.mat[2][0], mat[2][1] - a.mat[2][1], mat[2][2] - a.mat[2][2], mat[2][3] - a.mat[2][3],
		mat[3][0] - a.mat[3][0], mat[3][1] - a.mat[3][1], mat[3][2] - a.mat[3][2], mat[3][3] - a.mat[3][3] );
}
ID_INLINE HMat44 &HMat44::operator*=( const FLOAT a ) {
	mat[0][0] *= a; mat[0][1] *= a; mat[0][2] *= a; mat[0][3] *= a;
	mat[1][0] *= a; mat[1][1] *= a; mat[1][2] *= a; mat[1][3] *= a;
	mat[2][0] *= a; mat[2][1] *= a; mat[2][2] *= a; mat[2][3] *= a;
	mat[3][0] *= a; mat[3][1] *= a; mat[3][2] *= a; mat[3][3] *= a;
    return *this;
}

ID_INLINE HMat44 &HMat44::operator*=( const HMat44 &a ) {
	*this = (*this) * a;
	return *this;
}
ID_INLINE HMat44 &HMat44::operator+=( const HMat44 &a ) {
	mat[0][0] += a.mat[0][0]; mat[0][1] += a.mat[0][1]; mat[0][2] += a.mat[0][2]; mat[0][3] += a.mat[0][3];
	mat[1][0] += a.mat[1][0]; mat[1][1] += a.mat[1][1]; mat[1][2] += a.mat[1][2]; mat[1][3] += a.mat[1][3];
	mat[2][0] += a.mat[2][0]; mat[2][1] += a.mat[2][1]; mat[2][2] += a.mat[2][2]; mat[2][3] += a.mat[2][3];
	mat[3][0] += a.mat[3][0]; mat[3][1] += a.mat[3][1]; mat[3][2] += a.mat[3][2]; mat[3][3] += a.mat[3][3];
    return *this;
}

ID_INLINE HMat44 &HMat44::operator-=( const HMat44 &a ) {
	mat[0][0] -= a.mat[0][0]; mat[0][1] -= a.mat[0][1]; mat[0][2] -= a.mat[0][2]; mat[0][3] -= a.mat[0][3];
	mat[1][0] -= a.mat[1][0]; mat[1][1] -= a.mat[1][1]; mat[1][2] -= a.mat[1][2]; mat[1][3] -= a.mat[1][3];
	mat[2][0] -= a.mat[2][0]; mat[2][1] -= a.mat[2][1]; mat[2][2] -= a.mat[2][2]; mat[2][3] -= a.mat[2][3];
	mat[3][0] -= a.mat[3][0]; mat[3][1] -= a.mat[3][1]; mat[3][2] -= a.mat[3][2]; mat[3][3] -= a.mat[3][3];
    return *this;
}
ID_INLINE HMat44 operator*( const FLOAT a, const HMat44 &mat ) {
	return mat * a;
}
ID_INLINE HMat44 HMat44::InverseFast( void ) const {
	HMat44 invMat;

	invMat = *this;
	int r = invMat.InverseFastSelf();
	assert( r );
	return invMat;
}
ID_INLINE HMat44 HMat44::Transpose()
{
    HMat44 t = *this;
    t.TransposeSelf();
    return t;
}
#endif
