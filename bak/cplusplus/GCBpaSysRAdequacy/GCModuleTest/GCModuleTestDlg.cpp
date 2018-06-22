
// GCModuleTestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GCModuleTest.h"
#include "GCModuleTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGCModuleTestDlg �Ի���
static	unsigned char	m_bFreezeUI = 0;
CGCModuleTestDlg::CGCModuleTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGCModuleTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_nSampleType = 0;
	m_nPRSampleObject=0;
	m_nThreadArray.clear();
}

void CGCModuleTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_PRCOMP_ALL, m_nPRSampleObject);
	DDX_Radio(pDX, IDC_MONTECARLO, m_nSampleType);
}

BEGIN_MESSAGE_MAP(CGCModuleTestDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_DAT_BROWSE, &CGCModuleTestDlg::OnBnClickedDatBrowse)
	ON_BN_CLICKED(IDC_SWI_BROWSE, &CGCModuleTestDlg::OnBnClickedSwiBrowse)
	ON_BN_CLICKED(IDC_RPARAM_BROWSE, &CGCModuleTestDlg::OnBnClickedRparamBrowse)
	ON_EN_CHANGE(IDC_MAXFAULT_GEN, &CGCModuleTestDlg::ResolveRParam)
	ON_EN_CHANGE(IDC_MAXFAULT_BRAN, &CGCModuleTestDlg::ResolveRParam)
	ON_EN_CHANGE(IDC_MCS_SIMULATETIME, &CGCModuleTestDlg::ResolveRParam)
	ON_EN_CHANGE(IDC_MCS_MINSTATEPROB, &CGCModuleTestDlg::ResolveRParam)
	ON_EN_CHANGE(IDC_FST_MAXCUMUPROB, &CGCModuleTestDlg::ResolveRParam)
	ON_EN_CHANGE(IDC_FST_MINSTATEPROB, &CGCModuleTestDlg::ResolveRParam)
	ON_EN_CHANGE(IDC_FST_MAXSTATE, &CGCModuleTestDlg::ResolveRParam)
	ON_EN_CHANGE(IDC_STS_MAXSTATE, &CGCModuleTestDlg::ResolveRParam)
	ON_EN_CHANGE(IDC_ANA_MINSTATEPROB, &CGCModuleTestDlg::ResolveRParam)

	ON_BN_CLICKED(IDC_PRCOMP_ALL, &CGCModuleTestDlg::ResolveRParam)
	ON_BN_CLICKED(IDC_PRCOMP_LT, &CGCModuleTestDlg::ResolveRParam)
	ON_BN_CLICKED(IDC_PRCOMP_GEN, &CGCModuleTestDlg::ResolveRParam)
	ON_BN_CLICKED(IDC_MONTECARLO, &CGCModuleTestDlg::ResolveRParam)
	ON_BN_CLICKED(IDC_ANALYTICAL, &CGCModuleTestDlg::ResolveRParam)
	ON_BN_CLICKED(IDC_STATESAMPLE, &CGCModuleTestDlg::ResolveRParam)
	ON_BN_CLICKED(IDC_FASTSORT, &CGCModuleTestDlg::ResolveRParam)

	ON_BN_CLICKED(IDC_LINE_LIMIT, &CGCModuleTestDlg::ResolveRParam)
	ON_BN_CLICKED(IDC_TRAN_LIMIT, &CGCModuleTestDlg::ResolveRParam)
	ON_BN_CLICKED(IDC_GEN_ELIMIT, &CGCModuleTestDlg::ResolveRParam)
	ON_BN_CLICKED(IDC_UPFC_ELIMIT, &CGCModuleTestDlg::ResolveRParam)
	ON_BN_CLICKED(IDC_UPFC_ADJUSTRC, &CGCModuleTestDlg::ResolveRParam)
	ON_BN_CLICKED(IDC_AUXLOAD_ADJUST, &CGCModuleTestDlg::ResolveRParam)
	ON_BN_CLICKED(IDC_EQGEN_ADJUST, &CGCModuleTestDlg::ResolveRParam)
	ON_BN_CLICKED(IDC_EQLOAD_ADJUST, &CGCModuleTestDlg::ResolveRParam)
	ON_EN_CHANGE(IDC_ISLAND_MINIMAL_GLRATIO, &CGCModuleTestDlg::ResolveRParam)

	ON_BN_CLICKED(IDC_GENBUS_LOAD_ASAUX, &CGCModuleTestDlg::ResolveRParam)
	ON_EN_CHANGE(IDC_DC2AC_FACTOR, &CGCModuleTestDlg::ResolveRParam)
	ON_BN_CLICKED(IDC_BPA_LOADER, &CGCModuleTestDlg::OnBnClickedBpaLoader)
	ON_BN_CLICKED(IDC_BPA_2PR, &CGCModuleTestDlg::OnBnClickedBpa2pr)
	ON_BN_CLICKED(IDC_STATE_SAMPLE, &CGCModuleTestDlg::OnBnClickedStateSample)
	ON_BN_CLICKED(IDC_STATE_ESTIMATE, &CGCModuleTestDlg::OnBnClickedStateEstimate)
	ON_BN_CLICKED(IDC_RELIABILITY_INDEX, &CGCModuleTestDlg::OnBnClickedReliabilityIndex)

	ON_EN_CHANGE(IDC_REMOTE_ADDR, &CGCModuleTestDlg::OnEnChangeRemoteAddr)
	ON_EN_CHANGE(IDC_REMOTE_PORT, &CGCModuleTestDlg::OnEnChangeRemotePort)
	ON_EN_CHANGE(IDC_LOCAL_PORT, &CGCModuleTestDlg::OnEnChangeLocalPort)
	ON_BN_CLICKED(IDC_NETWORK_DATAREADY, &CGCModuleTestDlg::OnBnClickedNetworkDataready)
	ON_BN_CLICKED(IDC_CLEAR_MESG, &CGCModuleTestDlg::OnBnClickedClearMesg)
	ON_MESSAGE(UM_MESSAGE,			OnShowMessage)
	ON_BN_CLICKED(IDC_NETWORK_JOBFINISHED, &CGCModuleTestDlg::OnBnClickedNetworkJobfinished)
	ON_BN_CLICKED(IDC_NETWORK_STATEESTIMATE, &CGCModuleTestDlg::OnBnClickedNetworkStateEstimate)
	ON_EN_CHANGE(IDC_THREAD_NUM, &CGCModuleTestDlg::OnEnChangeThreadNum)
