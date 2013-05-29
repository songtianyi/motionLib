#include "CQuatInterp.h"

CQuatInterp::CQuatInterp(void)
{
    //ctor
}

CQuatInterp::~CQuatInterp(void)
{
    //dtor
}
void CQuatInterp::slerp(CQuaternion *rot,const int n)
{
    CQuaternion s = rot[0],e = rot[n-1];
	FLOAT cosa = s.w*e.w + s.x*e.x + s.y*e.y + s.z*e.z;
	if ( cosa < 0.0f ) {
		e.w = -e.w;
		e.x = -e.x;
		e.y = -e.y;
		e.z = -e.z;
		cosa = -cosa;
	}
	for(int i = 1;i < n-1;i++)
	{
	    FLOAT t = 1.0*i / (n-1);
        FLOAT k0, k1;
        if ( cosa > 0.999999 )
        {
            k0 = 1.0 - t;
            k1 = t;
        }
        else
        {
            FLOAT sina = sqrt( 1.0 - cosa*cosa );
            FLOAT a = atan2( sina, cosa );
            FLOAT invSina = 1.0 / sina;
            k0 = sin((1.0 - t)*a) * invSina;
            k1 = sin(t*a) * invSina;
        }
        rot[i].w = ( s.w*k0 + e.w*k1 );
        rot[i].x = ( s.x*k0 + e.x*k1 );
        rot[i].y = ( s.y*k0 + e.y*k1 );
        rot[i].z = ( s.z*k0 + e.z*k1 );
        rot[i].normalize();
	}
}

CQuaternion CQuatInterp::slerp(const CQuaternion &s,const CQuaternion &ee,const FLOAT t)
{
    CQuaternion rs,e = ee;
    FLOAT cosa = s.w*e.w + s.x*e.x + s.y*e.y + s.z*e.z;

	if ( cosa < 0.0 ) {
		e.w = -e.w;
		e.x = -e.x;
		e.y = -e.y;
		e.z = -e.z;
		cosa = -cosa;
	}

    FLOAT k0, k1;
    if ( cosa > 0.999999 ) {
        k0 = 1.0 - t;
        k1 = t;
    }
    else {
        FLOAT sina = sqrt( 1.0 - cosa*cosa );
        FLOAT a = atan2( sina, cosa );
        FLOAT invSina = 1.0 / sina;
        k0 = sin((1.0 - t)*a) * invSina;
        k1 = sin(t*a) * invSina;
    }
    rs.w = ( s.w*k0 + e.w*k1 );
    rs.x = ( s.x*k0 + e.x*k1 );
    rs.y = ( s.y*k0 + e.y*k1 );
    rs.z = ( s.z*k0 + e.z*k1 );
    return rs;
}
void CQuatInterp::slerp(CQuaternion &rs,const CQuaternion &s,const CQuaternion &ee,const FLOAT t)
{
	rs = slerp(s,ee,t);
}


void CQuatInterp::squad(CQuaternion &rs,const CQuaternion &q0,const CQuaternion &q1,const CQuaternion &q2,const CQuaternion &q3,const FLOAT t)
{
/*
	//direct X
	D3DXQUATERNION s1,s2,C,dxrs;
	D3DXQUATERNION dxq0,dxq1,dxq2,dxq3;
	dxq0.w = q0.w; dxq1.w = q1.w; dxq2.w = q2.w; dxq3.w = q3.w;
	dxq0.x = q0.x; dxq1.x = q1.x; dxq2.x = q2.x; dxq3.x = q3.x;
	dxq0.y = q0.y; dxq1.y = q1.y; dxq2.y = q2.y; dxq3.y = q3.y;
	dxq0.z = q0.z; dxq1.z = q1.z; dxq2.z = q2.z; dxq3.z = q3.z;

	D3DXQuaternionSquadSetup(&s1,&s2,&C,&dxq0,&dxq1,&dxq2,&dxq3);
	D3DXQuaternionSquad(&dxrs,&dxq1,&s1,&s2,&C,t);
	rs.w = dxrs.w;
	rs.x = dxrs.x;
	rs.y = dxrs.y;
	rs.z = dxrs.z;

*/
	//
	CQuaternion ss1,ss2,rss;

	//formulation
	//s1 = exp(( -(log(q2*Inv(q1)) + log( q0*Inv(q1)) ) )/4)q1
	//s2 = exp(( -(log(q3*Inv(q2)) + log( q1*Inv(q2)) ) )/4)q2

	ss1 = ( ( -( (q2*q1.conjugated()).log() + (q0*q1.conjugated()).log() ) )/4 ).exp()*q1;
	ss2 = ( ( -( (q3*q2.conjugated()).log() + (q1*q2.conjugated()).log() ) )/4 ).exp()*q2;

	rss = slerp(slerp(q1,q2,t),slerp(ss1,ss2,t),2*t*(1-t));

	rs = rss;

}

