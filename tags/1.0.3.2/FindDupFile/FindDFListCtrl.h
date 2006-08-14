#pragma once
#include "KDListCtrl.h"

class CFindDFListCtrl : public CKDListCtrl
{
	DECLARE_MESSAGE_MAP()
public:
	CFindDFListCtrl();
	virtual ~CFindDFListCtrl();
	void Init();

	void Localize();

protected:
	CMenu m_mContextMenu;

private:
	bool m_bInit;

public:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnDropFiles(HDROP hDropInfo);
};
