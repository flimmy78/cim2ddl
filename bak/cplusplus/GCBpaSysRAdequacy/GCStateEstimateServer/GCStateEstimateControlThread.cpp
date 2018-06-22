// GCStateEstimateControlThread.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GCStateEstimateServer.h"
#include "GCStateEstimateControlThread.h"

// CGCStateEstimateControlThread

IMPLEMENT_DYNCREATE(CGCStateEstimateControlThread, CWinThread)

CGCStateEstimateControlThread::CGCStateEstimateControlThread()
{
	m_bAutoDelete		= FALSE;

	m_pMainWnd = NULL;
	m_pParentThread = NULL;
	m_pDataProcThreadArray.clear();
	strcpy(m_szRemoteAddr, "127.0.0.1");
	m_nRemotePort = 9002;
}

CGCStateEstimateControlThread::~CGCStateEstimateControlThread()
{
	if (!m_pDataProcThreadArray.empty())
	{
		for (int nThread=0; nThread<m_pDataProcThreadArray.size(); nThread++)
		{
			if (m_pDataProcThreadArray[nThread])
			{
				m_pDataProcThreadArray[nThread]->Close();
				delete m_pDataProcThreadArray[nThread];
			}
		}

		m_pDataProcThreadArray.clear();
	}
}

BOOL CGCStateEstimateControlThread::InitInstance()
{
	// TODO: �ڴ�ִ���������̳߳�ʼ��
	return TRUE;
}

int CGCStateEstimateControlThread::ExitInstance()
{
	// TODO: �ڴ�ִ���������߳�����
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CGCStateEstimateControlThread, CWinThread)
	ON_THREAD_MESSAGE(TM_CONNECT,		OnConnect)
	ON_THREAD_MESSAGE(TM_QUIT,			OnQuit)
END_MESSAGE_MAP()


// CGCStateEstimateControlThread ��Ϣ�������
void CGCStateEstimateControlThread::OnConnect(WPARAM wParam, LPARAM lParam)
{
#ifdef _DEBUG
	SendMessageToWindow("    ��ʼ���ݴ����߳�");
#endif

	unsigned char	bNetRecvSuccess;
	clock_t			dBeg, dEnd;
	int				nDur;

	tagThreadArg	tArg;
	memcpy(&tArg, (char*)lParam, sizeof(tagThreadArg));
	GlobalFree((void*)lParam);

	SOCKET	sComm=tArg.sSocket;
	if (sComm == INVALID_SOCKET)
	{
		Log(g_lpszLogFile, "OnConnect INVALID_SOCKET\n");
		return;
	}
	dBeg=clock();

	std::string	strRecv;
	bNetRecvSuccess = g_NetInterface.ReadTCPSocket(sComm, strRecv);

	shutdown(sComm, SD_BOTH);
	closesocket(sComm);

	dEnd=clock();
	nDur=(int)((1000.0*(double)(dEnd-dBeg))/CLOCKS_PER_SEC);

#ifdef _DEBUG
	SendMessageToWindow("        �������ݽ������(%d)������ %d �ֽڣ���ʱ %d ����", bNetRecvSuccess, strRecv.size(), nDur);

// 	Log(g_lpszLogFile, "�������ݽ�����ɣ����� %d �ֽڣ���ʱ %d ����", strRecv.size(), nDur);
// 	Log(g_lpszLogFile, "\n********************************************************************************\n");
// 	Log(g_lpszLogFile, "%s\n", strRecv.c_str());
// 	Log(g_lpszLogFile, "********************************************************************************\n\n\n");
#endif

	if (bNetRecvSuccess)
		NetworkDataProcess(strRecv);
	else
		SendMessageToWindow("        �������ݽ��մ���(%d)������ %d �ֽڣ���ʱ %d ����", bNetRecvSuccess, strRecv.size(), nDur);
}

void CGCStateEstimateControlThread::OnQuit(WPARAM wParam, LPARAM lParam)
{
	Log(g_lpszLogFile, "CGCStateEstimateControlThread OnQuit %d\n", m_pDataProcThreadArray.size());
	if (!m_pDataProcThreadArray.empty())
	{
		for (int nThread=0; nThread<m_pDataProcThreadArray.size(); nThread++)
		{
			if (m_pDataProcThreadArray[nThread])
			{
				m_pDataProcThreadArray[nThread]->Close();
				delete m_pDataProcThreadArray[nThread];
			}
		}
		m_pDataProcThreadArray.clear();
	}

	AfxEndThread(0);

	return;

	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
}

//////////////////////////////////////////////////////////////////////////
//	���ⲿ����
int	CGCStateEstimateControlThread::Create(CWnd* pMainWnd, CWinThread* pParent, const char* lpszRemoteAddr, const int nRemotePort)
{
	m_pMainWnd			= pMainWnd;
	m_pParentThread		= pParent;
	strcpy(m_szRemoteAddr, lpszRemoteAddr);
	m_nRemotePort = nRemotePort;
	m_pMainWnd = pMainWnd;

	if (!CreateThread())
		return 0;

	return 1;
}

