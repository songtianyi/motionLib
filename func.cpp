#include "func.h"

void __matrixMultiXYZ(FLOAT *rs,const FLOAT *a,const FLOAT *b,const int X,const int Y,const int Z)
{
    FLOAT *tmp = new FLOAT[X*Z];
	for(int i = 0;i < X*Z;i++)
	{
		tmp[i] = 0.0f;
	}
    for(int i = 0;i < X;i++)
    {
        for(int j = 0;j < Z;j++)
        {
            for(int k = 0;k < Y;k++)
            {
                tmp[i*Z + j] += a[i*Y+k]*b[k*Z+j];
            }
        }
    }
    memcpy(rs,tmp,sizeof(FLOAT)*X*Z);
    delete [] tmp; tmp = 0;
}
void 	Pitch(CVector3f tarVec,CVector3f curVec,FLOAT &pitch)//x
{
	tarVec.x = 0;
	tarVec.normalize();
	curVec.x =0;
	curVec.normalize();
	if( (curVec.y == 0 && curVec.z == 0)
	||  (tarVec.y == 0 && tarVec.z == 0))//if curVec or tarVec is zero-vector
	{
		pitch = 0;return;
	}
	FLOAT dot = dotpdut(tarVec,curVec);
	if (dot < 0.9999 && dot > -0.9999)
	{
		CVector3f axis = cropdut(curVec,tarVec);
		if(axis.x > 0)
		{
			pitch =  acos(dot) * H3DMath::M_RAD2DEG;
		}
		else
		{
			pitch =  -acos(dot) * H3DMath::M_RAD2DEG;
		}
	}
	else
	{
		pitch = 0;
	}
}
void 	Yaw(CVector3f tarVec,CVector3f curVec,FLOAT &yaw)//y
{
	tarVec.y = 0;
	tarVec.normalize();
	curVec.y =0;
	curVec.normalize();
	if((curVec.x == 0 && curVec.z == 0)
	||  (tarVec.x == 0 && tarVec.z == 0))
	{
		yaw = 0;return;
	}
	FLOAT dot = dotpdut(tarVec,curVec);
	if (dot < 0.9999 && dot > -0.9999)
	{
		CVector3f axis = cropdut(tarVec,curVec);
		if(axis.y > 0)
		{
			yaw =  acos(dot) * H3DMath::M_RAD2DEG;
		}
		else
		{
			yaw =  -acos(dot) * H3DMath::M_RAD2DEG;
		}
	}
	else
	{
		yaw = 0;
	}
}
void 	Roll(CVector3f tarVec,CVector3f curVec,FLOAT &roll)//z
{
	tarVec.z = 0;
	tarVec.normalize();
	curVec.z =0;
	curVec.normalize();
	if((curVec.x == 0 && curVec.y == 0)
	||  (tarVec.x == 0 && tarVec.y == 0))
	{
		roll = 0;return;
	}
	FLOAT dot = dotpdut(tarVec,curVec);
	if (dot < 0.9999 && dot > -0.9999)
	{
		CVector3f axis = cropdut(tarVec,curVec);
		if(axis.z > 0)
		{
			roll =  acos(dot) * H3DMath::M_RAD2DEG;
		}
		else
		{
			roll =  -acos(dot) * H3DMath::M_RAD2DEG;
		}
	}
	else
	{
		roll = 0;
	}
}

CQuaternion CCD(CVector3f root,CVector3f curEnd,CVector3f desireEnd)
{
	//Local Variables
	FLOAT	cosAngle,turnAngle;
	CVector3f curVector,targetVector;
	CQuaternion quat(0,0,0,1);
	FLOAT IK_POS_THRESH = 0.0000029403;
	// SEE IF I AM ALREADY CLOSE ENOUGH
	if (euclideanDist(curEnd, desireEnd) > IK_POS_THRESH){
			// CREATE THE VECTOR TO THE CURRENT EFFECTOR POS
			curVector	= curEnd	- root;
			// CREATE THE DESIRED EFFECTOR POSITION VECTOR
			targetVector= desireEnd - root;
			// NORMALIZE THE VECTORS (EXPENSIVE, REQUIRES A SQRT)
			curVector.normalize();
			targetVector.normalize();
			// THE DOT PRODUCT GIVES ME THE COSINE OF THE DESIRED ANGLE
			cosAngle = dotpdut(targetVector,curVector);

			assert(! (fabs(cosAngle)-0.0000001 > 1));//fabscosAngle = [0,1]
			// IF THE DOT PRODUCT RETURNS 1.0, I DON'T NEED TO ROTATE AS IT IS 0 DEGREES
			if(cosAngle < 0.99999999999)
			{
				// USE THE CROSS PRODUCT TO CHECK WHICH WAY TO ROTATE
				CVector3f crossResult = cropdut(curVector,targetVector);
				crossResult.normalize();			//normalize the vector

				turnAngle = acos((FLOAT)cosAngle);	// GET THE ANGLE

				//calculate quaternion
				CQuaternion quat(\
					crossResult.x*sin(turnAngle/2),\
					crossResult.y*sin(turnAngle/2),\
					crossResult.z*sin(turnAngle/2),\
					cos(turnAngle/2));

				quat.normalize();
				return quat;
			}
	}
	return quat;
}

