// WallChanger.cpp : 實作檔
//

#include "stdafx.h"
#include "MagicKD.h"
#include "WallChanger.h"
#include ".\wallchanger.h"


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
	DDX_Control(pDX, IDC_NEWCLASSNAME, m_Edit_NewClass);
	DDX_Control(pDX, IDC_BTN_NEWCLASS, m_BTN_NewClass);
}


BEGIN_MESSAGE_MAP(CWallChanger, CDialog)
	ON_WM_MOVE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_WALLCHANGERCHECK, OnBnClickedWallchangercheck)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_WALLCLASSLIST, OnLvnItemchangedWallclasslist)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_WALLENABLECLASS, OnLvnItemchangedWallenableclass)
	ON_BN_CLICKED(IDC_BTN_NEWCLASS, OnBnClickedBtnNewclass)
	ON_NOTIFY(NM_SETFOCUS, IDC_WALLENABLECLASS, OnNMSetfocusWallenableclass)
	ON_NOTIFY(NM_SETFOCUS, IDC_WALLCLASSLIST, OnNMSetfocusWallclasslist)
END_MESSAGE_MAP()


// CWallChanger 訊息處理常式

BOOL CWallChanger::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_EnableClass.Init(this);
	m_EnableClass.InsertColumn(0, _T(""));
	m_ClassList.Init(this);
	m_ClassList.InsertColumn(0, _T(""));
	m_DirList.Init(this);
	m_DirList.InsertColumn(0, _T(""));

	m_EnableWallChanger.SetCheck(1);
	m_WaitTime.SetWindowText(_T("30"));
	m_uWaitTime = 30;
	m_ClassList.InsertItem(0, _T("Default0"));
	m_ClassList.InsertItem(1, _T("Default1"));
	m_ClassList.InsertItem(2, _T("Default2"));

	CreateThread();
	m_bInit = true;
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX 屬性頁應傳回 FALSE
}

void CWallChanger::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if (!m_bInit)
		return;

	CRect rcWin;
	CRect rcGrpWin;
	CRect rcParWin;
	GetClientRect(rcParWin);

	rcGrpWin.top = 0;
	rcGrpWin.bottom = 40;
	rcGrpWin.left = 20;
	rcGrpWin.right = 260;
	{
		// Reset Enable WallChanger Position
		rcWin.top = rcGrpWin.top;
		rcWin.bottom = rcWin.top + 20;
		rcWin.left = rcGrpWin.left;
		rcWin.right = rcWin.left + 120;
		m_EnableWallChanger.MoveWindow(rcWin);

		// Reset Wait Time String Position
		rcWin.top = rcGrpWin.top + 3;
		rcWin.bottom = rcWin.top + 20;
		rcWin.left = rcWin.right + 10;
		rcWin.right = rcWin.left + 70;
		m_Static4.MoveWindow(rcWin);
		// Reset Wait Time Position
		rcWin.top = rcGrpWin.top;
		rcWin.left = rcWin.right;
		rcWin.right = rcGrpWin.right;
		m_WaitTime.MoveWindow(rcWin);
	}

	rcGrpWin.top = 40;
	rcGrpWin.bottom = rcParWin.bottom - 10;
	rcGrpWin.left = 20;
	rcGrpWin.right = rcParWin.right/2 - 10;
	{
		// Reset Enable Class List String Position
		rcWin.top = rcGrpWin.top;
		rcWin.bottom = rcWin.top + 20;
		rcWin.left = rcGrpWin.left;
		rcWin.right = rcGrpWin.right;
		m_Static1.MoveWindow(rcWin);
		// Reset Enable Class List Position
		rcWin.top = rcWin.bottom;
		rcWin.bottom = rcGrpWin.bottom/2 - 10;
		m_EnableClass.MoveWindow(rcWin);
		m_EnableClass.SetColumnWidth(0, rcWin.right-rcWin.left-4);

		// Reset Class List String Position
		rcWin.top = rcWin.bottom + 20;
		rcWin.bottom = rcWin.top + 20;
		m_Static2.MoveWindow(rcWin);
		// Reset Class List Position
		rcWin.top = rcWin.bottom;
		rcWin.bottom = rcGrpWin.bottom - 40;
		m_ClassList.MoveWindow(rcWin);
		m_ClassList.SetColumnWidth(0, rcWin.right-rcWin.left-4);
		// Reset New Class Edit Position
		rcWin.top = rcWin.bottom + 10;
		rcWin.bottom = rcGrpWin.bottom - 10;
		rcWin.right = rcGrpWin.right - 80;
		m_Edit_NewClass.MoveWindow(rcWin);
		// Reset New Class Botton Position
		rcWin.left = rcWin.right;
		rcWin.right = rcGrpWin.right;
		m_BTN_NewClass.MoveWindow(rcWin);
	}

	rcGrpWin.top = 40;
	rcGrpWin.bottom = rcParWin.bottom - 10;
	rcGrpWin.left = rcGrpWin.right + 20;
	rcGrpWin.right = rcParWin.right - 10;
	{
		// Reset Directory List String Position
		rcWin.top = rcGrpWin.top;
		rcWin.bottom = rcWin.top + 20;
		rcWin.left = rcGrpWin.left;
		rcWin.right = rcGrpWin.right;
		m_Static3.MoveWindow(rcWin);
		// Reset Directory List Position
		rcWin.top = rcWin.bottom;
		rcWin.bottom = rcGrpWin.bottom;
		m_DirList.MoveWindow(rcWin);
		m_DirList.SetColumnWidth(0, rcWin.right-rcWin.left-4);
	}

	Invalidate();
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