void CGCStateEstimateControlThread::Close()
{
	if (m_hThread)
	{
		DWORD	dwExitCode;
		BOOL	bGet = GetExitCodeThread(m_hThread,&dwExitCode);
		if (bGet && dwExitCode == STILL_ACTIVE)
		{
			PostThreadMessage(TM_QUIT,0,0);
			WaitForSingleObject(m_hThread,1000);
		}
	}
}

int CGCStateEstimateControlThread::NetworkDataProcess(std::string strRecv)
{
	Json::Reader	sReader;
	Json::Value		sRoot, sContent, sData, sNull(nullValue);

	std::string		strCommand, strValue;

	strCommand.clear();
	strValue.clear();
	if (!sReader.parse(strRecv, sRoot, false))
	{
		strValue = "�����߳��� Parse Json ����, ����������� = ";
		strValue.append(strRecv);

#ifdef _DEBUG
		SendMessageToWindow("        �����߳��� Parse Json ����");
		Log(g_lpszLogFile, "        �����߳��� Parse Json ����\n");
#endif
		SendNetworkData(m_szRemoteAddr, m_nRemotePort, FormResultJSon("Error", "0", strValue.c_str()).c_str());
		return FALSE;
	}

	strCommand = sRoot["Command"].asString();
#ifdef _DEBUG
	SendMessageToWindow("����Command = %s", strCommand.c_str());
	Log(g_lpszLogFile, "Command = %s\n", strCommand.c_str());
#endif

	if (stricmp(strCommand.c_str(), Command_DataReady) == 0)
	{
		if (!m_pDataProcThreadArray.empty())
		{
#ifdef _DEBUG
			SendMessageToWindow("        ��ǰ��������û�н���������JobFinished");
			Log(g_lpszLogFile, "        ��ǰ��������û�н���������JobFinished\n");
#endif
			SendNetworkData(m_szRemoteAddr, m_nRemotePort, FormResultJSon(strCommand.c_str(), "0", "�����߳��� DataReady���� �������������").c_str());
			return FALSE;
		}

		tagBpaPRAdequacySetting	sPRAdequacySetting;
		PRAdequacyBase::InitBpaPRAdequacySetting(&sPRAdequacySetting);

		sContent = sRoot.get("Content", sNull);
		if (!sContent.isNull())
		{
			sData = sContent.get("PRAdequacySetting", sNull);
			if (!sData.isNull())
			{
				ParsePRSetting(sData, &sPRAdequacySetting);
			}
			else
			{
#ifdef _DEBUG
				SendMessageToWindow("        Content����PRAdequacySetting");
				Log(g_lpszLogFile, "Content����PRAdequacySetting\n");
#endif
				SendNetworkData(m_szRemoteAddr, m_nRemotePort, FormResultJSon(strCommand.c_str(), "0", "�����߳��� DataReady���� Content ���� PRAdequacySetting").c_str());
				return FALSE;
			}
		}
		SendMessageToWindow("        �����̳߳�ʼ��");
		DataReady(&sPRAdequacySetting);

		char	szMesg[260];
		sprintf(szMesg, "%d", m_pDataProcThreadArray.size());
		SendNetworkData(m_szRemoteAddr, m_nRemotePort, FormResultJSon(strCommand.c_str(), szMesg, "�����̳߳�ʼ�����").c_str());
	}
	else if (stricmp(strCommand.c_str(), Command_JobFinished) == 0)
	{
		if (m_pDataProcThreadArray.empty())
		{
#ifdef _DEBUG
			SendMessageToWindow("        ��ǰ�޼�������");
			Log(g_lpszLogFile, "        ��ǰ�޼�������\n");
#endif
			SendNetworkData(m_szRemoteAddr, m_nRemotePort, FormResultJSon(strCommand.c_str(), "0", "�����߳��� JobFinished���� ��ǰ�޼�������").c_str());
			return FALSE;
		}

		SendMessageToWindow("        �رռ�������");
		FinishJob();
		SendNetworkData(m_szRemoteAddr, m_nRemotePort, FormResultJSon(strCommand.c_str(), "1", "��������ر����").c_str());
	}
	else if (stricmp(strCommand.c_str(), Command_StateEstimate) == 0)
	{
		int	nIdleThread = -1;
		for (int nThread=0; nThread<m_pDataProcThreadArray.size(); nThread++)
		{
			if (m_pDataProcThreadArray[nThread]->m_bIdle)
			{
				nIdleThread = nThread;
				break;
			}
		}
		if (nIdleThread >= 0)
		{
			register int	i;
			char*	lpszData = (char*)GlobalAlloc(GPTR, strRecv.size());
			memset(lpszData, 0, strRecv.size());

			for (i=0; i<strRecv.size(); i++)
				lpszData[i] = strRecv[i];

			m_pDataProcThreadArray[nIdleThread]->m_bIdle = 0;
			m_pDataProcThreadArray[nIdleThread]->PostThreadMessage(TM_STATE_ESTIMATE, 0, (LPARAM)lpszData);
		}
		else
		{
			SendNetworkData(m_szRemoteAddr, m_nRemotePort, FormResultJSon(strCommand.c_str(), "0", "�����߳��� �޿��м����߳�").c_str());
		}
	}

	return TRUE;
}