END_MESSAGE_MAP()


// CGCModuleTestDlg ��Ϣ�������

BOOL CGCModuleTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	char	szBuf[260];

	m_bFreezeUI = 1;

	m_nPRSampleObject = g_PRAdeSetting.nPRSampleObject ;
	m_nSampleType = g_PRAdeSetting.nPRSampleMethod-1 ;

	((CButton*)GetDlgItem(IDC_GENBUS_LOAD_ASAUX))->SetCheck(g_PRAdeSetting.bGenBusLoadAsAux);
	sprintf(szBuf, "%.3f", g_PRAdeSetting.fDc2AcFactor);		GetDlgItem(IDC_DC2AC_FACTOR)->SetWindowText(szBuf);

	GetDlgItem(IDC_BPADAT_FILE)		->SetWindowText(g_PRAdeSetting.strBpaDatFile.c_str());
	GetDlgItem(IDC_BPASWI_FILE)		->SetWindowText(g_PRAdeSetting.strBpaSwiFile.c_str());
	GetDlgItem(IDC_BPA_RPARAM_FILE)	->SetWindowText(g_PRAdeSetting.strBpaRParamFile.c_str());

	sprintf(szBuf, "%d", g_PRAdeSetting.nMaxGenFault);		GetDlgItem(IDC_MAXFAULT_GEN)->SetWindowText(szBuf);
	sprintf(szBuf, "%d", g_PRAdeSetting.nMaxBranFault);		GetDlgItem(IDC_MAXFAULT_BRAN)->SetWindowText(szBuf);
	sprintf(szBuf, "%d", g_PRAdeSetting.nMCSSimulateTime);	GetDlgItem(IDC_MCS_SIMULATETIME)->SetWindowText(szBuf);
	sprintf(szBuf, "%g", g_PRAdeSetting.fMCSMinStateProb);	GetDlgItem(IDC_MCS_MINSTATEPROB)->SetWindowText(szBuf);
	sprintf(szBuf, "%d", g_PRAdeSetting.nFSTMaxStateNum);	GetDlgItem(IDC_FST_MAXSTATE)->SetWindowText(szBuf);
	sprintf(szBuf, "%f", g_PRAdeSetting.fFSTMaxCumuProb);	GetDlgItem(IDC_FST_MAXCUMUPROB)->SetWindowText(szBuf);
	sprintf(szBuf, "%g", g_PRAdeSetting.fFSTMinStateProb);	GetDlgItem(IDC_FST_MINSTATEPROB)->SetWindowText(szBuf);
	sprintf(szBuf, "%d", g_PRAdeSetting.nSTSMaxStateNum);	GetDlgItem(IDC_STS_MAXSTATE)->SetWindowText(szBuf);
	sprintf(szBuf, "%g", g_PRAdeSetting.fANAMinStateProb);	GetDlgItem(IDC_ANA_MINSTATEPROB)->SetWindowText(szBuf);

	((CButton*)GetDlgItem(IDC_LINE_LIMIT))		->SetCheck(g_PRAdeSetting.bLineELimit);
	((CButton*)GetDlgItem(IDC_TRAN_LIMIT))		->SetCheck(g_PRAdeSetting.bTranELimit);
	((CButton*)GetDlgItem(IDC_GEN_ELIMIT))		->SetCheck(g_PRAdeSetting.bGenPELimit);
	((CButton*)GetDlgItem(IDC_UPFC_ELIMIT))		->SetCheck(g_PRAdeSetting.bUPFCELimit);
	((CButton*)GetDlgItem(IDC_UPFC_ADJUSTRC))	->SetCheck(g_PRAdeSetting.bUPFCAdjustRC);
	((CButton*)GetDlgItem(IDC_AUXLOAD_ADJUST))	->SetCheck(g_PRAdeSetting.bAuxLoadAdjust);
	((CButton*)GetDlgItem(IDC_EQGEN_ADJUST))	->SetCheck(g_PRAdeSetting.bEQGenAdjust);
	((CButton*)GetDlgItem(IDC_EQLOAD_ADJUST))	->SetCheck(g_PRAdeSetting.bEQLoadAdjust);

	sprintf(szBuf, "%.3f", g_PRAdeSetting.fMinIslandGLRatio);	GetDlgItem(IDC_ISLAND_MINIMAL_GLRATIO)->SetWindowText(szBuf);

	((CButton*)GetDlgItem(IDC_GENBUS_LOAD_ASAUX))->SetCheck(g_PRAdeSetting.bGenBusLoadAsAux);
	sprintf(szBuf, "%.3f", g_PRAdeSetting.fDc2AcFactor);		GetDlgItem(IDC_DC2AC_FACTOR)->SetWindowText(szBuf);

	GetDlgItem(IDC_REMOTE_ADDR)->SetWindowText(g_szRemoteAddr);
	sprintf(szBuf, "%d", g_nRemotePort);
	GetDlgItem(IDC_REMOTE_PORT)->SetWindowText(szBuf);
	sprintf(szBuf, "%d", g_nLocalPort);
	GetDlgItem(IDC_LOCAL_PORT)->SetWindowText(szBuf);
	sprintf(szBuf, "%d", g_nThreadNum);
	GetDlgItem(IDC_THREAD_NUM)->SetWindowText(szBuf);

	m_bFreezeUI = 0;

	StartNetworkService(g_nLocalPort);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CGCModuleTestDlg::OnPaint()
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
HCURSOR CGCModuleTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

