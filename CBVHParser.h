#ifndef CBVHPARSER_H
#define CBVHPARSER_H

#include <cstdio>
#include <cstring>
#include <cassert>
#include "CStack.h"
using namespace std;



#ifndef BVH_MAX_JOINT
#define BVH_MAX_JOINT 256
#endif

#ifndef BVH_MAX_FRAME
#define BVH_MAX_FRAME 1000
#endif

#pragma warning(disable:4244)
#pragma warning(disable:4996)

struct HBVHJoint
{
    int m_id;//one-based,[1,BVH_MAX_JOINT]
    int m_channelNum;//channel {3,6}
    int m_channels[6];//{0,1,2} - {X,Y,Z}
    char m_jointName[256];
    float m_offset[3];//offset value
};
struct HBVHHead
{
    int m_jointNum;//joint count
    int m_endSiteNum;//END SITE count

    int m_columNum;//column num
    int m_frameNum;//frame count,row num

    float m_frameTime;//time per frame

    int *m_parentOf;
    bool *m_isEndSite;//note: you need to alloc memory your self, cause i don't know the memory size !
    ~HBVHHead()
    {
        assert(m_parentOf == 0);
        assert(m_isEndSite== 0);
    }
};

/*
 *@brief parse the data in BVH file 
	or restore the data to BVH file
 */
class CBVHParser
{
public:
        CBVHParser();
        ~CBVHParser();
        void parse(const char *dir,HBVHHead *pHeader,HBVHJoint *pJoint,float *mat);
        void restore(const char *dir,const HBVHHead *pHeader,const HBVHJoint *pJoint,const float *mat);
};

#endif // CBVHPARSER_H
