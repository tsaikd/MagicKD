#pragma once
#include "KDTabCtrl.h"

class CMagicKDTabCtrl : public CKDTabCtrl
{
	DECLARE_MESSAGE_MAP()
public:
	CMagicKDTabCtrl();
	virtual ~CMagicKDTabCtrl();

	int SetCurSel(int nItem);

protected:
	CMenu m_mContextMenu;

public:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);

protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
