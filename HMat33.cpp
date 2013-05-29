#include "HMat33.h"
#include "CQuaternion.h"

CQuaternion		HMat33::toQuat( void ) const
{
	CQuaternion rs;

	//Input matrix:


	FLOAT m11 = mat[0][0],m12 = mat[0][1],m13 = mat[0][2];
	FLOAT m21 = mat[1][0],m22 = mat[1][1],m23 = mat[1][2];
	FLOAT m31 = mat[2][0],m32 = mat[2][1],m33 = mat[2][2];
	//Output quaternion

	FLOAT w,x,y,z;
	// Determine which of w, x, y, or z has the largest absolute value

	FLOAT fourWSquaredMinus1 = m11 +  m22 +  m33;
	FLOAT fourXSquaredMinus1 = m11 -  m22 - m33;
	FLOAT fourYSquaredMinus1 = m22 -  m11 - m33;
	FLOAT fourZSquaredMinus1 = m33 -  m11 - m22;

	int biggestIndex = 0;
	FLOAT fourBiggestSquaredMinus1 = fourWSquaredMinus1;

	if (fourXSquaredMinus1 > fourBiggestSquaredMinus1)
	{
		fourBiggestSquaredMinus1 = fourXSquaredMinus1;
		biggestIndex = 1;
	}
	if (fourYSquaredMinus1 > fourBiggestSquaredMinus1)
	{
		fourBiggestSquaredMinus1 = fourYSquaredMinus1;
		biggestIndex = 2;
	}
	if (fourZSquaredMinus1 > fourBiggestSquaredMinus1)
	{
		fourBiggestSquaredMinus1 = fourZSquaredMinus1;
		biggestIndex = 3;
	}
	// Perform square root and division
	FLOAT biggestVal = sqrt(fourBiggestSquaredMinus1 + 1.0f) * 0.5f;
	FLOAT mult = 0.25f / biggestVal;
	// Apply table to compute quaternion values

	switch (biggestIndex)
	{
	case 0:
		w = biggestVal;
		x = (m23 - m32) * mult;
		y = (m31 - m13) * mult;
		z = (m12 - m21) * mult;
		break;
	case 1:
		x = biggestVal;
		w = (m23 - m32) * mult;
		y = (m12 + m21) * mult;
		z = (m31 + m13) * mult;
		break;
	case 2:
		y = biggestVal;
		w = (m31 - m13) * mult;
		x = (m12 + m21) * mult;
		z = (m23 + m32) * mult;
		break;
	case 3:
		z = biggestVal;
		w = (m12 - m21) * mult;
		x = (m31 + m13) * mult;
		y = (m23 + m32) * mult;
		break;
	}
	rs.w = w; rs.x = x;rs.y = y;rs.z = z;
	return rs;
}

void HMat33::TransposeSelf()
{
    for(int i = 0;i < 2;i++)
        for(int j = i+1;j < 3;j++)
            std::swap(this->mat[i][j],this->mat[i][j]);
}
