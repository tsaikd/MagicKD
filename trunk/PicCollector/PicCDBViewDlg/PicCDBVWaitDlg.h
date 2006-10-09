#pragma once
#include <afxmt.h>
#include "KDSimpleDlg.h"

class CPicCDBVWaitDlg : public CKDSimpleDlg
{
	DECLARE_DYNAMIC(CPicCDBVWaitDlg)
	DECLARE_MESSAGE_MAP()
public:
	CPicCDBVWaitDlg(CWnd* pParent = NULL);
	virtual ~CPicCDBVWaitDlg();

	bool Create(CWnd *pParent = NULL);
	void Display(LPCTSTR lpText, bool *pbStop);

private:
	bool *m_pbStop;
	CStatic m_static_Text;
	CString m_sDisplayText;
	CString m_sAppendText;

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};