int CGCModuleTestDlg::GetTextLen(LPCTSTR lpszText)
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

LRESULT CGCModuleTestDlg::OnShowMessage(WPARAM wParam, LPARAM lParam)
{
	const char*	pChar = (char*)lParam;
	CListBox*	pListBox=(CListBox*)GetDlgItem(IDC_MESG_LIST);
	int	iExt = GetTextLen(pChar);
	if (iExt > pListBox->GetHorizontalExtent())
		pListBox->SetHorizontalExtent(iExt);
	pListBox->AddString(pChar);
	pListBox->SetCurSel(pListBox->GetCount()-1);
	GlobalFree((HGLOBAL)pChar);

	return 0;
}

void CGCModuleTestDlg::ClearMessage()
{
	CListBox*	pListBox=(CListBox*)GetDlgItem(IDC_MESG_LIST);
	pListBox->ResetContent();
}

void CGCModuleTestDlg::PrintMessage(char* pformat, ...)
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

void CGCModuleTestDlg::ResolveRParam()
{
	if (m_bFreezeUI)
		return;

	char	szBuffer[260];

	UpdateData();

	g_PRAdeSetting.nPRSampleObject = m_nPRSampleObject;
	switch (m_nSampleType)
	{
	case	0:	g_PRAdeSetting.nPRSampleMethod = PRFState_SamplingMethod_MonteCarlo;	break;
	case	1:	g_PRAdeSetting.nPRSampleMethod = PRFState_SamplingMethod_StateSample;	break;
	case	2:	g_PRAdeSetting.nPRSampleMethod = PRFState_SamplingMethod_Analytical;	break;
	case	3:	g_PRAdeSetting.nPRSampleMethod = PRFState_SamplingMethod_FastSort;		break;
	default:	g_PRAdeSetting.nPRSampleMethod = PRFState_SamplingMethod_Unknown;		break;
	}

	GetDlgItem(IDC_MAXFAULT_GEN)->GetWindowText(szBuffer, 260);					g_PRAdeSetting.nMaxGenFault=atoi(szBuffer);
	GetDlgItem(IDC_MAXFAULT_BRAN)->GetWindowText(szBuffer, 260);				g_PRAdeSetting.nMaxBranFault=atoi(szBuffer);
	GetDlgItem(IDC_MCS_SIMULATETIME)->GetWindowText(szBuffer, 260);				g_PRAdeSetting.nMCSSimulateTime=atoi(szBuffer);
	GetDlgItem(IDC_MCS_MINSTATEPROB)->GetWindowText(szBuffer, 260);				g_PRAdeSetting.fMCSMinStateProb=atof(szBuffer);
	GetDlgItem(IDC_FST_MAXCUMUPROB)->GetWindowText(szBuffer, 260);				g_PRAdeSetting.fFSTMaxCumuProb=atof(szBuffer);
	GetDlgItem(IDC_FST_MAXSTATE)->GetWindowText(szBuffer, 260);					g_PRAdeSetting.nFSTMaxStateNum=atoi(szBuffer);
	GetDlgItem(IDC_FST_MINSTATEPROB)->GetWindowText(szBuffer, 260);				g_PRAdeSetting.fFSTMinStateProb=atof(szBuffer);
	GetDlgItem(IDC_STS_MAXSTATE)->GetWindowText(szBuffer, 260);					g_PRAdeSetting.nSTSMaxStateNum=atoi(szBuffer);
	GetDlgItem(IDC_ANA_MINSTATEPROB)->GetWindowText(szBuffer, 260);				g_PRAdeSetting.fANAMinStateProb=atof(szBuffer);

	g_PRAdeSetting.bLineELimit		= ((CButton*)GetDlgItem(IDC_LINE_LIMIT))	->GetCheck();
	g_PRAdeSetting.bTranELimit		= ((CButton*)GetDlgItem(IDC_TRAN_LIMIT))	->GetCheck();
	g_PRAdeSetting.bGenPELimit		= ((CButton*)GetDlgItem(IDC_GEN_ELIMIT))	->GetCheck();
	g_PRAdeSetting.bUPFCELimit		= ((CButton*)GetDlgItem(IDC_UPFC_ELIMIT))	->GetCheck();
	g_PRAdeSetting.bAuxLoadAdjust	= ((CButton*)GetDlgItem(IDC_AUXLOAD_ADJUST))->GetCheck();
	g_PRAdeSetting.bEQGenAdjust		= ((CButton*)GetDlgItem(IDC_EQGEN_ADJUST))	->GetCheck();
	g_PRAdeSetting.bEQLoadAdjust	= ((CButton*)GetDlgItem(IDC_EQLOAD_ADJUST))	->GetCheck();
	g_PRAdeSetting.bUPFCAdjustRC	= ((CButton*)GetDlgItem(IDC_UPFC_ADJUSTRC))	->GetCheck();

	GetDlgItem(IDC_ISLAND_MINIMAL_GLRATIO)->GetWindowText(szBuffer, 260);	g_PRAdeSetting.fMinIslandGLRatio=atof(szBuffer);

	g_PRAdeSetting.bGenBusLoadAsAux = ((CButton*)GetDlgItem(IDC_GENBUS_LOAD_ASAUX))->GetCheck();
	GetDlgItem(IDC_DC2AC_FACTOR)->GetWindowText(szBuffer, 260);					g_PRAdeSetting.fDc2AcFactor=atof(szBuffer);
	if (g_PRAdeSetting.fDc2AcFactor < 1 || g_PRAdeSetting.fDc2AcFactor > 2)
		g_PRAdeSetting.fDc2AcFactor=1.15;

	SaveBpaPRAdequacySetting(&g_PRAdeSetting);
}

