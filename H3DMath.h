
/**
 * H3DMath.h
 *
 * Copyright (c) 2013, www.horizon3d.com.cn/Dalian Nationalities University. All Rights  Reserved.
 * Tianyi Song <songtianyi630@163.com>
 *
 * You can use this library in your project, but do not redistribute it and/or modify
 * it.
 *
 */

#ifndef H3DMATH_H
#define H3DMATH_H

#include <cmath>
#include <assert.h>
class H3DMath
{
public:
    static float                ASin( const float a );			// arc sine with 32 bits precision, input is clamped to [-1, 1] to avoid a silent NaN
    static double				ASin64( const float a );			// arc sine with 64 bits precision

    static float				ACos( const float a );			// arc cosine with 32 bits precision, input is clamped to [-1, 1] to avoid a silent NaN
	static double				ACos64( const float a );			// arc cosine with 64 bits precision
	static double               WRAPPI(const double a);
	static int                  complf(const double x);

	//get exponent
	static int _exponentOf(const double v);
	static int _exponentOf(const float v);

public:
	static const double			PI;							// pi
	static const double			TWO_PI;						// pi * 2
	static const double			HALF_PI;					// pi / 2
	static const double			ONEFOURTH_PI;				// pi / 4
	static const double         _1DivPI;                    //1 / pi
	static const double         _1DivTWO_PI;                //1 / TWO_PI
	static const double			E;							// e
	static const double			SQRT_TWO;					// sqrt( 2 )
	static const double			SQRT_THREE;					// sqrt( 3 )
	static const double			SQRT_1OVER2;				// sqrt( 1 / 2 )
	static const double			SQRT_1OVER3;				// sqrt( 1 / 3 )
	static const double			M_DEG2RAD;					// degrees to radians multiplier
	static const double			M_RAD2DEG;					// radians to degrees multiplier
	static const double			M_SEC2MS;					// seconds to milliseconds multiplier
	static const double			M_MS2SEC;					// milliseconds to seconds multiplier
	static const double			INFINITY;					// huge number which should be larger than any valid number used
	static const double         LF_COMP_EPS;                   //compare epsilon
};

/**
 *return 0 indicate x==0，-1 indicate x < 0, 1 incicate x>0
 *complf(a-b) == 0, a == b or fabs(a-b) < eps
 *complf(a-b) != 0, a != b or fabs(a-b) >= eps
 *complf(a-b) <  0, a <  b or a - b < -eps
 *complf(a-b) >  0, a >  b or a - b > eps
 *complf(a-b) <= 0, a <= b or a - b <= -eps
 *complf(a-b) >= 0, a >= b or a - b >= eps
 */
inline int H3DMath::complf(double x){ return x < -H3DMath::LF_COMP_EPS?-1:((x < H3DMath::LF_COMP_EPS)?0:1);}

inline int H3DMath::_exponentOf(const double v)
{
    assert(sizeof(short) == 2);
    short *t = ((short *)&v);
    t += 3;
    short t_v = *t;
    t_v = t_v & (32767);t_v >>= 4;t_v -= 1023;
    return t_v;
}
inline int H3DMath::_exponentOf(const float v)
{
	assert(sizeof(short) == 2);
	short *t = ((short *)&v);
	t += 1;
	short t_v = *t;
	t_v = t_v & (32767); t_v >>= 7; t_v -= 127;
	return t_v;
}

inline float H3DMath::ASin(const float a ) {
	if ( a <= -1.0f ) {
		return -(float)HALF_PI;
	}
	if ( a >= 1.0f ) {
		return (float)HALF_PI;
	}
	return asinf( a );
}

inline double H3DMath::ASin64(const float a ) {
	if ( a <= -1.0f ) {
		return -HALF_PI;
	}
	if ( a >= 1.0f ) {
		return HALF_PI;
	}
	return asin( a );
}

inline float H3DMath::ACos(const float a ) {
	if ( a <= -1.0f ) {
		return (float)PI;
	}
	if ( a >= 1.0f ) {
		return 0.0f;
	}
	return acosf( a );
}

inline double H3DMath::ACos64(const float a ) {
	if ( a <= -1.0f ) {
		return PI;
	}
	if ( a >= 1.0f ) {
		return 0.0f;
	}
	return acos( a );
}
inline double WRAPPI(double theta)
{
    theta += H3DMath::PI;
    theta -= floor(theta*H3DMath::_1DivTWO_PI)*H3DMath::TWO_PI;
    theta -= H3DMath::PI;
    assert(theta >= -H3DMath::PI && theta <= H3DMath::PI);
    return theta;
}
#endif // H3DMATH_H
