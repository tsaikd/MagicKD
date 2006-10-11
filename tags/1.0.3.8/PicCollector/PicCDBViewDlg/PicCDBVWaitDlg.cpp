#include "stdafx.h"
#include "Others.h"

#include "PicCDBVWaitDlg.h"

enum {
	KDT_NULL,
	KDT_DESTROY
};

IMPLEMENT_DYNAMIC(CPicCDBVWaitDlg, CKDSimpleDlg)
CPicCDBVWaitDlg::CPicCDBVWaitDlg(CWnd* pParent/* = NULL*/)
	:	CKDSimpleDlg(pParent), m_pbStop(NULL)
{
}

CPicCDBVWaitDlg::~CPicCDBVWaitDlg()
{
}

BOOL CPicCDBVWaitDlg::OnInitDialog()
{
	CKDSimpleDlg::OnInitDialog();

	SetWindowSize(100, 50);

	m_static_Text.Create(m_sDisplayText, SS_LEFTNOWORDWRAP | SS_NOPREFIX | WS_CHILD, CRect(), this);
	m_static_Text.SetFont(&m_fontWin);

	CSize szStr = GetStrPixelSize(&m_static_Text, m_sDisplayText);
	m_static_Text.MoveWindow(0, 0, szStr.cx, szStr.cy, FALSE);
	m_static_Text.CenterWindow();
	m_static_Text.ShowWindow(SW_SHOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

bool CPicCDBVWaitDlg::Create(CWnd *pParent/* = NULL*/)
{
	return CKDSimpleDlg::Create(pParent, DS_SETFONT|WS_POPUP|WS_BORDER);
}

void CPicCDBVWaitDlg::Display(LPCTSTR lpText, bool *pbStop)
{
	m_sDisplayText = lpText;
	m_sDisplayText.Append(_T("\n "));
	m_pbStop = pbStop;

	if (m_hWnd) {
		m_static_Text.SetWindowText(m_sDisplayText);

		CSize szStr = GetStrPixelSize(&m_static_Text, m_sDisplayText);
		m_static_Text.MoveWindow(0, 0, szStr.cx, szStr.cy, FALSE);
		m_static_Text.CenterWindow();
	} else {
		Create();
	}
	ShowWindow(SW_SHOW);
	Invalidate();
	SetTimer(KDT_DESTROY, 500, NULL);
}

BEGIN_MESSAGE_MAP(CPicCDBVWaitDlg, CKDSimpleDlg)
	ON_WM_TIMER()
	ON_WM_SETFOCUS()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()

void CPicCDBVWaitDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent) {
	case KDT_DESTROY:
		if (*m_pbStop) {
			KillTimer(nIDEvent);
			ShowWindow(SW_HIDE);
			m_sAppendText.Empty();
		} else {
			m_sAppendText.AppendChar(_T('.'));
			if (GetStrPixelSize(&m_static_Text, m_sAppendText).cx > GetStrPixelSize(&m_static_Text, m_sDisplayText).cx)
				m_sAppendText.Empty();
			m_static_Text.SetWindowText(m_sDisplayText + m_sAppendText);
		}
		break;
	}

	CKDSimpleDlg::OnTimer(nIDEvent);
}

void CPicCDBVWaitDlg::OnSetFocus(CWnd* pOldWnd)
{
	CKDSimpleDlg::OnSetFocus(pOldWnd);

	if (GetParent())
		GetParent()->SetFocus();
}

void CPicCDBVWaitDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CKDSimpleDlg::OnActivate(nState, pWndOther, bMinimized);

	if (GetParent())
		GetParent()->SetActiveWindow();
}
