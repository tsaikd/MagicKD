#pragma once
#include "afxwin.h"

class CKDEdit :
	public CEdit
{
public:
	CKDEdit();
	virtual ~CKDEdit();

	bool EnableToolTips(LPCTSTR sToolTip = NULL, bool bEnable = true);
	bool IsEnableToolTips();
	void SetToolTips(LPCTSTR sToolTip);
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;

private:
	bool m_bEnableToolTip;
	CString m_sToolTip;
};