void CGCModuleTestDlg::OnBnClickedDatBrowse()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString	fileExt=_T("dat");
	CString	defaultFileName=g_PRAdeSetting.strBpaDatFile.c_str();
	CString	fileFilter=_T("BPA�����ļ�(*.dat)|*.dat;*.DAT|�����ļ�(*.*)|*.*||");
	DWORD	dwFlags = OFN_ENABLESIZING | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

	CFileDialog	dlg(TRUE,fileExt,
		defaultFileName,
		dwFlags,
		fileFilter,
		NULL);

	dlg.m_ofn.lpstrTitle=_T("��BPA�����ļ�");
	dlg.m_ofn.lpstrInitialDir=_T("");
	dlg.m_ofn.lStructSize=sizeof(dlg.m_ofn);

	if (dlg.DoModal() == IDCANCEL)
		return;

	g_PRAdeSetting.strBpaDatFile = dlg.GetPathName();
	GetDlgItem(IDC_BPADAT_FILE)->SetWindowText(g_PRAdeSetting.strBpaDatFile.c_str());
	SaveBpaPRAdequacySetting(&g_PRAdeSetting);
}

void CGCModuleTestDlg::OnBnClickedSwiBrowse()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString	fileExt=_T("swi");
	CString	defaultFileName=g_PRAdeSetting.strBpaSwiFile.c_str();
	CString	fileFilter=_T("BPA�ȶ��ļ�(*.swi)|*.swi;*.SWI|�����ļ�(*.*)|*.*||");
	DWORD	dwFlags = OFN_ENABLESIZING | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

	CFileDialog	dlg(TRUE,fileExt,
		defaultFileName,
		dwFlags,
		fileFilter,
		NULL);

	dlg.m_ofn.lpstrTitle=_T("��BPA�ȶ��ļ�");
	dlg.m_ofn.lpstrInitialDir=_T("");
	dlg.m_ofn.lStructSize=sizeof(dlg.m_ofn);

	if (dlg.DoModal() == IDCANCEL)
		return;

	g_PRAdeSetting.strBpaSwiFile = dlg.GetPathName();
	GetDlgItem(IDC_BPASWI_FILE)->SetWindowText(g_PRAdeSetting.strBpaSwiFile.c_str());
	SaveBpaPRAdequacySetting(&g_PRAdeSetting);
}

