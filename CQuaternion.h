#ifndef CQUATERNION_H
#define CQUATERNION_H

#include <math.h>
#include "H3DMath.h"

#ifndef OBJECTTOINERTIA
#define OBJECTTOINERTIA 1
#endif
#ifndef INERTIATOOBJECT
#define INERTIATOOBJECT 2
#endif

class CVector3f;
class HMat33;
class HMat44;
class CAAP;

/*
 *@brief quaternion operations
 *		result quaternions are not normalized!
 */
class CQuaternion
{
public:
        CQuaternion(void);
        CQuaternion(const float x,const float y,const float z,const float w);
        ~CQuaternion(void);

        float				operator[]( const int index ) const;
        float &				operator[]( const int index );
		CQuaternion			operator-() const;
		CQuaternion &		operator=( const CQuaternion &a );
		CQuaternion			operator+( const CQuaternion &a ) const;
		CQuaternion &		operator+=( const CQuaternion &a );
		CQuaternion			operator-( const CQuaternion &a ) const;//each element minus the corresponding element in a
		CQuaternion &		operator-=( const CQuaternion &a );
		CQuaternion			operator*( const CQuaternion &a ) const;//cross product of two quaternions
		CQuaternion			operator*( const float a ) const;//multiply each element  with a
		CQuaternion			operator/(const float a)const;//divide each element with a
		CQuaternion &		operator*=( const CQuaternion &a );
		CQuaternion &		operator*=( const float a );
		

		friend CQuaternion	operator*( const float a, const CQuaternion &b );
		friend CQuaternion	diff(const CQuaternion &a,const CQuaternion &b);//if b*diff = a, calculate diff
		friend float		dotpdut(const CQuaternion &a,const CQuaternion &b);//dot product

		

        CVector3f toEulerAngle(const int type);
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
        float w,x,y,z;

};

inline float CQuaternion::operator[]( const int index ) const {
	assert( ( index > -1 ) && ( index < 4 ) );
	return ( &x )[ index ];
}
inline float &CQuaternion::operator[]( const int index ) {
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

inline CQuaternion CQuaternion::operator*( float a ) const {
	return CQuaternion( x * a, y * a, z * a, w * a );
}
inline CQuaternion CQuaternion::operator/( float a ) const {
	assert(a != 0);
	return CQuaternion( x/a, y/a, z/a, w/a );
}
inline CQuaternion operator*( const float a, const CQuaternion &b ) {
	return b * a;
}

inline CQuaternion& CQuaternion::operator*=( const CQuaternion &a ) {
	(*this) = (*this) * a;
	return (*this);
}


inline CQuaternion& CQuaternion::operator*=( float a ) {
	x *= a;
	y *= a;
	z *= a;
	w *= a;

	return (*this);
}
inline float dotpdut(const CQuaternion &a,const CQuaternion &b)
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
