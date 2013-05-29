
/**
 * CordAnm.h
 *
 * Copyright (c) 2013, Dalian Nationalities University. All Rights  Reserved.
 * Tianyi Song <songtianyi630@163.com>
 *
 * You can use this library in your project, but do not redistribute it and/or modify
 * it.
 *
 */

#ifndef CORDANM_H
#define CORDANM_H




#include <stdio.h>
#include <stdlib.h>
#include "def.h"
#include "CVector3f.h"

#ifdef MS_BUILD_ENV
#pragma warning(disable:4244)
#pragma warning(disable:4996)
#endif

struct HCordAnmHeader
{
    int m_frameNum;
    int m_boneNum;
    char m_name[256];
};
class CordAnm
{
public:
    CordAnm();
    void getCordAnmHeader(const char *dir,HCordAnmHeader *pHead);
    void restore(const char *dir,const HCordAnmHeader *pHead,const int *parent_of,const CVector3f *data);
    void parse(const char *dir,HCordAnmHeader *pHead,int *parent_of,CVector3f *data);
};

#endif // CORDANM_H
