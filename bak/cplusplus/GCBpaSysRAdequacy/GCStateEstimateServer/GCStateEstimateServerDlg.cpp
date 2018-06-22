
// GCStateEstimateServerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GCStateEstimateServer.h"
#include "GCStateEstimateServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGCStateEstimateServerDlg �Ի���
unsigned char	m_bFreezeUI = 0;

CGCStateEstimateServerDlg::CGCStateEstimateServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGCStateEstimateServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}

void CGCStateEstimateServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGCStateEstimateServerDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_EN_CHANGE(IDC_LOCAL_PORT, &CGCStateEstimateServerDlg::OnEnChangeLocalPort)
	ON_EN_CHANGE(IDC_REMOTE_PORT, &CGCStateEstimateServerDlg::OnEnChangeRemotePort)
	ON_EN_CHANGE(IDC_REMOTE_ADDR, &CGCStateEstimateServerDlg::OnEnChangeRemoteAddr)
	ON_BN_CLICKED(IDC_CLEAR_MESG, &CGCStateEstimateServerDlg::OnBnClickedClearMesg)
	ON_BN_CLICKED(IDC_START_NETSVR, &CGCStateEstimateServerDlg::OnBnClickedStartNetSvr)
	ON_BN_CLICKED(IDC_STOP_NETSVR, &CGCStateEstimateServerDlg::OnBnClickedStopNetSvr)
	ON_BN_CLICKED(IDC_JSON_WRITE, &CGCStateEstimateServerDlg::OnBnClickedJsonWrite)
	ON_BN_CLICKED(IDC_COMM, &CGCStateEstimateServerDlg::OnBnClickedComm)
	ON_BN_CLICKED(IDC_JSON_READ, &CGCStateEstimateServerDlg::OnBnClickedJsonRead)

	ON_MESSAGE(UM_MESSAGE,			OnShowMessage)
END_MESSAGE_MAP()


// CGCStateEstimateServerDlg ��Ϣ�������
BOOL CGCStateEstimateServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	int		nColumn=0;
	char	szBuffer[260];

	m_bFreezeUI = 1;
	sprintf(szBuffer, "%d", g_nLocalPort);
	GetDlgItem(IDC_LOCAL_PORT)->SetWindowText(szBuffer);
	sprintf(szBuffer, "%d", g_nRemotePort);
	GetDlgItem(IDC_REMOTE_PORT)->SetWindowText(szBuffer);
	GetDlgItem(IDC_REMOTE_ADDR)->SetWindowText(g_szRemoteAddr);

	CListCtrl*	pListCtrl=(CListCtrl*)GetDlgItem(IDC_CLIENT_LIST);
	pListCtrl->ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
	pListCtrl->SetExtendedStyle(pListCtrl->GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	pListCtrl->SetFont(CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT)));

	nColumn=0;
	pListCtrl->InsertColumn(nColumn, "���");			pListCtrl->SetColumnWidth(nColumn, 50);		nColumn++;
	pListCtrl->InsertColumn(nColumn, "��������");		pListCtrl->SetColumnWidth(nColumn, 200);	nColumn++;
	pListCtrl->InsertColumn(nColumn, "�����ֽ�");		pListCtrl->SetColumnWidth(nColumn, 100);	nColumn++;
	pListCtrl->InsertColumn(nColumn, "�����ֽ�");		pListCtrl->SetColumnWidth(nColumn, 100);	nColumn++;

	StartNetworkService(&g_StateEstimateControlThread, g_nLocalPort);

	m_bFreezeUI = 0;

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CGCStateEstimateServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CGCStateEstimateServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

int CGCStateEstimateServerDlg::GetTextLen(LPCTSTR lpszText)
{
	ASSERT(AfxIsValidString(lpszText));

	CDC *pDC = GetDC();
	ASSERT(pDC);

	CSize size;
	CFont* pOldFont = pDC->SelectObject(GetFont());
	if ((GetStyle() & LBS_USETABSTOPS) == 0)
	{
		size = pDC->GetTextExtent(lpszText, (int) _tcslen(lpszText));
		size.cx += 3;
	}
	else
	{
		// Expand tabs as well
		size = pDC->GetTabbedTextExtent(lpszText, (int) _tcslen(lpszText), 0, NULL);
		size.cx += 2;
	}
	pDC->SelectObject(pOldFont);
	ReleaseDC(pDC);

	return size.cx;
}