void CGCStateEstimateControlThread::SendMessageToWindow(char* pformat, ...)
{
	va_list args;
	va_start( args, pformat );

	if (m_pMainWnd)
	{
		char	szMesg[1024];
		vsprintf(szMesg, pformat, args);

		int		nMesgLen = strlen(szMesg)+1;
		char*	lpszMesg = (char*)malloc(nMesgLen);
		if (lpszMesg)
		{
			memset(lpszMesg, 0, nMesgLen);
			strcpy(lpszMesg, szMesg);
			m_pMainWnd->PostMessage(UM_MESSAGE, 0, (LPARAM)lpszMesg);
		}
	}

	va_end(args);
}

void CGCStateEstimateControlThread::ParsePRSetting(Json::Value& sData, tagBpaPRAdequacySetting* pSetting)
{
	Json::Value	sObject, sNull(nullValue);

	sObject = sData.get("BpaDatFile", sNull);		if (!sObject.isNull())	pSetting->strBpaDatFile		= sObject.asString();				//���������ļ�
	sObject = sData.get("BpaSwiFile", sNull);		if (!sObject.isNull())	pSetting->strBpaSwiFile		= sObject.asString();				//�ȶ������ļ�����Ҫ�������γɷ����ģ��
	sObject = sData.get("BpaRParamFile", sNull);	if (!sObject.isNull())	pSetting->strBpaRParamFile	= sObject.asString();				//�ɿ��Բ��������ļ�
	sObject = sData.get("TinyGenThreshold", sNull);	if (!sObject.isNull())	pSetting->fTinyGenThreshold	= sObject.asDouble();	//�������ֽⷢ��������ż�ֵ���������ڸ��ż�ֵ�ķ������Ϊ���Ƿ������
	sObject = sData.get("LowVoltThreshold", sNull);	if (!sObject.isNull())	pSetting->fLowVoltThreshold	= sObject.asDouble();	//�������ֽⷢ����͵�ѹ�ż�ֵ����ѹ���ڸ��ż�ֵ�ķ������Ϊ�Ǹ��ɶ����Ƿ��磩
	sObject = sData.get("ZIL", sNull);				if (!sObject.isNull())	pSetting->fZIL				= sObject.asDouble();	//С�迹���С�迹�ż�ֵ
	sObject = sData.get("PRSampleObject", sNull);	if (!sObject.isNull())	pSetting->nPRSampleObject	= sObject.asInt();	//��������ȫ�������硢֧·��
	sObject = sData.get("PRSampleMethod", sNull);	if (!sObject.isNull())	pSetting->nPRSampleMethod	= sObject.asInt();	//��������
	sObject = sData.get("MaxGenFault", sNull);		if (!sObject.isNull())	pSetting->nMaxGenFault		= sObject.asInt();	//MCS, FST, STS, ANA ������󷢵����������
	sObject = sData.get("MaxBranFault", sNull);		if (!sObject.isNull())	pSetting->nMaxBranFault		= sObject.asInt();	//MCS, FST, STS, ANA �������֧·��������
	sObject = sData.get("LineELimit", sNull);		if (!sObject.isNull())	pSetting->bLineELimit		= sObject.asInt();	//��·���޿���
	sObject = sData.get("TranELimit", sNull);		if (!sObject.isNull())	pSetting->bTranELimit		= sObject.asInt();	//�������޿���
	sObject = sData.get("GenPELimit", sNull);		if (!sObject.isNull())	pSetting->bGenPELimit		= sObject.asInt();	//�������������
	sObject = sData.get("UPFCELimit", sNull);		if (!sObject.isNull())	pSetting->bUPFCELimit		= sObject.asInt();	//UPFC��������
	sObject = sData.get("AuxLoadAdjust", sNull);	if (!sObject.isNull())	pSetting->bAuxLoadAdjust	= sObject.asInt();	//���õ縺�ɲ������
	sObject = sData.get("EQGenAdjust", sNull);		if (!sObject.isNull())	pSetting->bEQGenAdjust		= sObject.asInt();	//��ֵ������������
	sObject = sData.get("EQLoadAdjust", sNull);		if (!sObject.isNull())	pSetting->bEQLoadAdjust		= sObject.asInt();	//��ֵ������������
	sObject = sData.get("MCSSimulateTime", sNull);	if (!sObject.isNull())	pSetting->nMCSSimulateTime	= sObject.asInt();	//MCS����������ʱ��
	sObject = sData.get("MCSMinStateProb", sNull);	if (!sObject.isNull())	pSetting->fMCSMinStateProb	= sObject.asInt();	//MCS.get(���ؿ���]�豸���ϸ����ż�ֵ
	sObject = sData.get("FSTMaxCumuProb", sNull);	if (!sObject.isNull())	pSetting->fFSTMaxCumuProb	= sObject.asDouble();	//FST.get(��������]�ۻ�����
	sObject = sData.get("FSTMinStateProb", sNull);	if (!sObject.isNull())	pSetting->fFSTMinStateProb	= sObject.asDouble();	//FST.get(��������]�豸���ϸ����ż�ֵ
	sObject = sData.get("FSTMaxStateNum", sNull);	if (!sObject.isNull())	pSetting->nFSTMaxStateNum	= sObject.asInt();	//FST.get(��������]���״̬��
	sObject = sData.get("STSMaxStateNum", sNull);	if (!sObject.isNull())	pSetting->nSTSMaxStateNum	= sObject.asInt();	//STS.get(״̬����]���״̬��
	sObject = sData.get("ANAMinStateProb", sNull);	if (!sObject.isNull())	pSetting->fANAMinStateProb	= sObject.asDouble();	//ANA.get( ������]�豸���ϸ����ż�ֵ
	sObject = sData.get("Dc2AcFactor", sNull);		if (!sObject.isNull())	pSetting->fDc2AcFactor		= sObject.asDouble();	//ֱ������2 ��������ϵ��
	sObject = sData.get("MinIslandGLRatio", sNull);	if (!sObject.isNull())	pSetting->fMinIslandGLRatio	= sObject.asDouble();	//�µ�����С���ر�
	sObject = sData.get("MultiThread", sNull);		if (!sObject.isNull())	pSetting->nMultiThread		= sObject.asInt();	//�߳���
	sObject = sData.get("UPFCAdjustRC", sNull);		if (!sObject.isNull())	pSetting->bUPFCAdjustRC		= sObject.asInt();	//UPFC���õ���������
	sObject = sData.get("GenBusLoadAsAux", sNull);	if (!sObject.isNull())	pSetting->bGenBusLoadAsAux	= sObject.asInt();	//�����ĸ�߸��ɰ����õ紦��
}

