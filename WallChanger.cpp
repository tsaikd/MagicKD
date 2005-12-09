// WallChanger.cpp : 實作檔
//

#include "stdafx.h"
#include "MagicKD.h"
#include "WallChanger.h"


// CWallChanger 對話方塊

IMPLEMENT_DYNAMIC(CWallChanger, CDialog)
CWallChanger::CWallChanger(CWnd* pParent /*=NULL*/)
	: CDialog(CWallChanger::IDD, pParent), m_bInit(false), m_bEnableWallChanger(true), m_uWaitTime(0)
{
}

CWallChanger::~CWallChanger()
{
}

void CWallChanger::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_WALLCHANGERCHECK, m_EnableWallChanger);
	DDX_Control(pDX, IDC_WALLSTATIC1, m_Static1);
	DDX_Control(pDX, IDC_WALLCLASSLIST, m_ClassList);
	DDX_Control(pDX, IDC_WALLSTATIC2, m_Static2);
	DDX_Control(pDX, IDC_WALLDIRLIST, m_DirList);
	DDX_Control(pDX, IDC_WALLSTATIC3, m_Static3);
	DDX_Control(pDX, IDC_WALLENABLECLASS, m_EnableClass);
	DDX_Control(pDX, IDC_WALLSTATIC4, m_Static4);
	DDX_Control(pDX, IDC_WALLCHANGETIMEEDIT, m_WaitTime);
}


BEGIN_MESSAGE_MAP(CWallChanger, CDialog)
	ON_WM_MOVE()
	ON_WM_SIZE()
	ON_LBN_SELCHANGE(IDC_WALLCLASSLIST, OnLbnSelchangeWallclasslist)
	ON_BN_CLICKED(IDC_WALLCHANGERCHECK, OnBnClickedWallchangercheck)
END_MESSAGE_MAP()


// CWallChanger 訊息處理常式

void CWallChanger::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if (!m_bInit)
		return;

	CRect rcWin;
	CRect rcParWin;
	GetClientRect(rcParWin);

	// Reset Enable WallChanger Position
	rcWin.top = 0;
	rcWin.bottom = rcWin.top + 20;
	rcWin.left = 20;
	rcWin.right = rcWin.left + 110;
	m_EnableWallChanger.MoveWindow(rcWin);

	// Reset Wait Time String Position
	rcWin.top = 3;
	rcWin.bottom = rcWin.top + 20;
	rcWin.left = 160;
	rcWin.right = rcWin.left + 70;
	m_Static4.MoveWindow(rcWin);
	// Reset Wait Time Position
	rcWin.top = 0;
	rcWin.left = rcWin.right;
	rcWin.right = rcWin.left + 40;
	m_WaitTime.MoveWindow(rcWin);

	// Reset Class List String Position
	rcWin.top = 40;
	rcWin.bottom = rcWin.top + 20;
	rcWin.left = 20;
	rcWin.right = rcParWin.right/2 - 10;
	m_Static1.MoveWindow(rcWin);
	// Reset Class List Position
	rcWin.top = rcWin.bottom;
	rcWin.bottom = rcParWin.bottom/2 - 30 + rcWin.top/2;
	rcWin.right = rcParWin.right/2 - 10;
	m_EnableClass.MoveWindow(rcWin);

	// Reset Directory List String Position
	rcWin.top = rcWin.bottom + 10;
	rcWin.bottom = rcWin.top + 20;
	rcWin.left = 20;
	rcWin.right = rcParWin.right/2 - 10;
	m_Static2.MoveWindow(rcWin);
	// Reset Directory List Position
	rcWin.top = rcWin.bottom;
	rcWin.bottom = rcParWin.bottom - 10;
	rcWin.right = rcParWin.right/2 - 10;
	m_ClassList.MoveWindow(rcWin);

	// Reset File List String Position
	rcWin.top = 40;
	rcWin.bottom = rcWin.top + 20;
	rcWin.left = rcParWin.right/2 + 10;
	rcWin.right = rcParWin.right - 20;
	m_Static3.MoveWindow(rcWin);
	// Reset File List Position
	rcWin.top = rcWin.bottom;
	rcWin.bottom = rcParWin.bottom - 10;
	rcWin.right = rcParWin.right - 10;
	m_DirList.MoveWindow(rcWin);

}

BOOL CWallChanger::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_ClassList.Init(this);
	m_DirList.Init(this);
	m_EnableClass.Init(this);

	m_EnableWallChanger.SetCheck(1);
	m_WaitTime.SetWindowText(_T("30"));
	m_uWaitTime = 30;
	m_ClassList.InsertString(0, _T("Default0"));
	m_ClassList.InsertString(1, _T("Default1"));
	ChangeList(0);

	CreateThread();
	m_bInit = true;
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX 屬性頁應傳回 FALSE
}

void CWallChanger::OnLbnSelchangeWallclasslist()
{
	ChangeList(m_ClassList.GetCurSel());
}

bool CWallChanger::ChangeList(int iListID)
{
	switch (iListID) {
	case 0:
		{
			m_DirList.DeleteAllString();
			m_DirList.InsertString(0, _T("A0"));
			m_DirList.InsertString(1, _T("A1"));
		}
		break;
	case 1:
		{
			m_DirList.DeleteAllString();
			m_DirList.InsertString(0, _T("B0"));
			m_DirList.InsertString(1, _T("B1"));
		}
		break;
	default:
		break;
	}
	return true;
}

void CWallChanger::OnBnClickedWallchangercheck()
{
	m_bEnableWallChanger = m_EnableWallChanger.GetCheck() ? true : false;
	CreateThread();
}

DWORD CWallChanger::ThreadProc()
{
	CString timer;
	int iBuf;
	while (m_bEnableWallChanger) {
		Sleep(1000);
		m_WaitTime.GetWindowText(timer);
		iBuf = atoi(timer) - 1;
		if (iBuf < 0)
			iBuf = m_uWaitTime;
		timer.Format("%d", iBuf);
		m_WaitTime.SetWindowText(timer);
	}
	return 0;
}

bool CWallChanger::AddEnableClass()
{
	CString sBuf;
	int iCurSel = m_ClassList.GetCurSel();
	if (iCurSel == LB_ERR)
		return false;
	m_ClassList.GetText(iCurSel, sBuf);
	if (m_EnableClass.FindString(0, sBuf) == LB_ERR)
		m_EnableClass.AddString(sBuf);
	return true;
}

bool CWallChanger::DelEnableClass()
{
	int iCurSel = m_EnableClass.GetCurSel();
	if (iCurSel == LB_ERR)
		return false;
	return (m_EnableClass.DeleteString(iCurSel) != LB_ERR);
}