void CGCModuleTestDlg::OnBnClickedRparamBrowse()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString	fileExt=_T("XML");
	CString	defaultFileName=g_PRAdeSetting.strBpaRParamFile.c_str();
	CString	fileFilter=_T("�ɿ����豸�����ļ�(*.xml)|*.xml;*.XML|�����ļ�(*.*)|*.*||");
	DWORD	dwFlags = OFN_ENABLESIZING | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

	CFileDialog	dlg(TRUE,fileExt,
		defaultFileName,
		dwFlags,
		fileFilter,
		NULL);

	dlg.m_ofn.lpstrTitle=_T("�ɿ����豸�����ļ�");
	dlg.m_ofn.lpstrInitialDir=_T("");
	dlg.m_ofn.lStructSize=sizeof(dlg.m_ofn);

	if (dlg.DoModal() == IDCANCEL)
		return;

	g_PRAdeSetting.strBpaRParamFile = dlg.GetPathName();
	GetDlgItem(IDC_BPA_RPARAM_FILE)->SetWindowText(g_PRAdeSetting.strBpaRParamFile.c_str());
	SaveBpaPRAdequacySetting(&g_PRAdeSetting);
}

void CGCModuleTestDlg::OnBnClickedBpaLoader()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char	szExce[260];
	sprintf(szExce, "%s/GCBPALoader %s %s %s", g_szRunDir, g_szRunDir, g_PRAdeSetting.strBpaDatFile.c_str(), g_PRAdeSetting.strBpaSwiFile.c_str());
	Log(g_lpszLogFile, "Exec = %s\n", szExce);
	system(szExce);
}

void CGCModuleTestDlg::OnBnClickedBpa2pr()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char	szExce[260];
	sprintf(szExce, "%s/GCBPA2PR %s %s %s %s", g_szRunDir, g_szRunDir, g_PRAdeSetting.strBpaDatFile.c_str(), g_PRAdeSetting.strBpaSwiFile.c_str(), g_PRAdeSetting.strBpaRParamFile.c_str());
	Log(g_lpszLogFile, "Exec = %s\n", szExce);
	system(szExce);
}

void CGCModuleTestDlg::OnBnClickedStateSample()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char	szBuffer[260];
	std::string	strExec;

	strExec.clear();
	sprintf(szBuffer, "%s/GCStateSample %s ", g_szRunDir, g_szRunDir);	strExec.append(szBuffer);
	sprintf(szBuffer, "%d ", g_PRAdeSetting.nPRSampleObject);		strExec.append(szBuffer); //�����������ͣ�ȫ����֧·�������
	sprintf(szBuffer, "%d ", g_PRAdeSetting.nPRSampleMethod);		strExec.append(szBuffer); //��������
	sprintf(szBuffer, "%d ", g_PRAdeSetting.nMaxGenFault);			strExec.append(szBuffer); // ������󷢵����������
	sprintf(szBuffer, "%d ", g_PRAdeSetting.nMaxBranFault);			strExec.append(szBuffer); //�������֧·��������
	sprintf(szBuffer, "%d ", g_PRAdeSetting.nMCSSimulateTime);		strExec.append(szBuffer); //MCS����������ʱ��
	sprintf(szBuffer, "%f ", g_PRAdeSetting.fMCSMinStateProb);		strExec.append(szBuffer); //MCS[���ؿ���]�豸���ϸ����ż�ֵ
	sprintf(szBuffer, "%f ", g_PRAdeSetting.fFSTMaxCumuProb);		strExec.append(szBuffer); //FST[��������]�ۻ�����
	sprintf(szBuffer, "%f ", g_PRAdeSetting.fFSTMinStateProb);		strExec.append(szBuffer); //FST[��������]�豸���ϸ����ż�ֵ
	sprintf(szBuffer, "%d ", g_PRAdeSetting.nFSTMaxStateNum);		strExec.append(szBuffer); //FST[��������]���״̬��
	sprintf(szBuffer, "%d ", g_PRAdeSetting.nSTSMaxStateNum);		strExec.append(szBuffer); //STS[״̬����]���״̬��
	sprintf(szBuffer, "%f ", g_PRAdeSetting.fANAMinStateProb);		strExec.append(szBuffer); //ANA[������]�豸���ϸ����ż�ֵ

	Log(g_lpszLogFile, "Exec = %s\n", strExec.c_str());
	system(strExec.c_str());
}

