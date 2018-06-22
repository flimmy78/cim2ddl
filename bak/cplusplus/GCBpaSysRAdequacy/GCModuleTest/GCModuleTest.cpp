
// GCModuleTest.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "GCModuleTest.h"
#include "GCModuleTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGCModuleTestApp

BEGIN_MESSAGE_MAP(CGCModuleTestApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CGCModuleTestApp ����

CGCModuleTestApp::CGCModuleTestApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CGCModuleTestApp ����

CGCModuleTestApp		theApp;
tagBpaBlock*			g_pBpaBlock;
tagPRBlock*				g_pPRBlock;
CPRMemDBInterface		g_PRMemDBInterface;
tagBpaPRAdequacySetting	g_PRAdeSetting;

char					g_szRemoteAddr[48];
unsigned int			g_nRemotePort;
unsigned int			g_nLocalPort;
int						g_nThreadNum;
char					g_szRunDir[260];

// CGCModuleTestApp ��ʼ��
const	char*	g_lpszLogFile="GCModuleTest.log";
const	char*	g_lpszIniFile="GCModuleTest.xml";

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

BOOL CGCModuleTestApp::InitInstance()
{
	if (!CreateOneAppMutex("ZhongND_GCModuleTest"))
	{
		AfxMessageBox("��������Ѿ�����");
		return FALSE;
	}
	HANDLE hEstStateSem = InitSem(g_lpszGCPRStateEstimateSemaphore);

	GetCurrentDirectory(260, g_szRunDir);
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
		AfxMessageBox("�����ʼ������");
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

	ClearLog(g_lpszLogFile);

	strcpy(g_szRemoteAddr, "127.0.0.1");
	g_nRemotePort = 8950;
	g_nLocalPort = 9002;
	g_nThreadNum = 2;

	g_pBpaBlock=(tagBpaBlock*)Init_BpaBlock();
	if (!g_pBpaBlock)
	{
		AfxMessageBox("��ȡBpa�ڴ�����");
		return FALSE;
	}

	g_pPRBlock=(tagPRBlock*)g_PRMemDBInterface.Init_PRBlock();
	if (!g_pPRBlock)
	{
		AfxMessageBox("��ȡPR�ڴ�����");
		return FALSE;
	}

	InitBpaPRAdequacySetting(&g_PRAdeSetting);
	char	szFileName[260];
	sprintf(szFileName, "%s/Ini", g_szRunDir);
	if (access(szFileName, 0) == 0)
	{
		sprintf(szFileName, "%s/Ini/%s", g_szRunDir, g_lpszIniFile);
		if (access(szFileName, 0) != 0)
			sprintf(szFileName, "%s/%s", g_szRunDir, g_lpszIniFile);
	}
	else
		sprintf(szFileName, "%s/%s", g_szRunDir, g_lpszIniFile);
	ReadBpaPRAdequacySetting(szFileName, &g_PRAdeSetting);

	CGCModuleTestDlg dlg;
	m_pMainWnd = &dlg;
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

	if (hEstStateSem != INVALID_HANDLE_VALUE)
		CloseHandle(hEstStateSem);

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

void	PrintMessage(const char* lpszFormat, ...)
{
	va_list args;
	va_start( args, lpszFormat );

	char	szMesg[1024];
	vsprintf(szMesg, lpszFormat, args);

	CGCModuleTestDlg* pDlg=(CGCModuleTestDlg*)AfxGetMainWnd();
	if (pDlg)
		pDlg->PrintMessage(szMesg);

	va_end(args);
}
