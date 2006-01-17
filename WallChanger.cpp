// WallChanger.cpp : 實作檔
//

#include "StdAfx.h"
#include "MagicKDDlg.h"
#include "WallChanger.h"
#include "WallDirListCtrl.h"

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

void CWallChanger::SetEnableWallChanger(bool bEnable)
{
	m_bEnableWallChanger = bEnable;
	if (bEnable) {
		m_EnableWallChanger.SetCheck(BST_CHECKED);
		CreateThread();
	} else {
		m_EnableWallChanger.SetCheck(BST_UNCHECKED);
	}
	((CMagicKDDlg*)GetParent())->MainConfigSyncTabEnable();
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

void CWallChanger::AddClassToEnable()
{
	CString sClassName;
	LVFINDINFO findInfo = {0};
	findInfo.flags = LVFI_STRING;
	POSITION pos = m_ClassList.GetFirstSelectedItemPosition();

	if (pos) {
		int nItem = m_ClassList.GetNextSelectedItem(pos);
		CWallClassListItem *pItem = (CWallClassListItem*) m_ClassList.GetFirstSelectedItemLParam();
		sClassName = m_ClassList.GetItemText(nItem, 0);
		findInfo.psz = sClassName;
		void *pChildList = pItem->GetChildDirList();
		if (m_EnableClass.FindItem(&findInfo) == -1) {
			m_EnableClass.AddItem(sClassName, pChildList);
			m_EnableClass.SetModified();
		}
	}
	SyncListSel(&m_ClassList, &m_EnableClass);
	Invalidate();
}

void CWallChanger::DelEnableClass()
{
	m_EnableClass.DeleteSelectItem();
	m_EnableClass.SetModified();
	Invalidate();
}

void CWallChanger::NewClassList(LPCTSTR sClassName/* = NULL*/)
{
	CString sBuf, sPos;

	if (sClassName)
		sBuf = sClassName;
	else if (!m_Edit_NewClass.GetWindowTextLength())
		return;
	else
		m_Edit_NewClass.GetWindowText(sBuf);

	sPos.Format(_T("%d"), m_ClassList.GetItemCount());
	m_ClassList.AddItem(sBuf);
	m_ClassList.SetModified();
	m_cIni.WriteString(_T("ClassList"), sPos, sBuf);
	m_Edit_NewClass.SetWindowText(_T(""));

	Invalidate();
}

void CWallChanger::DelClassList()
{
	int nItem;
	CString sClassName;
	LVFINDINFO findInfo = {0};
	findInfo.flags = LVFI_STRING;
	POSITION pos = m_ClassList.GetFirstSelectedItemPosition();

	if (pos) {
		m_ClassList.SetModified();
		CWallDirListCtrl *pChildDirList = ((CWallClassListItem *)m_ClassList.GetFirstSelectedItemLParam())->GetChildDirList();
		pChildDirList->DeleteAllItems();
		pChildDirList->SetModified();
	}
	while (pos) {
		nItem = m_ClassList.GetNextSelectedItem(pos);
		sClassName = m_ClassList.GetItemText(nItem, 0);
		findInfo.psz = sClassName;
		m_ClassList.DeleteItem(nItem);

		nItem = m_EnableClass.FindItem(&findInfo);
		if (nItem != -1)
			m_EnableClass.DeleteItem(nItem);
	}

	m_pCurDirList = &m_DirList;
	m_pCurDirList->ShowWindow(SW_SHOW);

	DoOnSize();
}

void CWallChanger::AddClassDir()
{
	if (m_ClassList.GetSelectedCount() == 1) {
		TCHAR sPath[MAX_PATH] = {0};
		if (ChooseFolder(sPath, m_hWnd)) {
			POSITION pos = m_ClassList.GetFirstSelectedItemPosition();
			int nItem = m_ClassList.GetNextSelectedItem(pos);
			m_pCurDirList->AddItem(nItem, sPath, 0);
			m_pCurDirList->SetModified();
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
	m_pCurDirList->DeleteSelectItem();
	m_pCurDirList->SetModified();
	Invalidate();
}

bool CWallChanger::SetDirList(CWallDirListCtrl *pDirList, LPCTSTR sDirListName)
{
	if (!pDirList || !sDirListName)
		return false;
	m_Static3.SetWindowText(sDirListName);
	if (m_pCurDirList)
		m_pCurDirList->ShowWindow(SW_HIDE);
	m_pCurDirList = pDirList;
	m_pCurDirList->ShowWindow(SW_SHOW);
	DoOnSize();
	return true;
}

void CWallChanger::SyncListSel(CWallListCtrl *pMasterList, CWallListCtrl *pSlaveList)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_STRING;
	int nItem;
	POSITION pos = pMasterList->GetFirstSelectedItemPosition();
	CString sBuf;

	while (pos) {
		nItem = pMasterList->GetNextSelectedItem(pos);
		sBuf = pMasterList->GetItemText(nItem, 0);
		FindInfo.psz = sBuf;
		nItem = pSlaveList->FindItem(&FindInfo);
		if ((nItem != -1) && (nItem != PosToInt(pSlaveList->GetFirstSelectedItemPosition())))
			pSlaveList->SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);
	}
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
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CWallChanger 訊息處理常式

BOOL CWallChanger::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_cIni.SetPathName(_T(".\\WallChanger.ini"));

	m_DirList.Init(CResString(IDS_WALL_NOTSELCLASS), -1);
	m_DirList.EnableWindow(FALSE);
	m_pCurDirList = &m_DirList;
	m_ClassList.Init();
	m_EnableClass.Init();

	m_uWaitTime = m_cIni.GetUInt(_T("General"), _T("uWaitTime"), DEFAULT_WAIT_TIME_PERWALLPAPER);
	CString sBuf;
	sBuf.Format(_T("%d"), m_uWaitTime);
	m_WaitTime.SetWindowText(sBuf);

	m_EnableWallChanger.SetCheck(m_bEnableWallChanger == true ? BST_CHECKED : BST_UNCHECKED);
	CreateThread();

	m_bInit = true;
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX 屬性頁應傳回 FALSE
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
	if (m_EnableClass.IsModified()) {
		m_cIni.DeleteSection(_T("EnableClassList"));

		int iCount = m_EnableClass.GetItemCount();
		if (iCount) {
			int i;
			CString sPos, sBuf;

			for (i=0 ; i<iCount ; i++) {
				sPos.Format(_T("%d"), i);
				sBuf = m_EnableClass.GetItemText(i, 0);
				m_cIni.WriteString(_T("EnableClassList"), sPos, sBuf);
			}
		}
	}
	if (m_ClassList.IsModified()) {
		m_cIni.DeleteSection(_T("ClassList"));

		int iCount = m_ClassList.GetItemCount();
		if (iCount) {
			int i;
			CString sPos, sBuf;

			for (i=0 ; i<iCount ; i++) {
				sPos.Format(_T("%d"), i);
				sBuf = m_ClassList.GetItemText(i, 0);
				m_cIni.WriteString(_T("ClassList"), sPos, sBuf);
			}
		}
	}
}

void CWallChanger::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	DoOnSize();
}

