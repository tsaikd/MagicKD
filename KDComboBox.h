#pragma once
#include "afxwin.h"

class CKDComboBox :
	public CComboBox
{
public:
	CKDComboBox();
	virtual ~CKDComboBox();

	BOOL EnableToolTips(LPCTSTR sToolTip = NULL, BOOL bEnable = TRUE);
	BOOL IsEnableToolTips();
	void SetToolTips(LPCTSTR sToolTip);
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;

private:
	BOOL m_bEnableToolTip;
	CString m_sToolTip;
};
