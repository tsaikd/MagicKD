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
	POSITION pos = GetFirstSelectedItemPosition();
	while (pos) {
		DeleteItem((int)pos-1);
		pos = GetFirstSelectedItemPosition();
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