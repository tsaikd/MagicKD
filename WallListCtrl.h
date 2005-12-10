#pragma once
#include "Language.h"
#include "KDListCtrl.h"

// CWallListCtrl

class CWallListCtrl : public CKDListCtrl
{
	DECLARE_DYNAMIC(CWallListCtrl)

public:
	CWallListCtrl();
	virtual ~CWallListCtrl();

	void Init(void *pParent);

	void *m_pParent;

private:
	CMenu m_mContextMenu;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};