void CGCModuleTestDlg::OnBnClickedStateEstimate()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char	szBuffer[260];
	std::string	strExec;

	strExec.clear();
	sprintf(szBuffer, "%s/GCStateEstimate %s ", g_szRunDir, g_szRunDir);	strExec.append(szBuffer);
	sprintf(szBuffer, "%f ", g_PRAdeSetting.fDc2AcFactor);		strExec.append(szBuffer); //ֱ������2 ��������ϵ��
	sprintf(szBuffer, "%d ", g_PRAdeSetting.bLineELimit);		strExec.append(szBuffer); //��·����
	sprintf(szBuffer, "%d ", g_PRAdeSetting.bTranELimit);		strExec.append(szBuffer); //��������
	sprintf(szBuffer, "%d ", g_PRAdeSetting.bGenPELimit);		strExec.append(szBuffer); //�������������
	sprintf(szBuffer, "%d ", g_PRAdeSetting.bUPFCELimit);		strExec.append(szBuffer); //����UPFC����
	sprintf(szBuffer, "%d ", g_PRAdeSetting.bAuxLoadAdjust);	strExec.append(szBuffer); //���õ��������
	sprintf(szBuffer, "%d ", g_PRAdeSetting.bEQGenAdjust);		strExec.append(szBuffer); //��ֵ�������������
	sprintf(szBuffer, "%d ", g_PRAdeSetting.bEQLoadAdjust);		strExec.append(szBuffer); //��ֵ���ɲ�������
	sprintf(szBuffer, "%f ", g_PRAdeSetting.fMinIslandGLRatio);	strExec.append(szBuffer); //�µ�����С���ر�
	sprintf(szBuffer, "%d ", g_PRAdeSetting.bUPFCAdjustRC);		strExec.append(szBuffer); //UPFC���ñ��ݷ�

	Log(g_lpszLogFile, "Exec = %s\n", strExec.c_str());
	system(strExec.c_str());
}


void CGCModuleTestDlg::OnBnClickedReliabilityIndex()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char	szExce[260];
	sprintf(szExce, "%s/GCReliabilityIndex %s %f", g_szRunDir, g_szRunDir, g_PRAdeSetting.fDc2AcFactor);
	Log(g_lpszLogFile, "Exec = %s\n", szExce);
	system(szExce);
}

void CGCModuleTestDlg::OnEnChangeRemoteAddr()
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
		strcpy(g_szRemoteAddr, szBuffer);
	}
}

void CGCModuleTestDlg::OnEnChangeRemotePort()
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
		g_nRemotePort=atoi(szBuffer);
	}
}

void CGCModuleTestDlg::OnEnChangeLocalPort()
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
		g_nLocalPort=atoi(szBuffer);
	}
}

void CGCModuleTestDlg::OnEnChangeThreadNum()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_bFreezeUI)
		return;

	SYSTEM_INFO sysInfo;
	::GetSystemInfo(&sysInfo);

	char	szBuffer[260];
	GetDlgItem(IDC_THREAD_NUM)->GetWindowText(szBuffer, 260);
	g_nThreadNum=atoi(szBuffer);
	if (g_nThreadNum <= 0)
		g_nThreadNum=1;
	if (g_nThreadNum > sysInfo.dwNumberOfProcessors)
		g_nThreadNum=sysInfo.dwNumberOfProcessors;
}

void CGCModuleTestDlg::OnBnClickedClearMesg()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CListBox*	pListBox=(CListBox*)GetDlgItem(IDC_MESG_LIST);
	pListBox->ResetContent();
}

void CGCModuleTestDlg::OnBnClickedNetworkDataready()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char	szBuf[260];
	GetDlgItem(IDC_THREAD_NUM)->GetWindowText(szBuf, 260);
	SendNetworkData(g_szRemoteAddr, g_nRemotePort, FormCommandJSon(Command_DataReady, 1).c_str());
	m_nThreadArray.resize(atoi(szBuf), 0);
}

void CGCModuleTestDlg::OnBnClickedNetworkJobfinished()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SendNetworkData(g_szRemoteAddr, g_nRemotePort, FormCommandJSon(Command_JobFinished, 0).c_str());
}

