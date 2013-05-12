#ifndef CVECTOR3F_H
#define CVECTOR3F_H

#include <math.h>
#include <stdio.h>
#include "H3DMath.h"

#define pitch x //[-90,90]
#define heading y//[-180 180]
#define bank z//[-180 180]

#ifndef OBJECTTOINERTIA
#define OBJECTTOINERTIA 1
#endif

#ifndef INERTIATOOBJECT
#define INERTIATOOBJECT 2
#endif

#pragma warning(disable:4244)

class CQuaternion;

/*
 *@brief 3 dimension vector operations
 */
class CVector3f
{
public:
        CVector3f(void);
        CVector3f(const float x,const float y,const float z);
        ~CVector3f(void);

        float			operator[]( const int index ) const;
        float &			operator[]( const int index );
        CVector3f		operator-() const;
        CVector3f &		operator=( const CVector3f &a );		// required because of a msvc 6 & 7 bug
        //float			operator*( const CVector3f &a ) const; //avoid confusion between dot and multiply
        CVector3f		operator*( const float a ) const;
        CVector3f		operator/( const float a ) const;
        CVector3f		operator+( const CVector3f &a ) const;
        CVector3f		operator-( const CVector3f &a ) const;
        CVector3f &		operator+=( const CVector3f &a );
        CVector3f &		operator-=( const CVector3f &a );
        CVector3f &		operator/=( const CVector3f &a );
        CVector3f &		operator/=( const float a );
        CVector3f &		operator*=( const float a );

        friend CVector3f	operator*( const float a, const CVector3f b );
        friend float        dotpdut(const CVector3f &a,const CVector3f &b);
        float               dotpdut(const CVector3f &a);
        friend CVector3f    cropdut(const CVector3f &a,const CVector3f &b);
        CVector3f &		    cropdut( const CVector3f &a, const CVector3f &b );
        friend double       manhattnDist(const CVector3f &a, const CVector3f &b);//self defined manhattn distance
		friend double		euclideanDist(const CVector3f &a, const CVector3f &b);//euclidean distance

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
        float x,y,z;//pitch heading bank,degree
};

//inline float CVector3f::operator*( const CVector3f &a ) const {
//	return x * a.x + y * a.y + z * a.z;
//}

inline double euclideanDist(const CVector3f &a, const CVector3f &b)
{
	return sqrt ((a.x -b.x)*(a.x -b.x) +  (a.y - b.y)*(a.y - b.y) + (a.z-b.z)*(a.z-b.z) );
}
inline double manhattanDist(const CVector3f &a, const CVector3f &b)
{
    return ( fabs(a.x -b.x) + fabs(a.y - b.y) + fabs(a.z-b.z) );
}
inline float dotpdut(const CVector3f &a,const CVector3f &b)
{
    return a.x*b.x + a.y*b.y + a.z*b.z;
}
inline float CVector3f::dotpdut(const CVector3f &a)
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

inline CVector3f operator*( const float a, const CVector3f b ) {
	return CVector3f( b.x * a, b.y * a, b.z * a );
}
inline CVector3f CVector3f::operator-( const CVector3f &a ) const {
	return CVector3f( x - a.x, y - a.y, z - a.z );
}
inline float CVector3f::operator[]( const int index ) const {
	return ( &x )[ index ];
}
inline float &CVector3f::operator[]( const int index ) {
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

inline CVector3f CVector3f::operator*( const float a ) const {
	return CVector3f( x * a, y * a, z * a );
}
inline CVector3f CVector3f::operator/( const float a ) const {
	float inva = 1.0f / a;
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
inline CVector3f &CVector3f::operator/=( const float a ) {
	float inva = 1.0f / a;
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

inline CVector3f &CVector3f::operator*=( const float a ) {
	x *= a;
	y *= a;
	z *= a;

	return *this;
}

#endif // CVECTOR3F_H