void CWallChanger::OnBnClickedWallchangercheck()
{
	((CMagicKDDlg*)GetParent())->MainConfigSyncTabEnable();
	if (m_EnableWallChanger.GetCheck() == BST_CHECKED)
		((CMagicKDDlg*)GetParent())->SetEnableFunc(CMagicKDDlg::eFunc_WallChanger, true);
	else
		((CMagicKDDlg*)GetParent())->SetEnableFunc(CMagicKDDlg::eFunc_WallChanger, false);
}

void CWallChanger::OnLvnItemchangedWallenableclass(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	CWallEnableItem *pItem = (CWallEnableItem *) m_EnableClass.GetFirstSelectedItemLParam();
	if (pItem) {
		SetDirList(pItem->GetChildDirList(), pItem->GetItemName());
		SyncListSel(&m_EnableClass, &m_ClassList);
	} else {
		SetDirList(&m_DirList, CResString(IDS_WALL_NOTSELCLASS));
		m_ClassList.CancleAllSelected();
	}

	*pResult = 0;
}

void CWallChanger::OnNMSetfocusWallenableclass(NMHDR *pNMHDR, LRESULT *pResult)
{
	CWallEnableItem *pItem = (CWallEnableItem *) m_EnableClass.GetFirstSelectedItemLParam();
	if (pItem) {
		SetDirList(pItem->GetChildDirList(), pItem->GetItemName());
		SyncListSel(&m_EnableClass, &m_ClassList);
	} else {
		SetDirList(&m_DirList, CResString(IDS_WALL_NOTSELCLASS));
		m_ClassList.CancleAllSelected();
	}

	*pResult = 0;
}

void CWallChanger::OnLvnItemchangedWallclasslist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	CWallClassListItem *pItem = (CWallClassListItem *) m_ClassList.GetFirstSelectedItemLParam();
	if (pItem) {
		SetDirList(pItem->GetChildDirList(), pItem->GetItemName());
		SyncListSel(&m_ClassList, &m_EnableClass);
	} else {
		SetDirList(&m_DirList, CResString(IDS_WALL_NOTSELCLASS));
		m_EnableClass.CancleAllSelected();
	}
	
	*pResult = 0;
}

void CWallChanger::OnNMSetfocusWallclasslist(NMHDR *pNMHDR, LRESULT *pResult)
{
	CWallClassListItem *pItem = (CWallClassListItem *) m_ClassList.GetFirstSelectedItemLParam();
	if (pItem) {
		SetDirList(pItem->GetChildDirList(), pItem->GetItemName());
		SyncListSel(&m_ClassList, &m_EnableClass);
	} else {
		SetDirList(&m_DirList, CResString(IDS_WALL_NOTSELCLASS));
		m_EnableClass.CancleAllSelected();
	}

	*pResult = 0;
}

void CWallChanger::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if (bShow)
		m_Edit_NewClass.SetFocus();
}