int CGCStateEstimateControlThread::DataReady(tagBpaPRAdequacySetting* pSetting)
{
	CGCStateEstimateCalculateThread* pThread;
	SYSTEM_INFO sysInfo;
	::GetSystemInfo(&sysInfo);

	int		nThreadNum = pSetting->nMultiThread;
	if (nThreadNum <= 0)
		nThreadNum = 1;
	if (nThreadNum > sysInfo.dwNumberOfProcessors)
		nThreadNum = sysInfo.dwNumberOfProcessors;

	for (int nThread=0; nThread<nThreadNum; nThread++)
	{
		pThread = new CGCStateEstimateCalculateThread();
		if (pThread)
		{
			if (!pThread->Create(m_pMainWnd, this, nThread+1, pSetting, m_szRemoteAddr, m_nRemotePort))
			{
				SendMessageToWindow("    ������������߳�[%d]ʧ��!", nThread+1);
				Log(g_lpszLogFile, "������������߳�[%d]ʧ��!\n", nThread+1);
				continue;
			}
			SendMessageToWindow("    ������������߳�[%d]���!\n", nThread+1);
			Log(g_lpszLogFile, "������������߳�[%d]���!\n", nThread+1);
			m_pDataProcThreadArray.push_back(pThread);
		}
	}
	return 1;
}

int CGCStateEstimateControlThread::FinishJob()
{
	for (int nThread=0; nThread<m_pDataProcThreadArray.size(); nThread++)
	{
		CGCStateEstimateCalculateThread* pThread=(CGCStateEstimateCalculateThread*)m_pDataProcThreadArray[nThread];
		pThread->Close();
		delete pThread;
	}

	m_pDataProcThreadArray.clear();

	return 1;
}

std::string	CGCStateEstimateControlThread::FormResultJSon(const char* lpszCommand, const char* lpszRetValue, const char* lpszMesg)
{
	Json::Value sRoot;
	Json::Value sContent;
	Json::FastWriter writer;

	//sprintf(szValue, "%d", nRetValue);
	sRoot["Command"] = lpszCommand;
	sContent["Value"] = lpszRetValue;
	sContent["Message"] = lpszMesg;
	sRoot["Content"] = sContent;

	return writer.write(sRoot);
}