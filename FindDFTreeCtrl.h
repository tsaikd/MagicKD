#pragma once
#include "afxcmn.h"

class CFindDFTreeCtrl : public CTreeCtrl
{
	DECLARE_MESSAGE_MAP()
public:
	CFindDFTreeCtrl();
	virtual ~CFindDFTreeCtrl();

	HTREEITEM m_hContextItem;

protected:
	CMenu m_mContextMenu;

public:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};
