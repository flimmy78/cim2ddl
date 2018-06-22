
// GCStateEstimateServer.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "GCStateEstimateNetwork.h"
#include "GCStateEstimateControlThread.h"

// CGCStateEstimateServerApp:
// �йش����ʵ�֣������ GCStateEstimateServer.cpp
//

class CGCStateEstimateServerApp : public CWinAppEx
{
public:
	CGCStateEstimateServerApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CGCStateEstimateServerApp theApp;
extern	tagPRBlock*			g_pPRBlock;
extern	CPRMemDBInterface	g_PRMemDBInterface;

extern	unsigned int	g_nLocalPort;
extern	unsigned int	g_nRemotePort;
extern	char			g_szRemoteAddr[48];

extern	void	ReadIni(void);
extern	void	SaveIni(void);

extern	const	char*	g_lpszLogFile;
extern	void	Log(const char* lpszLogFile, char* pformat, ...);
extern	void	ClearLog(const char*);
extern	void	ClearMessage();
extern	void	PrintMessage(const char* lpszFormat, ...);

extern	CNetworkInterface		g_NetInterface;

extern	CPRAdequacyEstimate		g_PRAdeEstimate;

extern	CGCStateEstimateControlThread	g_StateEstimateControlThread;
extern	void	StartNetworkService(CWinThread* pDataProcThread, const int nSvrPort);
extern	void	StopNetworkService();
extern	int		SendNetworkData(const char* lpszAddr, const int nPort, const char* lpszData);
