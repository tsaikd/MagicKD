#include "StdAfx.h"
#include "Resource.h"
#include "FeedSource.h"
#include "PicCollectorDlg.h"

#include "PicCFeedListCtrl.h"

CPicCFeedListCtrl::CPicCFeedListCtrl()
	: m_bInit(false)
{
}

CPicCFeedListCtrl::~CPicCFeedListCtrl()
{
}

void CPicCFeedListCtrl::Init()
{
	CKDListCtrl::Init();

	CRect rcWin;
	GetClientRect(rcWin);
//	InsertColumn(0, CResString(IDS_FIND_COLUMN_FINDLIST), LVCFMT_LEFT, rcWin.right);
	InsertColumn(0, _T("Name"), LVCFMT_LEFT, 50);
	InsertColumn(1, _T("Feed Title"), LVCFMT_LEFT, 100);
	InsertColumn(2, _T("Feed URL"), LVCFMT_LEFT, rcWin.right-150);

	SetExtendedStyle(GetExtendedStyle() | LVS_EX_FULLROWSELECT);

	CStringArray strTitleArray, strLinkArray;
	g_pPicCollectorDlg->m_Feed.GetFeedSourceList(strTitleArray, strLinkArray);

	ReloadItems();

	m_bInit = true;
}

void CPicCFeedListCtrl::AddItem(CPicCFeedListItem *pItem)
{
	InsertItem(LVIF_PARAM | LVIF_TEXT | LVIF_IMAGE, GetItemCount(), LPSTR_TEXTCALLBACK, 0, 0, I_IMAGECALLBACK, (LPARAM) pItem);
}

void CPicCFeedListCtrl::ReloadItems()
{
	CStringArray strTitleArray, strLinkArray;
	g_pPicCollectorDlg->m_Feed.GetFeedSourceList(strTitleArray, strLinkArray);

	CPicCFeedListItem *pItem;
	int i;
	for (i=0 ; i<strTitleArray.GetCount() ; i++) {
		pItem = new CPicCFeedListItem;
		pItem->SetText(0, g_pPicCollectorDlg->m_Feed.GetFeedName(strLinkArray[i]));
		pItem->SetText(1, strTitleArray[i]);
		pItem->SetText(2, strLinkArray[i]);
		AddItem(pItem);
	}
}

BEGIN_MESSAGE_MAP(CPicCFeedListCtrl, CKDListCtrl)
	ON_NOTIFY_REFLECT(LVN_DELETEITEM, &CPicCFeedListCtrl::OnLvnDeleteitem)
END_MESSAGE_MAP()

void CPicCFeedListCtrl::OnLvnDeleteitem(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	CPicCFeedListItem *pItem = (CPicCFeedListItem *)pNMLV->lParam;
	DEL(pItem);

	*pResult = 0;
}
