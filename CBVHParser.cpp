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
            #ifdef FLOAT_32
            fscanf(bvh,"%f%f%f",&pJoint[pHeader->m_jointNum].m_offset[0],\
                   &pJoint[pHeader->m_jointNum].m_offset[1],\
                   &pJoint[pHeader->m_jointNum].m_offset[2]);
            #endif

            #ifdef FLOAT_64
            fscanf(bvh,"%lf%lf%lf",&pJoint[pHeader->m_jointNum].m_offset[0],\
                   &pJoint[pHeader->m_jointNum].m_offset[1],\
                   &pJoint[pHeader->m_jointNum].m_offset[2]);
            #endif // FLOAT_64
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
#ifdef FLOAT_32
    fscanf(bvh," MOTION Frames:%d Frame Time:%f",&pHeader->m_frameNum,&pHeader->m_frameTime);
#endif // FLOAT_32
#ifdef FLOAT_64
    fscanf(bvh," MOTION Frames:%d Frame Time:%lf",&pHeader->m_frameNum,&pHeader->m_frameTime);
#endif // FLOAT_64


    assert((6 + (pHeader->m_jointNum - pHeader->m_endSiteNum - 1)*3) == pHeader->m_columNum);
    assert(pHeader->m_frameNum < BVH_MAX_FRAME);
    assert(pHeader->m_jointNum < BVH_MAX_JOINT);
    fclose( bvh ); bvh = 0;
}
void CBVHParser::parse(const char *dir,HBVHHead *pHeader,HBVHJoint *pJoint,CVector3f *mat)
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
            #ifdef FLOAT_32
            fscanf(bvh,"%f%f%f",&pJoint[pHeader->m_jointNum].m_offset[0],\
                   &pJoint[pHeader->m_jointNum].m_offset[1],\
                   &pJoint[pHeader->m_jointNum].m_offset[2]);
            #endif // FLOAT_32

            #ifdef FLOAT_64
            fscanf(bvh,"%lf%lf%lf",&pJoint[pHeader->m_jointNum].m_offset[0],\
                   &pJoint[pHeader->m_jointNum].m_offset[1],\
                   &pJoint[pHeader->m_jointNum].m_offset[2]);
            #endif // FLOAT_64

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
    #ifdef FLOAT_32
    fscanf(bvh," MOTION Frames:%d Frame Time:%f",&pHeader->m_frameNum,&pHeader->m_frameTime);
    #endif
    #ifdef FLOAT_64
    fscanf(bvh," MOTION Frames:%d Frame Time:%lf",&pHeader->m_frameNum,&pHeader->m_frameTime);
    #endif // FLOAT_64
    assert((6 + (pHeader->m_jointNum - pHeader->m_endSiteNum - 1)*3) == pHeader->m_columNum);
    assert(pHeader->m_frameNum < BVH_MAX_FRAME);
    assert(pHeader->m_jointNum < BVH_MAX_JOINT);
    assert(pHeader->m_columNum % 3 == 0);
    int nVecNum = pHeader->m_frameNum*pHeader->m_columNum%3;

    for(int i = 0;i < nVecNum;i++)
    {
        #ifdef FLOAT_32
        fscanf(bvh,"%f %f %f",&mat[i].x,&mat[i].y,&mat[i].z);
        #endif // FLOAT_32

        #ifdef FLOAT_64
        fscanf(bvh,"%lf %lf %lf",&mat[i].x,&mat[i].y,&mat[i].z);
        #endif // FLOAT_64

    }

    fclose( bvh ); bvh = 0;
}

void CBVHParser::restore(const char *dir,const HBVHHead *pHeader,const HBVHJoint *pJoint,const CVector3f *mat)
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
    assert(pHeader->m_columNum % 3 == 0);
    for(int i = 0;i < pHeader->m_frameNum;i++)
    {
        const CVector3f *array = &mat[i*pHeader->m_columNum%3];
        fprintf(re,"%0.4f %0.4f %0.4f",array[0].x,array[0].y,array[0].z);
        for(int j = 1;j < pHeader->m_columNum;j++)
        {
            fprintf(re," %0.4f %0.4f %0.4f",array[j].x,array[j].y,array[j].z);
        }
        fprintf(re,"\n");
    }
    fclose(re); re = 0;
}
