#include "stdafx.h"
#include "Others.h"

#include "KDSimpleDlg.h"

IMPLEMENT_DYNAMIC(CKDSimpleDlg, CDialog)
CKDSimpleDlg::CKDSimpleDlg(CWnd *pParent/* = NULL*/)
	:	CDialog(CKDSimpleDlg::IDD, pParent), m_sizeWin(200, 100)
{
}

CKDSimpleDlg::~CKDSimpleDlg()
{
	if (m_pParentWnd && (GetActiveWindow() == this)) {
		GetParent()->Invalidate();
		GetParent()->SetActiveWindow();
	}
}

BOOL CKDSimpleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	/*
	// Example for creating CStatic object
	CRect rcWin(MARGIN_X, MARGIN_Y, MARGIN_X + BUTTONS_WIDTH, MARGIN_Y + BUTTONHEIGHT);
	MapDialogRect(&rcWin);

	CStatic *m_static_Text = new CStatic;
	m_static_Text->Create(_T("Show Text"), SS_LEFTNOWORDWRAP | SS_NOPREFIX | WS_CHILD, rcWin, this);
	m_static_Text->SetFont(&m_fontWin);
	m_static_Text->ShowWindow(SW_SHOW);
	*/

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

bool CKDSimpleDlg::Create(CWnd *pParent/* = NULL*/, DWORD dwStyle/* = DS_SETFONT|WS_POPUP|WS_BORDER|WS_CAPTION*/, DWORD dwExStyle/* = 0*/)
{
	bool bRes = false;
	if (pParent)
		m_pParentWnd = pParent;
	PreModal();
	NONCLIENTMETRICS ncm = {0};
	ncm.cbSize = sizeof(ncm);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, &ncm, 0);
	m_fontWin.CreateFontIndirect(&ncm.lfMessageFont);

	struct X {
		DLGTEMPLATE vDLGTEMPLATE;
		struct {
			WORD Menu;
			WORD Class;
			WORD Caption;
			WORD PointSize;
			char FaceName[4];
		} dbInfo;
	};
	X buffer = {0};
	X *pX = &buffer;

	pX->vDLGTEMPLATE.style = dwStyle;
	pX->vDLGTEMPLATE.dwExtendedStyle = dwExStyle;
	pX->vDLGTEMPLATE.cdit = 0;
	pX->vDLGTEMPLATE.x = 0;
	pX->vDLGTEMPLATE.y = 0;
	pX->vDLGTEMPLATE.cx = m_sizeWin.cx;
	pX->vDLGTEMPLATE.cy = m_sizeWin.cy;
	pX->dbInfo.PointSize = (WORD)ncm.lfMessageFont.lfHeight;
	VERIFY(bRes = (CreateIndirect(&pX->vDLGTEMPLATE, m_pParentWnd)) != FALSE);

	CenterWindow();
	ShowWindow(SW_SHOW);
	return bRes;
}

void CKDSimpleDlg::SetWindowSize(int cx, int cy)
{
	if (m_hWnd) {
		SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOREDRAW);
		CenterWindow();
	} else {
		m_sizeWin.SetSize(cx, cy);
	}
}

BEGIN_MESSAGE_MAP(CKDSimpleDlg, CDialog)
END_MESSAGE_MAP()
















