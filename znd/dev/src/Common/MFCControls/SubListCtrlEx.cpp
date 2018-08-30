#include "stdafx.h"
#include "SubListCtrlEx.h"
//////////////////////////////////////////////////////////////////////////
// CSubListCtrlEx 
IMPLEMENT_DYNAMIC(CSubListCtrlEx, CMFCListCtrlEx)

CSubListCtrlEx::CSubListCtrlEx()
{
	m_pParentWnd=NULL;
}

CSubListCtrlEx::~CSubListCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CSubListCtrlEx, CMFCListCtrlEx)
	ON_NOTIFY_REFLECT(NM_CLICK, &CSubListCtrlEx::OnNMClick)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CSubListCtrlEx::OnNMDblclk)
END_MESSAGE_MAP()


// CSubListCtrlEx ��Ϣ�������
void CSubListCtrlEx::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_pParentWnd)
		m_pParentWnd->SendMessage(UM_SUBLISTCTRL_NMCLICKED, (WPARAM)GetDlgCtrlID(), (LPARAM)pNMItemActivate);
	*pResult = 0;
}

void CSubListCtrlEx::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_pParentWnd)
		m_pParentWnd->SendMessage(UM_SUBLISTCTRL_NMDBLCLK, (WPARAM)GetDlgCtrlID(), (LPARAM)pNMItemActivate);
	*pResult = 0;
}
