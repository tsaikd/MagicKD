#include "StdAfx.h"
#include "KDListCtrl.h"

CKDListCtrl::CKDListCtrl()
{
}

CKDListCtrl::~CKDListCtrl()
{
}

void CKDListCtrl::DeleteSelectItem()
{
	int nItem;
	POSITION pos = GetFirstSelectedItemPosition();
	while (pos) {
		nItem = GetNextSelectedItem(pos);
		DeleteItem(nItem);
	}
}

void CKDListCtrl::CancleAllSelected()
{
	int nItem;
	POSITION pos = GetFirstSelectedItemPosition();
	while (pos) {
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
