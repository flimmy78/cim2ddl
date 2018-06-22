
// GCStateEstimateServer.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "GCStateEstimateServer.h"
#include "GCStateEstimateServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGCStateEstimateServerApp

BEGIN_MESSAGE_MAP(CGCStateEstimateServerApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CGCStateEstimateServerApp ����

CGCStateEstimateServerApp::CGCStateEstimateServerApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CGCStateEstimateServerApp ����

CGCStateEstimateServerApp theApp;
const	char*	g_lpszLogFile="GCStateEstimateServer.log";
const	static	char*	lpszIniFile="GCStateEstimateServer.ini";

// CGCStateEstimateServerApp ��ʼ��
unsigned int			g_nLocalPort;
unsigned int			g_nRemotePort;
char					g_szRemoteAddr[48];
tagPRBlock*				g_pPRBlock;
CPRMemDBInterface		g_PRMemDBInterface;

CNetworkInterface		g_NetInterface;

CPRAdequacyEstimate		g_PRAdeEstimate;

CGCStateEstimateControlThread	g_StateEstimateControlThread;

HANDLE CreateOneAppMutex(const char* lpszName)
{
	HANDLE	hMutex = CreateMutex(NULL, 1, lpszName);   // Create mutex
	switch (GetLastError())
	{
	case ERROR_SUCCESS:
		//
		// Mutex created successfully. There is no instances running
		//
		break;

	case ERROR_ALREADY_EXISTS:
		//
		// Mutex already exists so there is a running instance of our app.
		//
		hMutex = NULL;
		break;

	default:
		//
		// Failed to create mutex by unkniown reason
		//
		break;
	}

	return hMutex;
}

BOOL CGCStateEstimateServerApp::InitInstance()
{
	//_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(1276);
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_LEAK_CHECK_DF);
	if (!CreateOneAppMutex("ZhongND_GCStateEstimateService"))
	{
		AfxMessageBox("��������Ѿ�����");
		return FALSE;
	}

	ReadIni();
	ClearLog(g_lpszLogFile);

	g_pPRBlock=(tagPRBlock*)g_PRMemDBInterface.Init_PRBlock();
	if (!g_pPRBlock)
	{
		AfxMessageBox("��ʼ���ɿ������ݿ����");
		return FALSE;
	}

	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CGCStateEstimateServerDlg dlg;
	m_pMainWnd = &dlg;

	if (!g_StateEstimateControlThread.Create(m_pMainWnd, this, g_szRemoteAddr, g_nRemotePort))
	{
		AfxMessageBox("�����ն������߳�ʧ��!");
		return FALSE;
	}

	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	g_StateEstimateControlThread.Close();

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

void ReadIni(void)
{
	char	szLine[1024];
	int		nReadContent, nIndex;
	int		nRow, nCol;
	std::vector<std::string>	strEleArray;
	FILE*	fp;

	g_nLocalPort=8950;
	g_nRemotePort=9002;
	strcpy(g_szRemoteAddr, "127.0.0.1");

	fp=fopen(lpszIniFile, "r");
	if (fp == NULL)
		return;

	nIndex=nRow=nCol=0;
	nReadContent=0;
	while (!feof(fp))
	{
		memset(szLine, 0, 1024);
		fgets(szLine, 1024, fp);
		if (strstr(szLine, "[GCStateEstimateServiceConfig]") != NULL)
		{
			nReadContent=1;
			continue;
		}
		else if (strstr(szLine, "[END]") != NULL || strstr(szLine, "[End_GCStateEstimateServiceConfig]") != NULL)
		{
			nReadContent=0;
			continue;
		}
		if (nReadContent == 1)
		{
			strEleArray.clear();
			char*	lpszToken=strtok(szLine, " \t\n=");
			while (lpszToken != NULL)
			{
				strEleArray.push_back(lpszToken);
				lpszToken=strtok(NULL, " \t\n=");
			}
			if (strEleArray.size() == 2)
			{
				if (strstr(szLine, "LocalPort") != NULL)		g_nLocalPort=atoi(strEleArray[1].c_str());
				else if (strstr(szLine, "RemoteAddr") != NULL)	strcpy(g_szRemoteAddr, strEleArray[1].c_str());
				else if (strstr(szLine, "RemotePort") != NULL)	g_nRemotePort=atoi(strEleArray[1].c_str());
			}
		}
	}
}

void SaveIni(void)
{
	FILE*	fp;

	fp=fopen(lpszIniFile, "w");
	if (fp == NULL)
		return;

	fprintf(fp, "[GCStateEstimateServiceConfig]\n");
	fprintf(fp, "LocalPort=%d\n",	g_nLocalPort);
	fprintf(fp, "RemoteAddr=%s\n",	g_szRemoteAddr);
	fprintf(fp, "RemotePort=%d\n",	g_nRemotePort);
	fprintf(fp, "[End_GCStateEstimateServiceConfig]\n");

	fprintf(fp, "[END]\n");

	fflush(fp);
	fclose(fp);
}

void	ClearMessage()
{
	CGCStateEstimateServerDlg*	pFrame=(CGCStateEstimateServerDlg*)theApp.GetMainWnd();
	pFrame->ClearMessage();
}

void	PrintMessage(const char* lpszFormat, ...)
{
	va_list args;
	va_start( args, lpszFormat );

	char	szMesg[1024];
	vsprintf(szMesg, lpszFormat, args);

	CGCStateEstimateServerDlg* pDlg=(CGCStateEstimateServerDlg*)AfxGetMainWnd();
	if (pDlg)
		pDlg->PrintMessage(szMesg);

	va_end(args);
}