void CWallChanger::OnLvnItemchangedWallenableclass(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	SetSelClassItemToDirList(&m_EnableClass, &m_DirList);
	*pResult = 0;
}

void CWallChanger::OnLvnItemchangedWallclasslist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	SetSelClassItemToDirList(&m_ClassList, &m_DirList);
	*pResult = 0;
}

void CWallChanger::OnNMSetfocusWallenableclass(NMHDR *pNMHDR, LRESULT *pResult)
{
	POSITION pos = m_EnableClass.GetFirstSelectedItemPosition();
	if (!pos)
		m_DirList.DeleteAllItems();
	else
		SetSelClassItemToDirList(&m_EnableClass, &m_DirList);
	*pResult = 0;
}

void CWallChanger::OnNMSetfocusWallclasslist(NMHDR *pNMHDR, LRESULT *pResult)
{
	POSITION pos = m_ClassList.GetFirstSelectedItemPosition();
	if (!pos)
		m_DirList.DeleteAllItems();
	else
		SetSelClassItemToDirList(&m_ClassList, &m_DirList);
	*pResult = 0;
}

bool CWallChanger::SetSelClassItemToDirList(CWallListCtrl *pClassList, CWallListCtrl *pDirList)
{
	if (!pClassList || !pDirList)
		return false;
	int iItem;
	POSITION pos = pClassList->GetFirstSelectedItemPosition();
	pDirList->DeleteAllItems();
	while (pos) {
		{
			int nItem = PosToInt(pos);
			switch (nItem) {
			case 0:
				{
					pDirList->InsertItem(0, _T("A0"));
					pDirList->InsertItem(1, _T("A1"));
					pDirList->InsertItem(2, _T("A2"));
					pDirList->InsertItem(3, _T("A3"));
				}
				break;
			case 1:
				{
					pDirList->InsertItem(0, _T("B0"));
					pDirList->InsertItem(1, _T("B1"));
					pDirList->InsertItem(2, _T("B2"));
					pDirList->InsertItem(3, _T("B3"));
				}
				break;
			case 2:
				{
					pDirList->InsertItem(0, _T("C0"));
					pDirList->InsertItem(1, _T("C1"));
					pDirList->InsertItem(2, _T("C2"));
					pDirList->InsertItem(3, _T("C3"));
				}
				break;
			}
		}
		iItem = pClassList->GetNextSelectedItem(pos);
	}

	return true;
}

void CWallChanger::AddClassToEnable()
{
	int iItem;
	CString sClassName;
	LVFINDINFO findInfo = {0};
	findInfo.flags = LVFI_STRING;
	POSITION pos = m_ClassList.GetFirstSelectedItemPosition();
	while (pos) {
		sClassName = m_ClassList.GetItemText(PosToInt(pos), 0);
		findInfo.psz = sClassName;
		if (m_EnableClass.FindItem(&findInfo) == -1)
			m_EnableClass.InsertItem(m_EnableClass.GetItemCount(), sClassName);
		iItem = m_ClassList.GetNextSelectedItem(pos);
	}
	m_EnableClass.SetColumnWidth(0, LVSCW_AUTOSIZE);
}

void CWallChanger::DelEnableClass()
{
	m_EnableClass.DeleteSelectItem();
	m_EnableClass.SetColumnWidth(0, LVSCW_AUTOSIZE);
}

void CWallChanger::NewClassList()
{
	CString sBuf;
	if (!m_Edit_NewClass.GetWindowTextLength())
		return;
	m_Edit_NewClass.GetWindowText(sBuf);
	m_ClassList.InsertItem(-1, sBuf);
	m_ClassList.SetColumnWidth(0, LVSCW_AUTOSIZE);
}

void CWallChanger::DelClassList()
{
	m_ClassList.DeleteSelectItem();
	m_ClassList.SetColumnWidth(0, LVSCW_AUTOSIZE);
}

void CWallChanger::AddClassDir()
{
	TCHAR sPath[MAX_PATH] = {0};
	if (ChooseFolder(sPath, m_hWnd))
		m_DirList.InsertItem(-1, sPath);
	m_DirList.SetColumnWidth(0, LVSCW_AUTOSIZE);
}

void CWallChanger::DelClassDir()
{
	m_DirList.DeleteSelectItem();
	m_DirList.SetColumnWidth(0, LVSCW_AUTOSIZE);
}

void CWallChanger::OnBnClickedBtnNewclass()
{
	NewClassList();
}
