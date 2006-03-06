#pragma once
#include "afxcmn.h"

class CKDTabCtrl :
	public CTabCtrl
{
public:
	CKDTabCtrl();
	virtual ~CKDTabCtrl();

	LPARAM GetCurItemLParam();
};
