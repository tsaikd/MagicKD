#pragma once
#include <afxcmn.h>
#include "KDListItem.h"

class CKDListCtrl : public CListCtrl
{
public:
	CKDListCtrl();
	virtual ~CKDListCtrl();

	void DeleteSelectItem();
	LPARAM GetFirstSelectedItemLParam();
};
