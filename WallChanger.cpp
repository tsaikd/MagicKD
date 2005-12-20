// WallChanger.cpp : 實作檔
//

#include "StdAfx.h"
#include "MagicKDDlg.h"
#include "WallChanger.h"

#define DEFAULT_WAIT_TIME_PERWALLPAPER 30

// CWallChanger 對話方塊

IMPLEMENT_DYNAMIC(CWallChanger, CDialog)
CWallChanger::CWallChanger(CWnd* pParent /*=NULL*/) : CDialog(CWallChanger::IDD, pParent), m_bInit(false),
	m_bEnableWallChanger(true), m_uWaitTime(0), m_hThread(NULL), m_bIsThreadRunning(false), m_pCurDirList(NULL)
{
}

CWallChanger::~CWallChanger()
{
}

void CWallChanger::DoOnSize()
{
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
//		m_DirList.MoveWindow(rcWin);
		m_pCurDirList->MoveWindow(rcWin);
	}

	Invalidate();
}

DWORD CWallChanger::ThreadProc()
{
	if (m_bIsThreadRunning)
		return 0;
	m_bIsThreadRunning = true;
	CString timer;
	int iBuf;
	while (m_bEnableWallChanger) {
		Sleep(1000);
		if (!m_bEnableWallChanger)
			break;
		m_WaitTime.GetWindowText(timer);
		iBuf = atoi(timer) - 1;
		if (iBuf < 0)
			iBuf = m_uWaitTime;
		timer.Format("%d", iBuf);
		m_WaitTime.SetWindowText(timer);
	}
	m_bIsThreadRunning = false;
	return 0;
}

