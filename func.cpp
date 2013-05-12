#include "func.h"

void __matrixMultiXYZ(float *rs,const float *a,const float *b,const int X,const int Y,const int Z)
{
    float *tmp = new float[X*Z];
	for(int i = 0;i < X*Z;i++)
	{
		tmp[i] = 0;
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
    memcpy(rs,tmp,sizeof(float)*X*Z);
    delete [] tmp; tmp = 0;
}
void 	Pitch(CVector3f tarVec,CVector3f curVec,float &pitch)//x
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
	float dot = dotpdut(tarVec,curVec);
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
void 	Yaw(CVector3f tarVec,CVector3f curVec,float &yaw)//y
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
	float dot = dotpdut(tarVec,curVec);
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
void 	Roll(CVector3f tarVec,CVector3f curVec,float &roll)//z
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
	float dot = dotpdut(tarVec,curVec);
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