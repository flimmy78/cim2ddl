#include <Windows.h>
#include <process.h>
#include <io.h>
#include <vector>
using namespace std;
#include <crtdbg.h>
#include <float.h>

#include "../../../../../MemDB/PRMemDB/PRMemDB.h"
#if (!defined(WIN64))
#	pragma comment(lib, "../../../../../lib/PRMemDB.lib")
#else
#	pragma comment(lib, "../../../../../lib_x64/PRMemDB.lib")
#endif
using	namespace	PRMemDB;

#include "../../PRAdequacyBase/PRAdequacyBase.h"
using	namespace	PRAdequacyBase;
#if (!defined(WIN64))
#	ifdef _DEBUG
#		pragma comment(lib, "../../../../../lib/libPRAdequacyBaseMDd.lib")
#	else
#		pragma comment(lib, "../../../../../lib/libPRAdequacyBaseMD.lib")
#	endif
#	pragma message("Link LibX86 PRAdequacyBase.lib")
#else
#	ifdef _DEBUG
#		pragma comment(lib, "../../../../../lib_x64/libPRAdequacyBaseMDd.lib")
#	else
#		pragma comment(lib, "../../../../../lib_x64/libPRAdequacyBaseMD.lib")
#	endif
#	pragma message("Link LibX64 PRAdequacyBase.lib")
#endif

#include "../../../../../Common/TinyXML/tinyxml.h"
//using	namespace	TinyXML;
#if (!defined(WIN64))
#	ifdef _DEBUG
#		pragma comment(lib, "../../../../../lib/libTinyXmlMDd.lib")
#	else
#		pragma comment(lib, "../../../../../lib/libTinyXmlMD.lib")
#	endif
#	pragma message("Link LibX86 TinyXml.lib")
#else
#	ifdef _DEBUG
#		pragma comment(lib, "../../../../../lib_x64/libTinyXmlMDd.lib")
#	else
#		pragma comment(lib, "../../../../../lib_x64/libTinyXmlMD.lib")
#	endif
#	pragma message("Link LibX64 TinyXml.lib")
#endif


#ifndef	_MAIN_
#	define _MAIN_
#endif
#undef	_MAIN_

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Ψһ��Ӧ�ó������
const	char*	g_lpszLogFile="GCStateSample.log";
extern	void	ClearLog(const char* lpszLogFile);
extern	void	Log(const char* lpszLogFile, char* pformat, ...);

tagPRBlock*				g_pPRBlock;
CPRMemDBInterface		g_PRMemDBInterface;
CPRAdequacyStateSample	m_StateSample;

void PrintMessage(const char* pformat, ...)
{
	va_list args;
	va_start( args, pformat );

	char	szMesg[1024];

	vsprintf(szMesg, pformat, args);
	vfprintf(stdout, pformat, args);
	fprintf(stdout, "\n");

	Log(g_lpszLogFile, szMesg);

	va_end(args);
}

int main(int argc, char** argv, char** envp)
{
	char	szRunDir[260];
	tagBpaPRAdequacySetting	sBpaPRSetting;
	clock_t	dBeg, dEnd;
	int		nDur;

	ClearLog(g_lpszLogFile);
	GetCurrentDirectory(260, szRunDir);
	InitBpaPRAdequacySetting(&sBpaPRSetting);
	ClearLog(g_lpszLogFile);

	int	nEle=1;
	if (argc > nEle)	strcpy(szRunDir, argv[nEle++]);								else	{	Log(g_lpszLogFile, "Error Arg RunDir\n");			return 0;	}
	if (argc > nEle)	sBpaPRSetting.nPRSampleObject = atoi(argv[nEle++]);			else	{	Log(g_lpszLogFile, "Error Arg SampleMethod\n");		return 0;	}
	if (argc > nEle)	sBpaPRSetting.nPRSampleMethod = atoi(argv[nEle++]);			else	{	Log(g_lpszLogFile, "Error Arg Sample Object\n");	return 0;	}
	if (argc > nEle)	sBpaPRSetting.nMaxGenFault = atoi(argv[nEle++]);			
	if (argc > nEle)	sBpaPRSetting.nMaxBranFault = atoi(argv[nEle++]);			
	if (argc > nEle)	sBpaPRSetting.nMCSSimulateTime = atoi(argv[nEle++]);		
	if (argc > nEle)	sBpaPRSetting.fMCSMinStateProb = atof(argv[nEle++]);		
	if (argc > nEle)	sBpaPRSetting.fFSTMaxCumuProb = atof(argv[nEle++]);			
	if (argc > nEle)	sBpaPRSetting.fFSTMinStateProb = atof(argv[nEle++]);		
	if (argc > nEle)	sBpaPRSetting.nFSTMaxStateNum = atoi(argv[nEle++]);		
	if (argc > nEle)	sBpaPRSetting.nSTSMaxStateNum = atoi(argv[nEle++]);		
	if (argc > nEle)	sBpaPRSetting.fANAMinStateProb = atof(argv[nEle++]);

	for (nEle=0; nEle<argc; nEle++)
		Log(g_lpszLogFile, "Argc[%d] = %s\n", nEle+1, argv[nEle]);

// 	unsigned char	nPRSampleObject;		//	�����������ͣ�0ȫ����1֧·��2�����
// 	unsigned char	nPRSampleMethod;		//	��������
// 	int				nMaxGenFault;			//	MCS, FST, STS, ANA ������󷢵����������
// 	int				nMaxBranFault;			//	MCS, FST, STS, ANA �������֧·��������
// 	int				nMCSSimulateTime;		//	MCS����������ʱ��
// 	double			fMCSMinStateProb;		//	MCS[���ؿ���]�豸���ϸ����ż�ֵ
// 	double			fFSTMaxCumuProb;		//	FST[��������]�ۻ�����
// 	double			fFSTMinStateProb;		//	FST[��������]�豸���ϸ����ż�ֵ
// 	int				nFSTMaxStateNum;		//	FST[��������]���״̬��
// 	int				nSTSMaxStateNum;		//	STS[״̬����]���״̬��
// 	double			fANAMinStateProb;		//	ANA[ ������ ]�豸���ϸ����ż�ֵ

	dBeg=clock();

	{
		g_pPRBlock=(tagPRBlock*)g_PRMemDBInterface.Init_PRBlock();
		if (!g_pPRBlock)
		{
			PrintMessage("��ȡPR�ڴ�����\n");
			return FALSE;
		}
	}

	int	nStateNum=m_StateSample.Sample(g_pPRBlock, sBpaPRSetting.nPRSampleObject, sBpaPRSetting.nPRSampleMethod, &sBpaPRSetting);


	dEnd=clock();
	nDur=(int)((1000.0*(double)(dEnd-dBeg))/CLOCKS_PER_SEC);
	PrintMessage("״̬������ϣ���ʱ%d���� ״̬��=%d\n",nDur, nStateNum);
}
