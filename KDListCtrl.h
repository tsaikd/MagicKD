#pragma once
#include <afxcmn.h>
#include "KDListItem.h"

class CKDListCtrl : public CListCtrl
{
public:
	CKDListCtrl();
	virtual ~CKDListCtrl();

	void DeleteSelectItem();
	void CancleAllSelected();
	LPARAM GetFirstSelectedItemLParam();
	LPARAM GetItemLParam(int nItem);
	int FindItemByText(LPCTSTR sText);
	BOOL SetItemSelected(int nItem);
};
