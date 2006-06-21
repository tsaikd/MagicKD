// InputBox.cpp : implementation file
//

#include "stdafx.h"
#include "InputBox.h"
#include <afxpriv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputBox
#define IDC_EDIT_INPUT		11476
#define IDC_MSGBOX_STATIC	65535
// some metrics
#define CAPTION_HEIGHT	GetSystemMetrics(SM_CYCAPTION)
#define BORDER_HEIGHT	GetSystemMetrics(SM_CYEDGE)
#define BORDER_WIDTH 	GetSystemMetrics(SM_CXEDGE)
#define MARGIN			7

//
// Move horizontally all the buttons of a dialog
//
BOOL CALLBACK MoveButtonsHorz(HWND hwnd, LPARAM lParam)
{
    INT nOffSet = *(INT*)lParam;
	
    TCHAR szBuf[256];
    GetClassName(hwnd, szBuf, 256);
    if (lstrcmp(szBuf, _T("Button")) == 0)
    {
        // get the window placement (position and size)
        WINDOWPLACEMENT placement;
        ::GetWindowPlacement(hwnd, &placement);
        CRect rc = placement.rcNormalPosition;	
        ::SetWindowPos(hwnd, NULL, 
			rc.left+nOffSet/2, 
			rc.top, 
			rc.Width(), rc.Height(), SWP_NOZORDER);
    }
    return TRUE;
}

//
// Move vertically all the buttons of a dialog
//
BOOL CALLBACK MoveButtonsVert(HWND hwnd, LPARAM lParam)
{
    INT newTopPos = *(INT*)lParam;
	
    TCHAR szBuf[256];
    GetClassName(hwnd, szBuf, 256);
    if (lstrcmp(szBuf, _T("Button")) == 0)
    {
        // get the window placement (position and size)
        WINDOWPLACEMENT placement;
        ::GetWindowPlacement(hwnd, &placement);
        CRect rc = placement.rcNormalPosition;	
        ::SetWindowPos(hwnd, NULL, 
			rc.left, 
			newTopPos, 
			rc.Width(), rc.Height(), SWP_NOZORDER);
    }
    return TRUE;
}

CInputBox::CInputBox(IN CWnd* pParent, IN bool bMultiLine/* = false*/)
	: m_dwEditFlag(0), m_iAddDlgWidth(0)
{
	m_pParent = pParent;

	m_dwEditFlag = WS_VISIBLE | WS_CHILD | WS_TABSTOP | ES_WANTRETURN | ES_AUTOHSCROLL;
	if (bMultiLine)
		m_dwEditFlag |= WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL;
}

CInputBox::~CInputBox()
{
}


BEGIN_MESSAGE_MAP(CInputBox, CWnd)
	//{{AFX_MSG_MAP(CInputBox)
	ON_WM_CREATE()
	ON_MESSAGE(WM_INITDIALOG, OnInitMsgBox)
	ON_EN_KILLFOCUS(IDC_EDIT_INPUT, OnKillfocusEditInput)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CInputBox message handlers
INT CInputBox::Show(IN LPCTSTR lpszText, OUT CString& strUserInput, IN UINT nIDHelp)
{
    AfxHookWindowCreate(this);
	m_pstrUserInput = &strUserInput;
    return AfxMessageBox(lpszText, MB_OKCANCEL|MB_ICONQUESTION, nIDHelp);
}

int CInputBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    AfxUnhookWindowCreate();
	return CWnd::OnCreate(lpCreateStruct);
}

LRESULT CInputBox::OnInitMsgBox(WPARAM wParam, LPARAM lParam)
{
	UNUSED_ALWAYS(wParam);
	UNUSED_ALWAYS(lParam);

	LRESULT lRes = Default();

	// TODO: Add extra initialization here
	CRect rcDlg, rcStatic;
	HFONT hFont = NULL;

	GetDlgRect(rcDlg);
	if (rcDlg.Width() < 300) {
		m_iAddDlgWidth = 300 - rcDlg.Width();
		rcDlg.right = rcDlg.left + 300;
	}
	if (rcDlg.Height() < 125)
		rcDlg.bottom = rcDlg.top + 125;
	GetStaticFontNRect(rcStatic, hFont);
	if (rcStatic.Width() < (rcDlg.Width()-30))
		rcStatic.right = rcDlg.Width() - 30;

	CenterMe(rcDlg);
	CRect rcEdit = CreateEdit(rcDlg, rcStatic, hFont);

	CenterButtons(rcEdit);	 	
	return lRes;
}

BOOL CInputBox::CenterMe(CRect& rect)
{
	MoveWindow(rect, FALSE);
	CenterWindow(m_pParent);

	return TRUE;
}

BOOL CInputBox::CenterButtons(CRect& rcEdit)
{
	CRect rcOldDlg;
	GetDlgRect(rcOldDlg);
	int offset = 10 + (m_iAddDlgWidth);
	EnumChildWindows(GetSafeHwnd(), MoveButtonsHorz, (LPARAM) &offset);

	offset = rcEdit.bottom + 7;
    EnumChildWindows(GetSafeHwnd(), MoveButtonsVert, (LPARAM) &offset);

	return TRUE;
}

CRect CInputBox::CreateEdit(CRect& rcDlg, CRect& rcStatic, HFONT& hFont)
{
	int nHeight;
	if (m_dwEditFlag & ES_MULTILINE)
		nHeight = rcDlg.Height() - rcStatic.Height() - CAPTION_HEIGHT - 65;
	else
		nHeight = 14;
	CRect rcEdit(rcStatic.left, 
				rcStatic.bottom+2,
				rcStatic.right, 
				rcStatic.bottom+2+nHeight);
	//Add ES_NUMBER if you want to limit the edit for numeric input
	if (!m_cEdit.Create(m_dwEditFlag, rcEdit, this, IDC_EDIT_INPUT))
		return FALSE;
    m_cEdit.SetFont(CFont::FromHandle(hFont));
	m_cEdit.SetFocus();
	return rcEdit;
}

BOOL CInputBox::GetStaticFontNRect(CRect& rcStatic, HFONT& hFont)
{
	// get standard font (from static)
    HWND hStatic = ::GetDlgItem(GetSafeHwnd(), IDC_MSGBOX_STATIC);
	
	hFont = (HFONT) ::SendMessage(hStatic, WM_GETFONT, 0, 0);
	
	// get the window placement (position and size)
    WINDOWPLACEMENT placement;
    //get the placement of Static
	::GetWindowPlacement(hStatic, &placement);
	rcStatic = placement.rcNormalPosition;

	return TRUE;
}

BOOL CInputBox::GetDlgRect(CRect& rcDlg)
{
	WINDOWPLACEMENT placement;
	GetWindowPlacement(&placement);
    rcDlg = placement.rcNormalPosition;
	rcDlg.bottom -= MARGIN;
	return TRUE;
}

void CInputBox::OnKillfocusEditInput()
{
	CString strUserInput;
	m_cEdit.GetWindowText(strUserInput);
	*m_pstrUserInput = strUserInput;
}
