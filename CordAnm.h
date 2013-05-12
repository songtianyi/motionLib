#ifndef CORDANM_H
#define CORDANM_H


#include <stdio.h>
#include <stdlib.h>

#pragma warning(disable:4244)
#pragma warning(disable:4996)

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
    void restore(const char *dir,const HCordAnmHeader *pHead,const int *parent_of,const float *data);
   void parse(const char *dir,HCordAnmHeader *pHead,int *parent_of,float *data);
};

#endif // CORDANM_H
