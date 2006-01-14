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
	LVITEM lvitem = {0};
	lvitem.mask = LVIF_PARAM;
	lvitem.iItem = (int)GetFirstSelectedItemPosition() - 1;
	if (GetItem(&lvitem))
		return lvitem.lParam;
	else
		return NULL;
}