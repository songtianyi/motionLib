#ifndef HMat44_H
#define HMat44_H

#include <string.h>
#include <assert.h>
#include <math.h>
#ifndef ID_INLINE
#define ID_INLINE inline
#endif
#define MATRIX_INVERSE_EPSILON		1e-14

class HMat44
{
public:
			HMat44( void );
			explicit HMat44(const float xx, const float xy, const float xz, const float xw,
							const float yx, const float yy, const float yz, const float yw,
							const float zx, const float zy, const float zz, const float zw,
							const float wx, const float wy, const float wz, const float ww );
			explicit HMat44( const float src[ 4 ][ 4 ] );

			HMat44			operator*( const float a ) const;
			HMat44			operator*( const HMat44 &a ) const;
			HMat44			operator+( const HMat44 &a ) const;
			HMat44			operator-( const HMat44 &a ) const;
			HMat44 &		operator*=( const float a );
			HMat44 &		operator*=( const HMat44 &a );
			HMat44 &		operator+=( const HMat44 &a );
			HMat44 &		operator-=( const HMat44 &a );

			friend HMat44	operator*( const float a, const HMat44 &mat );

			HMat44			InverseFast( void ) const;	// returns the inverse ( m * m.Inverse() = identity )
			bool			InverseFastSelf( void );	// returns false if determinant is zero
public:
	float mat[4][4];
};
ID_INLINE HMat44::HMat44( void ) {
}
ID_INLINE HMat44::HMat44( const float xx, const float xy, const float xz, const float xw,
							const float yx, const float yy, const float yz, const float yw,
							const float zx, const float zy, const float zz, const float zw,
							const float wx, const float wy, const float wz, const float ww ) {
	mat[0][0] = xx; mat[0][1] = xy; mat[0][2] = xz; mat[0][3] = xw;
	mat[1][0] = yx; mat[1][1] = yy; mat[1][2] = yz; mat[1][3] = yw;
	mat[2][0] = zx; mat[2][1] = zy; mat[2][2] = zz; mat[2][3] = zw;
	mat[3][0] = wx; mat[3][1] = wy; mat[3][2] = wz; mat[3][3] = ww;
}

ID_INLINE HMat44::HMat44( const float src[ 4 ][ 4 ] ) {
	memcpy( mat, src, 4 * 4 * sizeof( float ) );
}
ID_INLINE HMat44 HMat44::operator*( const float a ) const {
	return HMat44(
		mat[0][0] * a, mat[0][1] * a, mat[0][2] * a, mat[0][3] * a,
		mat[1][0] * a, mat[1][1] * a, mat[1][2] * a, mat[1][3] * a,
		mat[2][0] * a, mat[2][1] * a, mat[2][2] * a, mat[2][3] * a,
		mat[3][0] * a, mat[3][1] * a, mat[3][2] * a, mat[3][3] * a );
}
ID_INLINE HMat44 HMat44::operator*( const HMat44 &a ) const {
	int i, j;
	const float *m1Ptr, *m2Ptr;
	float *dstPtr;
	HMat44 dst;

	m1Ptr = reinterpret_cast<const float *>(this);
	m2Ptr = reinterpret_cast<const float *>(&a);
	dstPtr = reinterpret_cast<float *>(&dst);

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
ID_INLINE HMat44 &HMat44::operator*=( const float a ) {
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
ID_INLINE HMat44 operator*( const float a, const HMat44 &mat ) {
	return mat * a;
}
ID_INLINE HMat44 HMat44::InverseFast( void ) const {
	HMat44 invMat;

	invMat = *this;
	int r = invMat.InverseFastSelf();
	assert( r );
	return invMat;
}

#endif