std::string	CGCModuleTestDlg::FormCommandJSon(const char* lpszCommand, const unsigned char bSendSetting)
{
	Json::Value			sRoot;
	Json::Value			sContent;
	Json::Value			sObject;
	Json::FastWriter	sWriter;

	ResolveRParam();

	sRoot["Command"] = lpszCommand;

	if (bSendSetting)
	{
		sObject["BpaDatFile"] = g_PRAdeSetting.strBpaDatFile;				//���������ļ�
		sObject["BpaSwiFile"] = g_PRAdeSetting.strBpaSwiFile;				//�ȶ������ļ�����Ҫ�������γɷ����ģ��
		sObject["BpaRParamFile"] = g_PRAdeSetting.strBpaRParamFile;			//�ɿ��Բ��������ļ�
// 		sprintf(szBuffer, "%f", g_PRAdeSetting.fTinyGenThreshold);	sObject["TinyGenThreshold"] = szBuffer;	//�������ֽⷢ��������ż�ֵ���������ڸ��ż�ֵ�ķ������Ϊ���Ƿ������
// 		sprintf(szBuffer, "%f", g_PRAdeSetting.fLowVoltThreshold);	sObject["LowVoltThreshold"] = szBuffer;	//�������ֽⷢ����͵�ѹ�ż�ֵ����ѹ���ڸ��ż�ֵ�ķ������Ϊ�Ǹ��ɶ����Ƿ��磩
// 		sprintf(szBuffer, "%f", g_PRAdeSetting.fZIL				);	sObject["ZIL"] = szBuffer;				//С�迹���С�迹�ż�ֵ
// 		sprintf(szBuffer, "%d", g_PRAdeSetting.nPRSampleObject	);	sObject["PRSampleObject"] = szBuffer;	//��������ȫ�������硢֧·��
// 		sprintf(szBuffer, "%d", g_PRAdeSetting.nPRSampleMethod	);	sObject["PRSampleMethod"] = szBuffer;	//��������
// 		sprintf(szBuffer, "%d", g_PRAdeSetting.nMaxGenFault		);	sObject["MaxGenFault"] = szBuffer;		//MCS, FST, STS, ANA ������󷢵����������
// 		sprintf(szBuffer, "%d", g_PRAdeSetting.nMaxBranFault	);	sObject["MaxBranFault"] = szBuffer;		//MCS, FST, STS, ANA �������֧·��������
// 		sprintf(szBuffer, "%d", g_PRAdeSetting.bLineELimit		);	sObject["LineELimit"] = szBuffer;		//��·���޿���
// 		sprintf(szBuffer, "%d", g_PRAdeSetting.bTranELimit		);	sObject["TranELimit"] = szBuffer;		//�������޿���
// 		sprintf(szBuffer, "%d", g_PRAdeSetting.bGenPELimit		);	sObject["GenPELimit"] = szBuffer;		//�������������
// 		sprintf(szBuffer, "%d", g_PRAdeSetting.bUPFCELimit		);	sObject["UPFCELimit"] = szBuffer;		//UPFC��������
// 		sprintf(szBuffer, "%d", g_PRAdeSetting.bAuxLoadAdjust	);	sObject["AuxLoadAdjust"] = szBuffer;	//���õ縺�ɲ������
// 		sprintf(szBuffer, "%d", g_PRAdeSetting.bEQGenAdjust		);	sObject["EQGenAdjust"] = szBuffer;		//��ֵ������������
// 		sprintf(szBuffer, "%d", g_PRAdeSetting.bEQLoadAdjust	);	sObject["EQLoadAdjust"] = szBuffer;		//��ֵ������������
// 		sprintf(szBuffer, "%d", g_PRAdeSetting.nMCSSimulateTime	);	sObject["MCSSimulateTime"] = szBuffer;	//MCS����������ʱ��
// 		sprintf(szBuffer, "%f", g_PRAdeSetting.fMCSMinStateProb	);	sObject["MCSMinStateProb"] = szBuffer;	//MCS.get(���ؿ���]�豸���ϸ����ż�ֵ
// 		sprintf(szBuffer, "%f", g_PRAdeSetting.fFSTMaxCumuProb	);	sObject["FSTMaxCumuProb"] = szBuffer;	//FST.get(��������]�ۻ�����
// 		sprintf(szBuffer, "%f", g_PRAdeSetting.fFSTMinStateProb	);	sObject["FSTMinStateProb"] = szBuffer;	//FST.get(��������]�豸���ϸ����ż�ֵ
// 		sprintf(szBuffer, "%d", g_PRAdeSetting.nFSTMaxStateNum	);	sObject["FSTMaxStateNum"] = szBuffer;	//FST.get(��������]���״̬��
// 		sprintf(szBuffer, "%d", g_PRAdeSetting.nSTSMaxStateNum	);	sObject["STSMaxStateNum"] = szBuffer;	//STS.get(״̬����]���״̬��
// 		sprintf(szBuffer, "%f", g_PRAdeSetting.fANAMinStateProb	);	sObject["ANAMinStateProb"] = szBuffer;	//ANA.get( ������]�豸���ϸ����ż�ֵ
// 		sprintf(szBuffer, "%f", g_PRAdeSetting.fDc2AcFactor		);	sObject["Dc2AcFactor"] = szBuffer;		//ֱ������2 ��������ϵ��
// 		sprintf(szBuffer, "%f", g_PRAdeSetting.fMinIslandGLRatio);	sObject["MinIslandGLRatio"] = szBuffer;	//�µ�����С���ر�
// 		sprintf(szBuffer, "%d", g_nThreadNum					);	sObject["MultiThread"] = szBuffer;	//�߳���
// 		sprintf(szBuffer, "%d", g_PRAdeSetting.bUPFCAdjustRC	);	sObject["UPFCAdjustRC"] = szBuffer;		//UPFC���õ���������
// 		sprintf(szBuffer, "%d", g_PRAdeSetting.bGenBusLoadAsAux	);	sObject["GenBusLoadAsAux"] = szBuffer;	//�����ĸ�߸��ɰ����õ紦��

		sObject["TinyGenThreshold"] = g_PRAdeSetting.fTinyGenThreshold	;	//�������ֽⷢ��������ż�ֵ���������ڸ��ż�ֵ�ķ������Ϊ���Ƿ������
		sObject["LowVoltThreshold"] = g_PRAdeSetting.fLowVoltThreshold	;	//�������ֽⷢ����͵�ѹ�ż�ֵ����ѹ���ڸ��ż�ֵ�ķ������Ϊ�Ǹ��ɶ����Ƿ��磩
		sObject["ZIL"]				= g_PRAdeSetting.fZIL				;	//С�迹���С�迹�ż�ֵ
		sObject["PRSampleObject"]	= g_PRAdeSetting.nPRSampleObject	;	//��������ȫ�������硢֧·��
		sObject["PRSampleMethod"]	= g_PRAdeSetting.nPRSampleMethod	;	//��������
		sObject["MaxGenFault"]		= g_PRAdeSetting.nMaxGenFault		;	//MCS, FST, STS, ANA ������󷢵����������
		sObject["MaxBranFault"]		= g_PRAdeSetting.nMaxBranFault		;	//MCS, FST, STS, ANA �������֧·��������
		sObject["LineELimit"]		= g_PRAdeSetting.bLineELimit		;	//��·���޿���
		sObject["TranELimit"]		= g_PRAdeSetting.bTranELimit		;	//�������޿���
		sObject["GenPELimit"]		= g_PRAdeSetting.bGenPELimit		;	//�������������
		sObject["UPFCELimit"]		= g_PRAdeSetting.bUPFCELimit		;	//UPFC��������
		sObject["AuxLoadAdjust"]	= g_PRAdeSetting.bAuxLoadAdjust		;	//���õ縺�ɲ������
		sObject["EQGenAdjust"]		= g_PRAdeSetting.bEQGenAdjust		;	//��ֵ������������
		sObject["EQLoadAdjust"]		= g_PRAdeSetting.bEQLoadAdjust		;	//��ֵ������������
		sObject["MCSSimulateTime"]	= g_PRAdeSetting.nMCSSimulateTime	;	//MCS����������ʱ��
		sObject["MCSMinStateProb"]	= g_PRAdeSetting.fMCSMinStateProb	;	//MCS.get(���ؿ���]�豸���ϸ����ż�ֵ
		sObject["FSTMaxCumuProb"]	= g_PRAdeSetting.fFSTMaxCumuProb	;	//FST.get(��������]�ۻ�����
		sObject["FSTMinStateProb"]	= g_PRAdeSetting.fFSTMinStateProb	;	//FST.get(��������]�豸���ϸ����ż�ֵ
		sObject["FSTMaxStateNum"]	= g_PRAdeSetting.nFSTMaxStateNum	;	//FST.get(��������]���״̬��
		sObject["STSMaxStateNum"]	= g_PRAdeSetting.nSTSMaxStateNum	;	//STS.get(״̬����]���״̬��
		sObject["ANAMinStateProb"]	= g_PRAdeSetting.fANAMinStateProb	;	//ANA.get( ������]�豸���ϸ����ż�ֵ
		sObject["Dc2AcFactor"]		= g_PRAdeSetting.fDc2AcFactor		;	//ֱ������2 ��������ϵ��
		sObject["MinIslandGLRatio"] = g_PRAdeSetting.fMinIslandGLRatio	;	//�µ�����С���ر�
		sObject["MultiThread"]		= g_nThreadNum						;	//�߳���
		sObject["UPFCAdjustRC"]		= g_PRAdeSetting.bUPFCAdjustRC		;	//UPFC���õ���������
		sObject["GenBusLoadAsAux"]	= g_PRAdeSetting.bGenBusLoadAsAux	;	//�����ĸ�߸��ɰ����õ紦��

		sContent["PRAdequacySetting"] = sObject;
	}

	sRoot["Content"] = sContent;

	return sWriter.write(sRoot);
}

void CGCModuleTestDlg::OnBnClickedNetworkStateEstimate()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	PRStateEstimateCalculateDispatch(AfxGetMainWnd(), g_pPRBlock);
}