bool CWallChanger::SetSelClassItemToDirList(CWallListCtrl *pClassList, CWallDirListCtrl *pDirList)
{
	if (!pClassList || !pDirList)
		return false;
	int iItem;
	POSITION pos = pClassList->GetFirstSelectedItemPosition();
	pDirList->DeleteAllItems();
	while (pos) {
		{
			int nItem = PosToInt(pos);
			CString sPathBuf;
			switch (nItem) {
			case 0:
				{
					pDirList->AddItem(nItem, _T("C:\\A0"), 0);
					pDirList->AddItem(nItem, _T("C:\\A1"), 10);
					pDirList->AddItem(nItem, _T("C:\\A2"), 100);
					pDirList->AddItem(nItem, _T("C:\\A3"), 1000);
				}
				break;
			case 1:
				{
					pDirList->AddItem(nItem, _T("C:\\B0"), 0);
					pDirList->AddItem(nItem, _T("C:\\B1"), 10);
					pDirList->AddItem(nItem, _T("C:\\B2"), 100);
					pDirList->AddItem(nItem, _T("C:\\B3"), 1000);
				}
				break;
			case 2:
				{
					pDirList->AddItem(nItem, _T("C:\\C0"), 0);
					pDirList->AddItem(nItem, _T("C:\\C1"), 10);
					pDirList->AddItem(nItem, _T("C:\\C2"), 100);
					pDirList->AddItem(nItem, _T("C:\\C3"), 1000);
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
	CString sClassName;
	LVFINDINFO findInfo = {0};
	findInfo.flags = LVFI_STRING;
	POSITION pos = m_ClassList.GetFirstSelectedItemPosition();
	CWallClassListItem *pItem = NULL;
	void *pChildList = NULL;
	if (pos) {
		pItem = (CWallClassListItem*) m_ClassList.GetFirstSelectedItemLParam();
		pChildList = (void*)pItem->GetChildDirList();
		sClassName = m_ClassList.GetItemText(PosToInt(pos), 0);
		findInfo.psz = sClassName;
		if (m_EnableClass.FindItem(&findInfo) == -1)
			m_EnableClass.AddItem(sClassName, pChildList);
//		iItem = m_ClassList.GetNextSelectedItem(pos);
	}
	Invalidate();
}

void CWallChanger::DelEnableClass()
{
	m_EnableClass.DeleteSelectItem();
	Invalidate();
}

void CWallChanger::NewClassList(LPCTSTR sClassName/* = NULL*/)
{
	CString sBuf;
	if (sClassName)
		sBuf = sClassName;
	else if (!m_Edit_NewClass.GetWindowTextLength())
		return;
	else
		m_Edit_NewClass.GetWindowText(sBuf);
	m_ClassList.AddItem(sBuf);
	m_Edit_NewClass.SetWindowText(_T(""));
	Invalidate();
}

void CWallChanger::DelClassList()
{
	int iItem;
	CString sClassName;
	LVFINDINFO findInfo = {0};
	findInfo.flags = LVFI_STRING;
	POSITION pos = m_ClassList.GetFirstSelectedItemPosition();
	void *pChildList;
	while (pos) {
		pChildList = (void*)m_ClassList.GetItemData(PosToInt(pos));
		sClassName = m_ClassList.GetItemText(PosToInt(pos), 0);
		findInfo.psz = sClassName;
		iItem = m_EnableClass.FindItem(&findInfo);
		if (iItem != -1)
			m_EnableClass.DeleteItem(iItem);
		iItem = m_ClassList.GetNextSelectedItem(pos);
	}
	m_ClassList.DeleteSelectItem();
	Invalidate();
}

void CWallChanger::AddClassDir()
{
	if (m_ClassList.GetSelectedCount() == 1) {
		TCHAR sPath[MAX_PATH] = {0};
		if (ChooseFolder(sPath, m_hWnd)) {
			POSITION pos = m_ClassList.GetFirstSelectedItemPosition();
			m_pCurDirList->AddItem(PosToInt(pos), sPath, 0);
		}
	} else if (m_ClassList.GetItemCount() == 0) {
		MessageBox(CResString(IDS_WALLERR_HAVETONEWCLASS));
	} else {
		MessageBox(CResString(IDS_WALLERR_ONLYCANSELECTONECLASS));
	}
	Invalidate();
}

void CWallChanger::DelClassDir()
{
	m_DirList.DeleteSelectItem();
	Invalidate();
}

void CWallChanger::OnBnClickedBtnNewclass()
{
	NewClassList();
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
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CWallChanger 訊息處理常式

BOOL CWallChanger::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_EnableClass.Init(this);
	m_ClassList.Init(this);
	m_DirList.Init(this);
	m_pCurDirList = &m_DirList;

	CString sBuf;
	sBuf.Format(_T("%d"), DEFAULT_WAIT_TIME_PERWALLPAPER);
	m_WaitTime.SetWindowText(sBuf);
	m_uWaitTime = DEFAULT_WAIT_TIME_PERWALLPAPER;
	NewClassList(CResString(IDS_WALL_DEFAULTCLASS));
	m_EnableWallChanger.SetCheck(1);
	m_bEnableWallChanger = true;
	CreateThread();

	m_bInit = true;
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX 屬性頁應傳回 FALSE
}

void CWallChanger::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	DoOnSize();
}

void CWallChanger::OnBnClickedWallchangercheck()
{
	m_bEnableWallChanger = m_EnableWallChanger.GetCheck() ? true : false;
	if (m_bEnableWallChanger)
		CreateThread();
}

void CWallChanger::OnLvnItemchangedWallenableclass(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	CWallEnableItem *pItem = (CWallEnableItem *) m_EnableClass.GetFirstSelectedItemLParam();
	if (!pItem)
		return;
	m_pCurDirList->ShowWindow(SW_HIDE);
	m_pCurDirList = pItem->GetChildDirList();
	m_pCurDirList->ShowWindow(SW_SHOW);
	DoOnSize();

	*pResult = 0;
}

void CWallChanger::OnNMSetfocusWallenableclass(NMHDR *pNMHDR, LRESULT *pResult)
{
	CWallEnableItem *pItem = (CWallEnableItem *) m_EnableClass.GetFirstSelectedItemLParam();
	if (!pItem)
		return;
	m_pCurDirList->ShowWindow(SW_HIDE);
	m_pCurDirList = pItem->GetChildDirList();
	m_pCurDirList->ShowWindow(SW_SHOW);
	DoOnSize();

	*pResult = 0;
}

void CWallChanger::OnLvnItemchangedWallclasslist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	CWallClassListItem *pItem = (CWallClassListItem *) m_ClassList.GetFirstSelectedItemLParam();
	if (!pItem)
		return;
	m_pCurDirList->ShowWindow(SW_HIDE);
	m_pCurDirList = pItem->GetChildDirList();
	m_pCurDirList->ShowWindow(SW_SHOW);
	DoOnSize();
	
	*pResult = 0;
}

void CWallChanger::OnNMSetfocusWallclasslist(NMHDR *pNMHDR, LRESULT *pResult)
{
	CWallClassListItem *pItem = (CWallClassListItem *) m_ClassList.GetFirstSelectedItemLParam();
	if (!pItem)
		return;
	m_pCurDirList->ShowWindow(SW_HIDE);
	m_pCurDirList = pItem->GetChildDirList();
	m_pCurDirList->ShowWindow(SW_SHOW);
	DoOnSize();

	*pResult = 0;
}

void CWallChanger::OnDestroy()
{
	CDialog::OnDestroy();

	if (m_bIsThreadRunning) {
		m_bEnableWallChanger = false;
		WaitForSingleObject(m_hThread, 2000);
	}
	if (m_hThread) {
		CloseHandle(m_hThread);
		m_hThread = NULL;
		m_dwThreadId = 0;
	}
}
