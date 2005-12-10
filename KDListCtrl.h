#pragma once
#include "afxcmn.h"

class CKDListCtrl :
	public CListCtrl
{
public:
	CKDListCtrl();
	virtual ~CKDListCtrl();

	void DeleteSelectItem();
};