LRESULT CGCStateEstimateServerDlg::OnShowMessage(WPARAM wParam, LPARAM lParam)
{
	char*	pChar = (char*)lParam;
	PrintMessage(pChar);
	free(pChar);

	return 0;
}

void CGCStateEstimateServerDlg::ClearMessage()
{
	CListBox*	pListBox=(CListBox*)GetDlgItem(IDC_MESG_LIST);
	pListBox->ResetContent();
}

void CGCStateEstimateServerDlg::PrintMessage(char* pformat, ...)
{
	va_list args;
	va_start( args, pformat );

	char	szMesg[1024];
	CListBox*	pListBox=(CListBox*)GetDlgItem(IDC_MESG_LIST);

	if (pListBox->GetCount() > 2400)
		pListBox->ResetContent();

	vsprintf(szMesg, pformat, args);

	int	iExt = GetTextLen(szMesg);
	if (iExt > pListBox->GetHorizontalExtent())
		pListBox->SetHorizontalExtent(iExt);
	pListBox->AddString(szMesg);
	pListBox->SetCurSel(pListBox->GetCount()-1);

	va_end(args);
}

void CGCStateEstimateServerDlg::OnEnChangeLocalPort()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_bFreezeUI)
		return;

	char	szBuffer[260];
	GetDlgItem(IDC_LOCAL_PORT)->GetWindowText(szBuffer, 260);
	int		nPort=atoi(szBuffer);
	if (nPort != g_nLocalPort && nPort != 0)
	{
		PrintMessage("�������˿ںű仯����������������Ч");
		g_nLocalPort=atoi(szBuffer);
		SaveIni();
	}
}

void CGCStateEstimateServerDlg::OnEnChangeRemotePort()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_bFreezeUI)
		return;

	char	szBuffer[260];
	GetDlgItem(IDC_REMOTE_PORT)->GetWindowText(szBuffer, 260);
	int		nPort=atoi(szBuffer);
	if (nPort != g_nRemotePort && nPort != 0)
	{
		PrintMessage("����ͨѶ�˿ںű仯���´�����������Ч");
		g_nRemotePort=atoi(szBuffer);
		SaveIni();
	}
}

void CGCStateEstimateServerDlg::OnEnChangeRemoteAddr()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_bFreezeUI)
		return;

	char	szBuffer[260];
	GetDlgItem(IDC_REMOTE_ADDR)->GetWindowText(szBuffer, 260);
	if (strlen(szBuffer) > 0)
	{
		PrintMessage("����ͨѶ��ַ�仯���´�����������Ч");
		strcpy(g_szRemoteAddr, szBuffer);
		SaveIni();
	}
}

void CGCStateEstimateServerDlg::OnBnClickedClearMesg()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ClearMessage();
}

void CGCStateEstimateServerDlg::OnBnClickedStartNetSvr()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	StartNetworkService(&g_StateEstimateControlThread, g_nLocalPort);
}

void CGCStateEstimateServerDlg::OnBnClickedStopNetSvr()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	StopNetworkService();
}

void CGCStateEstimateServerDlg::OnBnClickedJsonWrite()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	FormJsonString(-1);
}

