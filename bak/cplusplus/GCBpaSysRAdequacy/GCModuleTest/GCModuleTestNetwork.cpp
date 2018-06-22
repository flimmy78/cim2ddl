#include "stdafx.h"
#include "GCModuleTest.h"
#include "GCModuleTestDlg.h"

CNetworkInterface	g_NetInterface;
SOCKET	m_WatchSocket=INVALID_SOCKET;

void SendMessageToWindow(CWnd* pMesgWnd, char* pformat, ...)
{
	va_list args;
	va_start( args, pformat );

	if (pMesgWnd)
	{
		char	szMesg[1024];
		vsprintf(szMesg, pformat, args);

		int		nMesgLen = strlen(szMesg)+1;
		char*	lpszMesg = (char*)GlobalAlloc(GPTR, nMesgLen);
		if (lpszMesg)
		{
			memset(lpszMesg, 0, nMesgLen);
			strcpy(lpszMesg, szMesg);
			pMesgWnd->PostMessage(UM_MESSAGE, 0, (LPARAM)lpszMesg);
		}
	}

	va_end(args);
}

void	NetworkDataProc(void* pParam)
{
	tagThreadArg	tArg;
	memcpy(&tArg, (char*)pParam, sizeof(tagThreadArg));
	GlobalFree((void*)pParam);

	CWnd*	pMainWnd = tArg.pMainWnd;
#ifdef _DEBUG
	SendMessageToWindow(pMainWnd, "**********����෵����Ϣ**********");
#endif

	SOCKET	sComm=tArg.sSocket;
	if (sComm == INVALID_SOCKET)
	{
		Log(g_lpszLogFile, "OnConnect INVALID_SOCKET\n");
		return;
	}

	std::string		strRecv;
	unsigned char	bNetRecvSuccess = g_NetInterface.ReadTCPSocket(sComm, strRecv);

	shutdown(sComm, SD_BOTH);
	closesocket(sComm);

	std::string		strCommand, strValue, strMesg;
	Json::Reader	sReader;
	Json::Value		sRoot, sContent, sData, sNull(nullValue);

	if (!sReader.parse(strRecv, sRoot, false))
		return;

	strCommand.clear();
	strValue.clear();
	strMesg.clear();

#ifdef _DEBUG
	Log(g_lpszLogFile, "ReturnJson=\n");
	Log(g_lpszLogFile, "%s",sRoot.toStyledString().c_str());
	Log(g_lpszLogFile, "**********************************************************************\n\n");
#endif

	strCommand = sRoot["Command"].asString();
#ifdef _DEBUG
	SendMessageToWindow(pMainWnd, "        ReturnCommand = %s", strCommand.c_str());
	Log(g_lpszLogFile, "        ReturnCommand = %s", strCommand.c_str());
#endif

	sContent = sRoot.get("Content", sNull);
	if (!sContent.isNull())
	{
		sData = sContent.get("Message", sNull);
		if (!sData.isNull())
		{
			strMesg = sData.asString();
			SendMessageToWindow(pMainWnd, "        ReturnMessage = %s", strMesg.c_str());
#ifdef _DEBUG
			Log(g_lpszLogFile, "ReturnMessage = %s\n", strMesg.c_str());
#endif
		}
	}

	if (stricmp(strCommand.c_str(), Command_DataReady) == 0)
	{
		sData = sContent.get("Value", sNull);
		if (!sData.isNull())
		{
			strValue = sData.asString();
			SendMessageToWindow(pMainWnd, "        ReturnCommandValue = %s", strValue.c_str());
#ifdef _DEBUG
			Log(g_lpszLogFile, "ReturnCommandValue = %s\n", strValue.c_str());
#endif
			if (atoi(strValue.c_str()) > 0)
				PRStateEstimateCalculateInit(g_pPRBlock);
		}
		else
		{
#ifdef _DEBUG
			SendMessageToWindow(pMainWnd, "        Content����Value");
			Log(g_lpszLogFile, "Content����Value\n");
#endif
			return;
		}
	}
	else if (stricmp(strCommand.c_str(), Command_JobFinished) == 0)
	{
		PRStateEstimateCalculateExit();
	}
	else if (stricmp(strCommand.c_str(), Command_StateEstimate) == 0)
	{
		PRStateEstimateReturn(pMainWnd, sContent);
	}
}

