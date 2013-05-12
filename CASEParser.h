#ifndef CASEPARSER_H
#define CASEPARSER_H

#include <stdio.h>
#include <string.h>
#include "CVector3f.h"
#include "CQuaternion.h"
#include "AAP.h"

#pragma warning(disable:4244)
#pragma warning(disable:4996)


#ifndef MAX_OBJECT
#define MAX_OBJECT 100
#endif

#ifndef MAX_HELPER_OBJECT
#define MAX_HELPER_OBJECT 100
#endif


struct ActionHeader
{
    int m_firstFrame;
    int m_lastFrame;
    int m_frameSpeed;
    int m_tpf;//time per frame
    int m_boneAnmCount;//object
    int m_bonePosCount;
    int m_boneRotCount;

	int m_helpAnmCount;//helper object
	int m_helpRotCount;
	int m_helpPosCount;

	char m_comment[256];
	char m_fileName[256];

	float BACKGROUND_STATIC[3];
	float AMBIENT_STATIC[3];
	
	bool m_isHelperObject[MAX_OBJECT + MAX_HELPER_OBJECT];//to mark this object is helper or not

	int m_boneRotIndex[MAX_OBJECT];//boject
	int m_bonePosIndex[MAX_OBJECT];

	int m_helpRotIndex[MAX_HELPER_OBJECT];//helper object
	int m_helpPosIndex[MAX_HELPER_OBJECT];
};
struct AnmHeaderBase
{
    int m_numPoskey,m_numRotkey;
    char m_nodeName[256];
    char m_nodeParentName[256];
	
	float INHERIT_POS[3];
	float INHERIT_ROT[3];
	float INHERIT_SCL[3];
	
	float TM_ROW0[3];
	float TM_ROW1[3];
	float TM_ROW2[3];
	float TM_ROW3[3];
	
	float m_initPos[3],m_initRot[4],m_initScale[3];//TM_POS TM_QUATERNION
	
	float TM_ROTAXIS[3];
	float TM_ROTANGLE;
	float TM_SCALEAXIS[3];
	float TM_SCALEAXISANG;
};
struct AnmHeader:public AnmHeaderBase//GEOMOBJECT Header information
{
	int PROP_MOTIONBLUR;
	int PROP_CASTSHADOW;
	int PROP_RECVSHADOW;
};
struct HAnmHeader:AnmHeaderBase//helper object
{
	char m_className[20];

	float BOUNDINGBOX_MIN[3];
	float BOUNDINGBOX_MAX[3];
};

class CASEParser
{
public:
        CASEParser(void);
        ~CASEParser(void);
        /*get action header*/
        void getActionHeader(const char *dir,ActionHeader *pActHead);

        /*before you call parse,call getActionHeader first!!!*/
        void parse(const char *dir,\
			CVector3f *poskey,CVector3f *hposkey,\
			CQuaternion *rotkey,CQuaternion *hrotkey,\
			CAAP *aapkey,CAAP *haapkey,\
			AnmHeader *anmHead,HAnmHeader *hanmHead,\
			const ActionHeader *pActHead);
		void restore(const char *dir,\
			const CVector3f *poskey,const CVector3f *hposkey,\
			const CQuaternion *rotkey,const CQuaternion *hrotkey,\
			const CAAP *aapkey,const CAAP *haapkey,\
			const AnmHeader *anmHead,const HAnmHeader *hanmHead,\
                        const ActionHeader *pActHead);\
private:
                        void initActionHeader(ActionHeader *pActHead);
private:
	bool CALLED;/*remark whether getActionHeader func is called or not
			   *if called remark true,otherwise remark false
			   */
};

#endif // CASEPARSER_H
