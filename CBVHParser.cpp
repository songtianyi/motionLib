#include "CBVHParser.h"

CBVHParser::CBVHParser()
{
    //ctor
}

CBVHParser::~CBVHParser()
{
    //dtor
}
void getBVHHeader(const char *dir,HBVHHead *pHeader,HBVHJoint *pJoint)
{
    FILE *bvh = fopen(dir,"r");
    if( bvh == NULL )
    {
        printf("[open file error] %s\n",dir);
        return;
    }
        char buffer[256];
    CStack my(BVH_MAX_JOINT+1);

    //initialization
    memset(pHeader->m_parentOf,-1,sizeof(int)*(BVH_MAX_JOINT+1));
    memset(pHeader->m_isEndSite,0,sizeof(bool)*(BVH_MAX_JOINT+1));


    pHeader->m_parentOf[1] = 0;

    //hierarchy construct
    while( fscanf(bvh,"%s",buffer) != EOF )
    {
        if( strcmp(buffer,"{") == 0 )
        {
            my.push(pHeader->m_jointNum);
        }
        else if( strcmp(buffer,"}") == 0 )
        {
            int c = my.top(); my.pop();

            if(c == 1)
            {
                //back to root
                break;
            }

            pHeader->m_parentOf[ c ] = my.top();

        }
        else if( strcmp(buffer,"OFFSET") == 0 )
        {
            fscanf(bvh,"%f%f%f",&pJoint[pHeader->m_jointNum].m_offset[0],\
                   &pJoint[pHeader->m_jointNum].m_offset[1],\
                   &pJoint[pHeader->m_jointNum].m_offset[2]);
        }
        else if( strcmp(buffer,"CHANNELS") == 0)
        {
            fscanf(bvh,"%d",&pJoint[pHeader->m_jointNum].m_channelNum);
            pHeader->m_columNum += pJoint[pHeader->m_jointNum].m_channelNum;
            for(int i = 0;i < pJoint[pHeader->m_jointNum].m_channelNum; i++)
            {
                fscanf(bvh,"%s",buffer);
                //x = 0, y = 1, z = 2
                //position rotation | rotation
                if( buffer[0] == 'X')
                {
                    pJoint[pHeader->m_jointNum].m_channels[i] = 0;
                }
                else if( buffer[0] == 'Y' )
                {
                    pJoint[pHeader->m_jointNum].m_channels[i] = 1;
                }
                else if( buffer[0] == 'Z')
                {
                    pJoint[pHeader->m_jointNum].m_channels[i] = 2;
                }
            }
        }
        else if( strcmp(buffer,"JOINT") == 0 || strcmp(buffer,"ROOT") == 0 || strcmp(buffer,"End") == 0)
        {
            fscanf(bvh,"%s",pJoint[++pHeader->m_jointNum].m_jointName);

            if(buffer[0] == 'E')
            {
                pHeader->m_isEndSite[pHeader->m_jointNum] = true;
                pHeader->m_endSiteNum++;
            }
        }
    }
    my.free();
    //end of construction

    fscanf(bvh," MOTION Frames:%d Frame Time:%f",&pHeader->m_frameNum,&pHeader->m_frameTime);

    assert((6 + (pHeader->m_jointNum - pHeader->m_endSiteNum - 1)*3) == pHeader->m_columNum);
    assert(pHeader->m_frameNum < BVH_MAX_FRAME);
    assert(pHeader->m_jointNum < BVH_MAX_JOINT);
    fclose( bvh ); bvh = 0;
}
void CBVHParser::parse(const char *dir,HBVHHead *pHeader,HBVHJoint *pJoint,float *mat)
{
    FILE *bvh = fopen( dir,"r" );
    if( bvh == NULL )
    {
        printf("[open file error] %s\n",dir);
        return;
    }

    char buffer[256];
    CStack my(BVH_MAX_JOINT+1);

    //initialization
    pHeader->m_jointNum = 0; pHeader->m_endSiteNum = 0; pHeader->m_columNum = 0;
    memset(pHeader->m_parentOf,-1,sizeof(int)*(BVH_MAX_JOINT+1));
    memset(pHeader->m_isEndSite,0,sizeof(bool)*(BVH_MAX_JOINT+1));


    pHeader->m_parentOf[1] = 0;

    //hierarchy construct
    while( fscanf(bvh,"%s",buffer) != EOF )
    {
        if( strcmp(buffer,"{") == 0 )
        {
            my.push(pHeader->m_jointNum);
        }
        else if( strcmp(buffer,"}") == 0 )
        {
            int c = my.top(); my.pop();

            if(c == 1)
            {
                //back to root
                break;
            }

            pHeader->m_parentOf[ c ] = my.top();

        }
        else if( strcmp(buffer,"OFFSET") == 0 )
        {
            fscanf(bvh,"%f%f%f",&pJoint[pHeader->m_jointNum].m_offset[0],\
                   &pJoint[pHeader->m_jointNum].m_offset[1],\
                   &pJoint[pHeader->m_jointNum].m_offset[2]);
        }
        else if( strcmp(buffer,"CHANNELS") == 0)
        {
            fscanf(bvh,"%d",&pJoint[pHeader->m_jointNum].m_channelNum);
            pHeader->m_columNum += pJoint[pHeader->m_jointNum].m_channelNum;
            for(int i = 0;i < pJoint[pHeader->m_jointNum].m_channelNum; i++)
            {
                fscanf(bvh,"%s",buffer);
                //x = 0, y = 1, z = 2
                //position rotation | rotation
                if( buffer[0] == 'X')
                {
                    pJoint[pHeader->m_jointNum].m_channels[i] = 0;
                }
                else if( buffer[0] == 'Y' )
                {
                    pJoint[pHeader->m_jointNum].m_channels[i] = 1;
                }
                else if( buffer[0] == 'Z')
                {
                    pJoint[pHeader->m_jointNum].m_channels[i] = 2;
                }
            }
        }
        else if( strcmp(buffer,"JOINT") == 0 || strcmp(buffer,"ROOT") == 0 || strcmp(buffer,"End") == 0)
        {
            fscanf(bvh,"%s",pJoint[++pHeader->m_jointNum].m_jointName);

            if(buffer[0] == 'E')
            {
                pHeader->m_isEndSite[pHeader->m_jointNum] = true;
                pHeader->m_endSiteNum++;
            }
        }
    }
    my.free();
    //end of construction

    fscanf(bvh," MOTION Frames:%d Frame Time:%f",&pHeader->m_frameNum,&pHeader->m_frameTime);

    assert((6 + (pHeader->m_jointNum - pHeader->m_endSiteNum - 1)*3) == pHeader->m_columNum);
    assert(pHeader->m_frameNum < BVH_MAX_FRAME);
    assert(pHeader->m_jointNum < BVH_MAX_JOINT);
    int nFloatNum = pHeader->m_frameNum*pHeader->m_columNum;

    for(int i = 0;i < nFloatNum;i++)
    {
        fscanf(bvh,"%f",&mat[i]);
    }

    fclose( bvh ); bvh = 0;
}

