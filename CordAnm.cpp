#include "CordAnm.h"

CordAnm::CordAnm()
{
}

void CordAnm::getCordAnmHeader(const char *dir,HCordAnmHeader *pHead)
{
    FILE *f = fopen(dir,"r");
    if(f == NULL)
        return;
    fscanf(f,"OBJECT_NAME:%s\n",pHead->m_name);
    fscanf(f,"BONE_NUM:%d\n",&pHead->m_boneNum);
    fscanf(f,"FRAME_NUM:%d\n",&pHead->m_frameNum);

    fclose(f); f = NULL;
}
void CordAnm::parse(const char *dir,HCordAnmHeader *pHead,int *parent_of,float *data)
{
    FILE *f = fopen(dir,"r");
    if(f == NULL)
        return;
    fscanf(f,"OBJECT_NAME:%s\n",pHead->m_name);
    fscanf(f,"BONE_NUM:%d\n",&pHead->m_boneNum);
    fscanf(f,"FRAME_NUM:%d\n",&pHead->m_frameNum);
    fscanf(f,"BONE_PARENT_ID:");
    for(int i = 0;i < pHead->m_boneNum;i++)
        fscanf(f,"%d",&parent_of[i]);
    fscanf(f,"\n");
    fscanf(f,"FRAME_DATA:\n");

    for(int i = 0;i < pHead->m_frameNum;i++)
    {
        for(int j = 0;j < pHead->m_boneNum*3;j++)
        {
            fscanf(f,"%f",&data[ i*(pHead->m_boneNum*3) + j ]);
        }
        fscanf(f,"\n");
    }
    fclose(f); f = NULL;
}
void CordAnm::restore(const char *dir,const HCordAnmHeader *pHead,const int *parent_of,const float *data)
{
    FILE *f = fopen(dir,"w");
    if(f == NULL)
        return;
    fprintf(f,"OBJECT_NAME:%s\n",pHead->m_name);
    fprintf(f,"BONE_NUM:%d\n",pHead->m_boneNum);
    fprintf(f,"FRAME_NUM:%d\n",pHead->m_frameNum);
    fprintf(f,"BONE_PARENT_ID:");
    for(int i = 0;i < pHead->m_boneNum;i++)
        fprintf(f,"%d ",parent_of[i]);
    fprintf(f,"\n");
    fprintf(f,"FRAME_DATA:\n");

    for(int i = 0;i < pHead->m_frameNum;i++)
    {
        for(int j = 0;j < pHead->m_boneNum*3;j++)
        {
            fprintf(f,"%f ",data[ i*(pHead->m_boneNum*3) + j ]);
        }
        fprintf(f,"\n");
    }
    fclose(f); f = NULL;
}
