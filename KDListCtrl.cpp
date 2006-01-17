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

int CKDListCtrl::FindItemByText(LPCTSTR sText)
{
	LVFINDINFO itemInfo;
	itemInfo.flags = LVFI_STRING;
	itemInfo.psz = sText;
	return FindItem(&itemInfo);
}

BOOL CKDListCtrl::SetItemSelected(int nItem)
{
	return SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);
}