void CGCStateEstimateServerDlg::OnBnClickedJsonRead()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString	fileExt=_T("set");
	CString	defaultFileName=_T("");
	CString	fileFilter="JSon�ļ�(*.json)|*.json;*.JSON|�����ļ�(*.*)|*.*||";
	DWORD	dwFlags = OFN_ENABLESIZING | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

	CFileDialog	dlg(TRUE,fileExt,
		defaultFileName,
		dwFlags,
		fileFilter,
		NULL);

	dlg.m_ofn.lpstrTitle=_T("JSon�ļ�");
	dlg.m_ofn.lpstrInitialDir=_T("");
	dlg.m_ofn.lStructSize=sizeof(dlg.m_ofn);

	if (dlg.DoModal() == IDCANCEL)
		return;

	ifstream ifs;
	ifs.open(dlg.GetPathName());
	assert(ifs.is_open());

	int				nRec;
	Json::Reader	sReader;
	Json::Value		sRoot, sContent, sData, sRecArray, sObject, sNull(nullValue);
	if (!sReader.parse(ifs, sRoot, false))
		return;
	ifs.close();

	PrintMessage("Command = %s", sRoot["Command"].asString().c_str());
	Log(g_lpszLogFile, "Command = %s\n", sRoot["Command"].asString().c_str());

	sContent = sRoot.get("Content", sNull);
	if (!sContent.isNull())
	{
		sData = sContent.get("Value", sNull);
		if (!sData.isNull())
		{
			PrintMessage("CommandValue = %s", sData.asString().c_str());
			Log(g_lpszLogFile, "CommandValue = %s\n", sData.asString().c_str());
		}
		else
		{
			PrintMessage("Content����Value");
		}

		sRecArray = sContent.get(g_PRMemDBInterface.PRGetTableName(PR_FSTATE), sNull);
		if (!sRecArray.isNull() || !sRecArray.isArray())
		{
			PrintMessage("    Table[%s] RecNum = %d", g_PRMemDBInterface.PRGetTableName(PR_FSTATE), sRecArray.size());
			for (nRec=0; nRec<sRecArray.size(); nRec++)
			{
				sObject = sRecArray.get(nRec, sNull);
				if (sObject.isNull() || !sObject.isObject())
					continue;
				PrintMessage("        Table[%s] Record[%d] FieldNum = %d", g_PRMemDBInterface.PRGetTableName(PR_FSTATE), nRec, sObject.size());

				Json::ValueIterator iter = sObject.begin();
				for (; iter != sObject.end(); iter++)
				{
					sData = sObject.get(iter.memberName(), sNull);
					PrintMessage("            Table[%s] Record[%d] Field[%s] = %s", g_PRMemDBInterface.PRGetTableName(PR_FSTATE), nRec, iter.memberName(), sData.asString().c_str());
					//Log(g_lpszLogFile, "Table[%s] Field[%d] = %s", g_PRMemDBInterface.PRGetTableName(PR_FSTATE), nRec, *iter.asString().c_str());
				}
			}
		}
		else
		{
			PrintMessage("Content����%s", g_PRMemDBInterface.PRGetTableName(PR_FSTATE));
		}
	}
	else
	{
		PrintMessage("root����Content");
	}
}

void CGCStateEstimateServerDlg::OnBnClickedComm()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SendNetworkData(g_szRemoteAddr, g_nRemotePort, FormJsonString(-1).c_str());
}


void CGCStateEstimateServerDlg::FormRecordJSon(tagPRBlock* pPRBlock, const int nTable, const int nRecord, Json::Value& sField)
{
	int		nField, nFieldType;
	char	szValue[MDB_CHARLEN_LONG];

	sField.clear();
	for (nField=0; nField<g_PRMemDBInterface.PRGetTableFieldNum(nTable); nField++)
	{
		g_PRMemDBInterface.PRGetRecordValue(pPRBlock, nTable, nField, nRecord, szValue);
		nFieldType = g_PRMemDBInterface.PRGetFieldType(nTable, nField);
		switch (nFieldType)
		{
		case	MDB_STRING:
			sField[g_PRMemDBInterface.PRGetFieldName(nTable, nField)] = szValue;
			break;
		case	MDB_DOUBLE:
			sField[g_PRMemDBInterface.PRGetFieldName(nTable, nField)] = atof(szValue);
			break;
		case	MDB_FLOAT:
			sField[g_PRMemDBInterface.PRGetFieldName(nTable, nField)] = atof(szValue);
			break;
		case	MDB_INT:
			sField[g_PRMemDBInterface.PRGetFieldName(nTable, nField)] = atoi(szValue);
			break;
		case	MDB_SHORT:
			sField[g_PRMemDBInterface.PRGetFieldName(nTable, nField)] = atoi(szValue);
			break;
		case	MDB_BIT:
			sField[g_PRMemDBInterface.PRGetFieldName(nTable, nField)] = atoi(szValue);
			break;
		case	MDB_CHAR:
			sField[g_PRMemDBInterface.PRGetFieldName(nTable, nField)] = (int)szValue[0];
			break;
		default:
			sField[g_PRMemDBInterface.PRGetFieldName(nTable, nField)] = szValue;
			break;
		}
	}
}

