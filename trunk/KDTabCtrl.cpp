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