void CQuatInterp::squad(CQuaternion *rot,const int n,const CQuaternion &q0,const CQuaternion &q3)//rot[0] = q1,rot[n-1] = q2
{
	for(int i = 1;i < n-1;i++)
	{
		FLOAT t = 1.0f/n-1;
		squad(rot[i],q0,rot[0],rot[n-1],q3,t);
	}
}
CQuaternion CQuatInterp::_Bisect(const CQuaternion &a,const CQuaternion &b)
{
	CQuaternion t = a+b;
	return t*(1/sqrt(t.w*t.w + t.x*t.x + t.y*t.y + t.z*t.z));
}
CQuaternion CQuatInterp::_Double(const CQuaternion &p,const CQuaternion &q)
{
	return (2*(dotpdut(p,q))*q) - p;
}
void CQuatInterp::sbezier(CQuaternion &rs,const CQuaternion &q0,const CQuaternion &q1,const CQuaternion &q2,const CQuaternion &q3,const FLOAT t)
{
	//qn-1 = q0,qn = q1,qn+1 = q2 qn+2 = q3
	CQuaternion an = _Bisect(_Double(q0,q1),q2);//an
	CQuaternion an1= _Bisect(_Double(q1,q2),q3);//an+1
	CQuaternion bn1 = _Double(an1,q2);//bn+1

/*Direct X
	//qn = q1 = p00,an = p10,bn+1 = p20,qn+1 = q2 = p30
	D3DXQUATERNION p00,p10,p20,p30;
	p00.w = q1.w; p10.w = an.w; p20.w = bn1.w; p30.w = q2.w;
	p00.x = q1.x; p10.x = an.x; p20.x = bn1.x; p30.x = q2.x;
	p00.y = q1.y; p10.y = an.y; p20.y = bn1.y; p30.y = q2.y;
	p00.z = q1.z; p10.z = an.z; p20.z = bn1.z; p30.z = q2.z;

	D3DXQUATERNION p01,p11,p21,p02,p12,p03;

	D3DXQuaternionSlerp(&p01,&p00,&p10,t);//p01
	D3DXQuaternionSlerp(&p11,&p10,&p20,t);//p11
	D3DXQuaternionSlerp(&p21,&p20,&p30,t);//p21

	D3DXQuaternionSlerp(&p02,&p01,&p11,t);//p02
	D3DXQuaternionSlerp(&p12,&p11,&p21,t);//p12

	D3DXQuaternionSlerp(&p03,&p02,&p12,t);//qn+t

	rs.w = p03.w; rs.x = p03.x; rs.y = p03.y; rs.z = p03.z;

*/
}
void CQuatInterp::qCatmullRom(CQuaternion &rs,const CQuaternion &q0,const CQuaternion &q1,const CQuaternion &q2,const CQuaternion &q3,const FLOAT t)
{
/* Direct X
	D3DXQUATERNION p00,p10,p20,p30;
	p00.w = q0.w; p10.w = q1.w; p20.w = q2.w; p30.w = q3.w;
	p00.x = q0.x; p10.x = q1.x; p20.x = q2.x; p30.x = q3.x;
	p00.y = q0.y; p10.y = q1.y; p20.y = q2.y; p30.y = q3.y;
	p00.z = q0.z; p10.z = q1.z; p20.z = q2.z; p30.z = q3.z;

	D3DXQUATERNION p01,p11,p21,p02,p12,p03;

	D3DXQuaternionSlerp(&p01,&p00,&p10,t+1);//p01
	D3DXQuaternionSlerp(&p11,&p10,&p20,t);//p11
	D3DXQuaternionSlerp(&p21,&p20,&p30,t-1);//p21

	D3DXQuaternionSlerp(&p02,&p01,&p11,(t+1)/2);//p02
	D3DXQuaternionSlerp(&p12,&p11,&p21,t/2);//p12

	D3DXQuaternionSlerp(&p03,&p02,&p12,t);//qn+t

	rs.w = p03.w; rs.x = p03.x; rs.y = p03.y; rs.z = p03.z;

	*/
}
