#include "StdAfx.h"
#include "KDTabCtrl.h"

CKDTabCtrl::CKDTabCtrl()
{
}

CKDTabCtrl::~CKDTabCtrl()
{
}

LPARAM CKDTabCtrl::GetCurItemLParam()
{
	TCITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_PARAM;
	if (!GetItem(GetCurSel(), &TabCtrlItem))
		return NULL;
	return TabCtrlItem.lParam;
}

int CKDTabCtrl::FindItemPos(LPCTSTR lpItemText)
{
	int i, iCount = GetItemCount();
	TCITEM tcItem = {0};
	CString sItemTest;
	tcItem.mask = TCIF_TEXT|TCIF_PARAM;
	tcItem.pszText = sItemTest.GetBuffer(MAX_PATH);
	tcItem.cchTextMax = MAX_PATH;

	for (i=0 ; i<iCount ; i++) {
		if (GetItem(i, &tcItem) && tcItem.pszText && (_tcscmp(tcItem.pszText, lpItemText) == 0))
			return i;
	}

	return -1;
}
