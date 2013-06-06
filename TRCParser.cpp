#include "TRCParser.h"


CTRCParser::CTRCParser(void)
{
}

CTRCParser::~CTRCParser(void)
{
}

void CTRCParser::getTRCHeader(const char *dir,HTRCHeader *pHead)
{
	FILE *f = fopen(dir,"r");
	fscanf(f,"PathFileType\t%d\t(%c/%c/%c)\t%s\n",\
		&pHead->m_pathFileType,\
		&pHead->m_XYZ[0],&pHead->m_XYZ[1],&pHead->m_XYZ[2],\
		pHead->m_fileName);
	fscanf(f,"DataRate\tCameraRate\tNumFrames\tNumMarkers\tUnits\tOrigDataRate\tOrigDataStartFrame\tOrigNumFrames\n");
	fscanf(f,"%d\t%d\t%d\t%d\t%s\t%d\t%d\t%d\n",\
		&pHead->m_dataRate,&pHead->m_camerRate,&pHead->m_numFrames,\
		&pHead->m_numMarkers,pHead->m_units,&pHead->m_origDataRate,\
		&pHead->m_origStartFrame,&pHead->m_origNumFrames);
	fclose(f); f = NULL;
}
void CTRCParser::parse(const char *dir,HTRCHeader *pHead,CVector3f *mat)
{
	FILE *f = fopen(dir,"r");
	fscanf(f,"PathFileType\t%d\t(%c/%c/%c)\t%s\n",\
		&pHead->m_pathFileType,\
		&pHead->m_XYZ[0],&pHead->m_XYZ[1],&pHead->m_XYZ[2],\
		pHead->m_fileName);
	fscanf(f,"DataRate\tCameraRate\tNumFrames\tNumMarkers\tUnits\tOrigDataRate\tOrigDataStartFrame\tOrigNumFrames\n");
	fscanf(f,"%d\t%d\t%d\t%d\t%s\t%d\t%d\t%d\n",\
		&pHead->m_dataRate,&pHead->m_camerRate,&pHead->m_numFrames,\
		&pHead->m_numMarkers,pHead->m_units,&pHead->m_origDataRate,\
		&pHead->m_origStartFrame,&pHead->m_origNumFrames);
	fscanf(f,"Frame#\tTime");
	for(int i = 0;i < pHead->m_numMarkers;i++)
	{
		fscanf(f,"\t%d",&pHead->m_markerSeq[i]);

	}
	fscanf(f,"\n\t\t");
	for(int i = 0;i < pHead->m_numMarkers;i++)
	{
            for(int j = 0;j < 3;j++)
            {
                int t; char c;
                fscanf(f,"%c%d\t",&c,&t);
                assert(t == pHead->m_markerSeq[i]+1);
                assert(c == pHead->m_XYZ[j]);
            }
	}
	fscanf(f,"\n");
	for(int i = 0;i < pHead->m_numFrames;i++)
	{
        fscanf(f,"%d\t%lf\t",&pHead->m_frameSeq[i],&pHead->m_timeSeq[i]);
		for(int j = 0;j < pHead->m_numMarkers;j++)
		{
                        int index = INDEX(i,j,pHead->m_numMarkers);
                        double d = 0;
                        for(int k = 0;k < 3;k++)
                        {
                            if(pHead->m_XYZ[k] == 'X')
                            {
                                fscanf(f,"%lf",&d);
                                mat[index].x = (FLOAT)d;
                            }
                            else if(pHead->m_XYZ[k] == 'Y')
                            {
                                fscanf(f,"%lf",&d);
                                mat[index].y = (FLOAT)d;
                            }
                            else if(pHead->m_XYZ[k] == 'Z')
                            {
                                fscanf(f,"%lf",&d);
                                mat[index].z = (FLOAT)d;
                            }
                        }
		}
        fscanf(f,"\n");
	}
	fclose(f); f = NULL;
}

void CTRCParser::resotre(const char *dir,const HTRCHeader *pHead,const CVector3f *mat)
{
	FILE *f = fopen(dir,"w");
	fprintf(f,"PathFileType\t%d\t(%c/%c/%c)\t%s\n",\
		pHead->m_pathFileType,\
		pHead->m_XYZ[0],pHead->m_XYZ[1],pHead->m_XYZ[2],\
		pHead->m_fileName);
	fprintf(f,"DataRate\tCameraRate\tNumFrames\tNumMarkers\tUnits\tOrigDataRate\tOrigDataStartFrame\tOrigNumFrames\n");
	fprintf(f,"%d\t%d\t%d\t%d\t%s\t%d\t%d\t%d\n",\
		pHead->m_dataRate,pHead->m_camerRate,pHead->m_numFrames,\
		pHead->m_numMarkers,pHead->m_units,pHead->m_origDataRate,\
		pHead->m_origStartFrame,pHead->m_origNumFrames);
	fprintf(f,"Frame#\tTime");
	for(int i = 0;i < pHead->m_numMarkers;i++)
	{
		fprintf(f,"\t%d",pHead->m_markerSeq[i]);
	}
	fprintf(f,"\n\t\t");
	for(int i = 0;i < pHead->m_numMarkers;i++)
	{
		for(int j = 0;j < 3;j++)
            fprintf(f,"%c%d\t",pHead->m_XYZ[j],pHead->m_markerSeq[i]+1);
	}
	fprintf(f,"\n");
	for(int i = 0;i < pHead->m_numFrames;i++)
	{
		fprintf(f,"%d\t%f\t",pHead->m_frameSeq[i],pHead->m_timeSeq[i]);
		for(int j = 0;j < pHead->m_numMarkers;j++)
		{
            int index = INDEX(i,j,pHead->m_numMarkers);
            for(int k = 0;k < 3;k++)
            {
                if(pHead->m_XYZ[k] == 'X')
                {
                    fprintf(f,"%0.6f",mat[index].x);
                }
                else if(pHead->m_XYZ[k] == 'Y')
                {
                    fprintf(f,"%0.6f",mat[index].y);
                }
                else if(pHead->m_XYZ[k] == 'Z')
                {
                    fprintf(f,"%0.6f",mat[index].z);
                }
            }
		}
		fprintf(f,"\n");
	}
	fclose(f); f = NULL;
}