std::string CGCStateEstimateServerDlg::FormJsonString(const int nEstimateFState)
{
	Json::Value sRoot;
	Json::Value sContent;
	Json::Value sField;
	Json::Value sFieldArray;
	Json::FastWriter writer;

	register int	i;
	int		nFState, nTable;

	clock_t	dBeg, dEnd;
	int		nDur;
	dBeg=clock();

	sRoot["Command"] = Command_DataReady;

	sContent["Value"] = "1";

	nFState = 446;
//	for (nFState=0; nFState<g_pPRBlock->m_nRecordNum[PR_FSTATE]; nFState++)
	{
//		if (nEstimateFState >= 0 && nFState != nEstimateFState)
//			continue;
//		else
//		{
//			if (nFState % 500 != 0)
//				continue;
//		}

		//////////////////////////////////////////////////////////////////////////
		nTable = PR_FSTATE;
		FormRecordJSon(g_pPRBlock, nTable, nFState, sField);
		sContent[g_PRMemDBInterface.PRGetTableName(nTable)].append( sField );

		//////////////////////////////////////////////////////////////////////////
		nTable = PR_FSTATEMISLAND;
		for (i=0; i<g_pPRBlock->m_nRecordNum[nTable]; i++)
		{
			if (g_pPRBlock->m_FStateMIslandArray[i].nFStateNo != nFState)
				continue;

			FormRecordJSon(g_pPRBlock, nTable, i, sField);
			sContent[g_PRMemDBInterface.PRGetTableName(nTable)].append( sField );
		}

		//////////////////////////////////////////////////////////////////////////
		nTable = PR_FSTATEOVLDEV;
		for (i=0; i<g_pPRBlock->m_nRecordNum[nTable]; i++)
		{
			if (g_pPRBlock->m_FStateOvlDevArray[i].nFStateNo != nFState)
				continue;

			FormRecordJSon(g_pPRBlock, nTable, i, sField);
			sContent[g_PRMemDBInterface.PRGetTableName(nTable)].append( sField );
		}

		//////////////////////////////////////////////////////////////////////////
		nTable = PR_FSTATEOVLAD;
		for (i=0; i<g_pPRBlock->m_nRecordNum[nTable]; i++)
		{
			if (g_pPRBlock->m_FStateOvlAdArray[i].nFStateNo != nFState)
				continue;

			FormRecordJSon(g_pPRBlock, nTable, i, sField);
			sContent[g_PRMemDBInterface.PRGetTableName(nTable)].append( sField );
		}

		//////////////////////////////////////////////////////////////////////////
		nTable = PR_ACBUS;
		for (i=1; i<g_pPRBlock->m_nRecordNum[nTable]; i++)
		{
			sField.clear();
			sField[g_PRMemDBInterface.PRGetFieldName(nTable, PR_ACBUS_NAME)]  = g_pPRBlock->m_ACBusArray[i].szName;
			sField[g_PRMemDBInterface.PRGetFieldName(nTable, PR_ACBUS_KV)]  = g_pPRBlock->m_ACBusArray[i].fkV;
			sField[g_PRMemDBInterface.PRGetFieldName(nTable, PR_ACBUS_PLC)]  = g_pPRBlock->m_ACBusArray[i].fPLC ;
			sField[g_PRMemDBInterface.PRGetFieldName(nTable, PR_ACBUS_ELC)]  = g_pPRBlock->m_ACBusArray[i].fELC ;
			sField[g_PRMemDBInterface.PRGetFieldName(nTable, PR_ACBUS_EENS)] = g_pPRBlock->m_ACBusArray[i].fEENS;
			sField[g_PRMemDBInterface.PRGetFieldName(nTable, PR_ACBUS_EFLC)] = g_pPRBlock->m_ACBusArray[i].fEFLC;
			sContent[g_PRMemDBInterface.PRGetTableName(nTable)].append( sField );
		}
	}
	sRoot["Content"] = sContent;

	dEnd=clock();
	nDur=(int)((1000.0*(double)(dEnd-dBeg))/CLOCKS_PER_SEC);
	PrintMessage("�γɼ�����JSON��ɣ���ʱ%d����\n", nDur);

	std::string	strJson = writer.write(sRoot);

	Log(g_lpszLogFile, "%s\n", strJson.c_str());
	dEnd=clock();
	nDur=(int)((1000.0*(double)(dEnd-dBeg))/CLOCKS_PER_SEC);
	PrintMessage("���JSON��������ɣ���ʱ%d����\n", nDur);

#ifdef _DEBUG
	Log(g_lpszLogFile, "%s\n", sRoot.toStyledString().c_str());
#endif

	dEnd=clock();
	nDur=(int)((1000.0*(double)(dEnd-dBeg))/CLOCKS_PER_SEC);
	PrintMessage("�����ʽ��JSON��������ɣ���ʱ%d����\n", nDur);

	return strJson;
}