void CBVHParser::restore(const char *dir,const HBVHHead *pHeader,const HBVHJoint *pJoint,const float *mat)
{
    assert(pHeader->m_frameNum > 0 && pHeader->m_jointNum > 0);
    assert(pHeader->m_parentOf[1] == 0);//root joint is 1

    FILE *re = fopen(dir,"w");
    if(re == NULL)
    {
        printf("[open file error] %s\n",dir);
        return;
    }
    fprintf(re,"HIERARCHY\n");

    CStack my(BVH_MAX_JOINT+1); my.push(0);
    for(int i = 1 ;i <= pHeader->m_jointNum;i++)
    {
        while(pHeader->m_parentOf[i] != my.top())
        {
            fprintf(re,"}\n");
            my.pop();
        }
        assert(pHeader->m_parentOf[i] == my.top());

        if(i == 1)
        {
            fprintf(re,"ROOT %s\n{\n\tOFFSET %.5f %.5f %.5f\n\tCHANNELS %d %cposition %cposition %cposition %crotation %crotation %crotation\n",\
                pJoint[i].m_jointName,\
                pJoint[i].m_offset[0],pJoint[i].m_offset[1],pJoint[i].m_offset[2],\
                pJoint[i].m_channelNum,\
                pJoint[i].m_channels[0]+'X',pJoint[i].m_channels[1]+'X',pJoint[i].m_channels[2]+'X',\
                pJoint[i].m_channels[3]+'X',pJoint[i].m_channels[4]+'X',pJoint[i].m_channels[5]+'X');
        }
        else if(pHeader->m_isEndSite[i] == 1)
        {
            fprintf(re,"End Site\n{\n\tOFFSET %.5f %.5f %.5f\n",pJoint[i].m_offset[0],pJoint[i].m_offset[1],pJoint[i].m_offset[2]);
        }
        else
        {
            fprintf(re,"JOINT %s\n{\n\tOFFSET %.5f %.5f %.5f\n\tCHANNELS %d %crotation %crotation %crotation\n",\
                pJoint[i].m_jointName,\
                pJoint[i].m_offset[0],pJoint[i].m_offset[1],pJoint[i].m_offset[2],\
                pJoint[i].m_channelNum,\
                pJoint[i].m_channels[0]+'X',pJoint[i].m_channels[1]+'X',pJoint[i].m_channels[2]+'X');
        }
        my.push(i);
    }
    while(my.top() != 0){fprintf(re,"}\n");my.pop();}
    my.free();
    fprintf(re,"MOTION\nFrames: %d\nFrame Time: %f\n",pHeader->m_frameNum,pHeader->m_frameTime);

    for(int i = 0;i < pHeader->m_frameNum;i++)
    {
        const float *array = &mat[i*pHeader->m_columNum];
        fprintf(re,"%0.4f",array[0]);
        for(int j = 1;j < pHeader->m_columNum;j++)
        {
            fprintf(re," %0.4f",array[j]);
        }
        fprintf(re,"\n");
    }
    fclose(re); re = 0;
}
