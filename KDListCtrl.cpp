#include "StdAfx.h"
#include "KDListCtrl.h"

CKDListCtrl::CKDListCtrl()
	: m_bEnableToolTip(FALSE)
{
}

CKDListCtrl::~CKDListCtrl()
{
}

void CKDListCtrl::DeleteSelectItem()
{
	int nItem;
	POSITION pos;
	while (pos = GetFirstSelectedItemPosition()) {
		nItem = GetNextSelectedItem(pos);
		DeleteItem(nItem);
	}
}

void CKDListCtrl::CancleAllSelected()
{
	int nItem;
	POSITION pos;
	while (pos = GetFirstSelectedItemPosition()) {
		nItem = GetNextSelectedItem(pos);
		SetItemState(nItem, 0, LVIS_SELECTED);
	}
}

LPARAM CKDListCtrl::GetFirstSelectedItemLParam()
{
	POSITION pos = GetFirstSelectedItemPosition();
	return GetItemLParam(GetNextSelectedItem(pos));
}

#if (_WIN32_IE >= 0x0300)
void CKDListCtrl::SetSelectItemCheckState(bool bCheck)
{
	int nItem;
	POSITION pos = GetFirstSelectedItemPosition();
	while (pos) {
		nItem = GetNextSelectedItem(pos);
		ListView_SetCheckState(m_hWnd, nItem, bCheck);
		SetItemState(nItem, 0, LVIS_SELECTED);
	}
}
#endif

int CKDListCtrl::FindItemByText(LPCTSTR sText)
{
	LVFINDINFO itemInfo;
	itemInfo.flags = LVFI_STRING;
	itemInfo.psz = sText;
	return FindItem(&itemInfo);
}

LPARAM CKDListCtrl::GetItemLParam(int nItem)
{
	LVITEM lvitem = {0};
	lvitem.mask = LVIF_PARAM;
	lvitem.iItem = nItem;
	if (GetItem(&lvitem))
		return lvitem.lParam;
	else
		return NULL;
}

BOOL CKDListCtrl::SetItemSelected(int nItem)
{
	return SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);
}

BOOL CKDListCtrl::EnableToolTips(LPCTSTR sToolTip/* = NULL*/, BOOL bEnable/* = TRUE*/)
{
	if (bEnable)
		SetToolTips(sToolTip);
	return CListCtrl::EnableToolTips(bEnable);
}

BOOL CKDListCtrl::IsEnableToolTips()
{
	return m_bEnableToolTip;
}

void CKDListCtrl::SetToolTips(LPCTSTR sToolTip)
{
	if (sToolTip)
		m_sToolTip = sToolTip;
}

INT_PTR CKDListCtrl::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	if (pTI && !m_sToolTip.IsEmpty()) {
		pTI->hwnd = m_hWnd;
		pTI->uId = (UINT_PTR) m_hWnd;
		pTI->uFlags |= TTF_ALWAYSTIP;
		pTI->lpszText = _tcsdup(m_sToolTip);
		GetClientRect(&pTI->rect);

		return GetDlgCtrlID();
	}
	return -1;
}

BEGIN_MESSAGE_MAP(CKDListCtrl, CListCtrl)
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, OnLvnGetdispinfo)
END_MESSAGE_MAP()

void CKDListCtrl::OnLvnGetdispinfo(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);

	if (pDispInfo->item.mask & LVIF_TEXT) {
		TCHAR sText[MAX_PATH] = {0};
		CKDListItem *pItem = (CKDListItem *) pDispInfo->item.lParam;
		_stprintf(sText, _T("%s"), pItem->GetText(pDispInfo->item.iSubItem));
		pDispInfo->item.pszText = sText;
	}

	*pResult = 0;
}