void	TCPWatchThread(void* pParam)
{
	tagThreadArg*	pArg=(tagThreadArg*)pParam;
	SOCKET			sWatch = pArg->sSocket;
	CWinThread*		pDataProcThread = pArg->pDataProcThread;
	CWnd*			pMainWnd = pArg->pMainWnd;
	vFun_v			pNetProcFunc = pArg->pNetProcFunc;

	GlobalFree(pParam);

	fd_set	read_fd;
	listen(sWatch, SOMAXCONN);
	while (1)
	{
		FD_ZERO(&read_fd);
		FD_SET(sWatch, &read_fd);
		if (select(sWatch+1, &read_fd, (fd_set *)0, (fd_set *)0, (struct timeval *)0) == -1)
			break;

		if (FD_ISSET(sWatch, &read_fd))
		{
			struct 	sockaddr_in	sAddress;
			socklen_t	nAddrLen=sizeof(sAddress);
			SOCKET		sConnect=accept(sWatch, (struct sockaddr *)&sAddress, (socklen_t*)&nAddrLen);
			if (sConnect != -1)
			{
				tagThreadArg*	pArg=(tagThreadArg*)GlobalAlloc(GPTR, sizeof(tagThreadArg));	//	malloc(sizeof(tagThreadArg));
				if (pArg)
				{
					memset(pArg, 0, sizeof(tagThreadArg));
					pArg->sSocket=sConnect;
					strcpy(pArg->szPeerAddr, inet_ntoa(sAddress.sin_addr));
					pArg->nPeerPort = sAddress.sin_port;
					pArg->pDataProcThread = pDataProcThread;
					pArg->pMainWnd = pMainWnd;

#if (!(defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(_WIN64) || defined(__WIN64__) || defined(WIN64)))
					int			nStatus;
					pthread_t	tid;
					int	nStatus = pthread_create(&tid, 
						NULL, 
						pNetProcFunc, 
						(void*)pArg);
					if (nStatus != 0)
					{
						switch (errno)
						{
						case	EAGAIN:
							Log("If WLM is running, the limit on the number of threads in the class may have been met. PTHREAD_THREADS_MAX.");
							break;
						case	EINVAL:
							Log("The value specified by attr is invalid.");
							break;
						case	EPERM:
							Log("The caller does not have appropriate permission to set the required scheduling parameters or scheduling policy.");
							break;
						default:
							Log("Unknow");
							break;
						}
						Log("pthread_create");
						continue;
					}
#else
#	ifdef _DEBUG
					SendMessageToWindow(pMainWnd, "%s ��������", pArg->szPeerAddr);
#	endif

					_beginthread(pNetProcFunc, 0, (void*)pArg);
#endif
				}
			}
		}
	}

	SendMessageToWindow(pMainWnd, "    �˳���������߳�");
	_endthread();
}

void StartNetworkService(const int nSvrPort)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_WatchSocket == INVALID_SOCKET)
	{
		PrintMessage("����������� ����˿ں� = %d", nSvrPort);

		m_WatchSocket=g_NetInterface.InitTCPServer(nSvrPort);
		if (m_WatchSocket < 0)
		{
			PrintMessage("�����������ʧ�ܣ�InitTCPServer");
			return;
		}

		tagThreadArg*	pArg=(tagThreadArg*)GlobalAlloc(GPTR, sizeof(tagThreadArg));
		if (pArg)
		{
			memset(pArg, 0, sizeof(tagThreadArg));
			pArg->sSocket = m_WatchSocket;
			pArg->pMainWnd = AfxGetMainWnd();
			pArg->pDataProcThread = &theApp;
			pArg->pNetProcFunc = NetworkDataProc;
			_beginthread(TCPWatchThread, 0, (void*)pArg);
		}
		else
		{
			PrintMessage("�����������ʧ�ܣ������ڴ�ʧ��");
		}
	}
	else
	{
		PrintMessage("��������Ѿ�����");
	}
}

int SendNetworkData(const char* lpszAddr, const int nPort, const char* lpszData)
{
	int				nSend=0;
	int				nRet=0;

	SOCKET	sComm=g_NetInterface.ConnectTCPServer(lpszAddr, nPort);
	if (sComm == INVALID_SOCKET)
	{
		PrintMessage("SendNetworkData �������Ӵ���, ��ַ = %s �� �˿� = %d", lpszAddr, nPort);
		Log(g_lpszLogFile, "�������Ӵ���, ��ַ = %s �� �˿� = %d\n", lpszAddr, nPort);
		return 0;
	}

	const int	nSndLen = strlen(lpszData);
	nSend=g_NetInterface.SendTCPSocket(sComm, nSndLen, lpszData);
	if (nSend != nSndLen)
	{
		PrintMessage("SendNetworkData �����������ݴ���");
		Log(g_lpszLogFile, "�����������ݴ���\n");
		return 0;
	}

	//::SendMessageToWindow(pMainWnd, "**********SendNetworkData ���������������[%d]**********", nSend);
	shutdown(sComm, SD_BOTH);
	closesocket(sComm);

	return nSend;
}
