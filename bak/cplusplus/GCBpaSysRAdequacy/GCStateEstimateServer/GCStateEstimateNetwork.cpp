#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include "GCStateEstimateNetwork.h"

#include <vector>
using namespace std;

extern	void	PrintMessage(const char* lpszFormat, ...);
extern	void	Log(const char* lpszLogFile, char* pformat, ...);
extern	const	char*	g_lpszLogFile;


SOCKET	m_WatchSocket=INVALID_SOCKET;

void	TCPWatchThread(void* pParam)
{
	tagThreadArg*	pArg=(tagThreadArg*)pParam;
	SOCKET			sWatch = pArg->sSocket;
	CWnd*			pMainWnd = pArg->pMainWnd;
	CWinThread*		pDataProcThread = pArg->pDataProcThread;

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
					pArg->pMainWnd = pMainWnd;
					pArg->pDataProcThread = pDataProcThread;

#if (!(defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(_WIN64) || defined(__WIN64__) || defined(WIN64)))
					int			nStatus;
					pthread_t	tid;
					int	nStatus = pthread_create(&tid, 
						NULL, 
						pDataProcFunc, 
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
#ifdef _DEBUG
					if (pMainWnd)
					{
						char*	lpszMesg = (char*)malloc(64);
						if (lpszMesg)
						{
							memset(lpszMesg, 0, 64);
							sprintf(lpszMesg, "%s ��������", pArg->szPeerAddr);
							pMainWnd->PostMessage(UM_MESSAGE, 0, (LPARAM)lpszMesg);
						}
					}
					Log(g_lpszLogFile, "%s ��������\n", pArg->szPeerAddr);
#endif
					pDataProcThread->PostThreadMessage(TM_CONNECT, 0, (LPARAM)pArg);
#endif
				}
			}
		}
	}

	if (pMainWnd)
	{
		char*	lpszMesg = (char*)malloc(64);
		if (lpszMesg)
		{
			memset(lpszMesg, 0, 64);
			strcpy(lpszMesg, "    �˳���������߳�");
			pMainWnd->PostMessage(UM_MESSAGE, 0, (LPARAM)lpszMesg);
		}
	}

	_endthread();
}

void StartNetworkService(CWinThread* pDataProcThread, const int nSvrPort)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_WatchSocket == INVALID_SOCKET)
	{
		PrintMessage("����������� ����˿ں� = %d", nSvrPort);

		m_WatchSocket=g_NetInterface.InitTCPServer(nSvrPort);
		if (m_WatchSocket < 0)
		{
			PrintMessage("    ��ʼ���������ʧ��");
			return;
		}
		PrintMessage("    ��ʼ������������");

		tagThreadArg*	pArg=(tagThreadArg*)GlobalAlloc(GPTR, sizeof(tagThreadArg));
		if (pArg)
		{
			memset(pArg, 0, sizeof(tagThreadArg));
			pArg->sSocket = m_WatchSocket;
			pArg->pMainWnd = AfxGetMainWnd();
			pArg->pDataProcThread = pDataProcThread;
			PrintMessage("    ���������������......");
			_beginthread(TCPWatchThread, 0, (void*)pArg);
		}
	}
	else
	{
		PrintMessage("��������Ѿ�����");
	}
}

void StopNetworkService()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_WatchSocket != INVALID_SOCKET)
	{
		PrintMessage("ֹͣ�������");
		shutdown(m_WatchSocket, SD_BOTH);
		closesocket(m_WatchSocket);
		m_WatchSocket=INVALID_SOCKET;
	}
	else
	{
		PrintMessage("��ǰû���������");
	}
}

int SendNetworkData(const char* lpszAddr, const int nPort, const char* lpszData)
{
	int				nSend=0;
	int				nRet=0;

	SOCKET	sComm=g_NetInterface.ConnectTCPServer(lpszAddr, nPort);
	if (sComm == INVALID_SOCKET)
	{
		PrintMessage("SendNetworkData �������Ӵ���");
		Log(g_lpszLogFile, "�������Ӵ���\n");
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

	//PrintMessage("**********SendNetworkData ���������������[%d]**********", nSend);
	shutdown(sComm, SD_BOTH);
	closesocket(sComm);

	return nSend;
}