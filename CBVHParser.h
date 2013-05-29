
/**
 * CBVHParser.h
 *
 * Copyright (c) 2013, Dalian Nationalities University. All Rights  Reserved.
 * Tianyi Song <songtianyi630@163.com>
 *
 * You can use this library in your project, but do not redistribute it and/or modify
 * it.
 *
 */

#ifndef CBVHPARSER_H
#define CBVHPARSER_H

/**
 *@brief parse BVH file
	 restore the data to BVH file
 */


#include <cstdio>
#include <cstring>
#include <cassert>
#include "CStack.h"
#include "CVector3f.h"
#include "def.h"
using namespace std;




#define BVH_MAX_JOINT 256
#define BVH_MAX_FRAME 1000

#ifdef MS_BUILD_ENV
#pragma warning(disable:4244)
#pragma warning(disable:4996)
#endif

struct HBVHJoint
{
    int m_id;//one-based,[1,BVH_MAX_JOINT]
    int m_channelNum;//channel {3,6}
    int m_channels[6];//{0,1,2} - {X,Y,Z}
    char m_jointName[256];
    FLOAT m_offset[3];//offset value
};
struct HBVHHead
{
    int m_jointNum;//joint count
    int m_endSiteNum;//END SITE count

    int m_columNum;//column num
    int m_frameNum;//frame count or row num

    FLOAT m_frameTime;//time per frame

    int *m_parentOf;
    bool *m_isEndSite;//note: you need to alloc memory your self, cause i don't know the memory size !
    HBVHHead()
    {
        m_parentOf = 0;
        m_isEndSite = 0;

        m_jointNum = 0;
        m_endSiteNum=0;
        m_columNum=0;
        m_frameNum=0;

        m_frameTime = 0.0;
    }
    ~HBVHHead()
    {
        assert(m_parentOf == 0);
        assert(m_isEndSite== 0);
    }
    void alloc()
    {
        m_parentOf = new int[BVH_MAX_JOINT+1];
        m_isEndSite= new bool[BVH_MAX_JOINT+1];
    }
    void dealloc()
    {
        delete [] m_parentOf; m_parentOf = 0;
        delete [] m_isEndSite; m_isEndSite = 0;
    }
};


class CBVHParser
{
public:
        CBVHParser();
        ~CBVHParser();
        void getBVHHeader(const char *dir,HBVHHead *pHeader,HBVHJoint *pJoint);
        void parse(const char *dir,HBVHHead *pHeader,HBVHJoint *pJoint,CVector3f *mat);
        void restore(const char *dir,const HBVHHead *pHeader,const HBVHJoint *pJoint,const CVector3f *mat);
};

#endif // CBVHPARSER_H
