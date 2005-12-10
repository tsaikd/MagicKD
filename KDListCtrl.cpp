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
