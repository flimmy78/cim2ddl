
// GCModuleTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "../../../../../Network/NetworkBase/NetworkInterface.h"

#ifndef UM_MESSAGE
#	define UM_MESSAGE			WM_APP+1000
#endif

#define	Command_DataReady		"DataReady"
#define	Command_StateEstimate	"StateEstimate"
#define	Command_JobFinished		"JobFinished"

typedef	void (*vFun_v)(void*);
typedef struct	_Thread_Arg_
{
	SOCKET				sSocket;
	char				szPeerAddr[48];
	unsigned int		nPeerPort;
	CWnd*				pMainWnd;
	CWinThread*			pDataProcThread;
	vFun_v				pNetProcFunc;	//	���������ݴ���ģ��
}	tagThreadArg;

// CGCModuleTestApp:
// �йش����ʵ�֣������ GCModuleTest.cpp
//

class CGCModuleTestApp : public CWinAppEx
{
public:
	CGCModuleTestApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CGCModuleTestApp		theApp;
extern	tagBpaBlock*		g_pBpaBlock;
extern	tagPRBlock*			g_pPRBlock;
extern	CPRMemDBInterface	g_PRMemDBInterface;
extern	char				g_szRemoteAddr[48];
extern	unsigned int		g_nRemotePort;
extern	unsigned int		g_nLocalPort;
extern	CNetworkInterface	g_NetInterface;
extern	int					g_nThreadNum;
extern	char				g_szRunDir[260];

extern	tagBpaPRAdequacySetting		g_PRAdeSetting;

extern	void	PrintMessage(const char* lpszFormat, ...);
extern	void	ClearLog(const char* lpszLogFile);
extern	void	Log(const char* lpszLogFile, char* pformat, ...);
extern	const	char*	g_lpszLogFile;

extern	void	StartNetworkService(const int nSvrPort);
extern	int		SendNetworkData(const char* lpszAddr, const int nPort, const char* lpszData);

extern	void	PRStateEstimateCalculateInit(tagPRBlock* pPRBlock);
extern	void	PRStateEstimateCalculateExit();
extern	void	PRStateEstimateCalculateDispatch(CWnd* pMainWnd, tagPRBlock* pPRBlock);
extern	void	PRStateEstimateReturn(CWnd* pMainWnd, Json::Value& sContent);

extern	HANDLE	InitSem(const char* lpszSemKey);
extern	const	char*	g_lpszGCPRStateEstimateSemaphore;
