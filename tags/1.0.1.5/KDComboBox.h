#pragma once
#include "afxwin.h"

class CKDComboBox :
	public CComboBox
{
public:
	CKDComboBox();
	virtual ~CKDComboBox();

	bool EnableToolTips(LPCTSTR sToolTip = NULL, bool bEnable = true);
	bool IsEnableToolTips();
	void SetToolTips(LPCTSTR sToolTip);
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;

private:
	bool m_bEnableToolTip;
	CString m_sToolTip;
};
