// WallChangerDlg.cpp : 實作檔
//

#include "stdafx.h"
#include "Others.h"
#include "Language.h"
#include "MagicKD.h"
#include "MagicKDDlg.h"
#include "WallChangerDlg.h"

// GLobal variable
CKDStringList slWallChangerEnablePicPath;


// CWallChangerDlg 對話方塊

IMPLEMENT_DYNAMIC(CWallChangerDlg, CDialog)
CWallChangerDlg::CWallChangerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWallChangerDlg::IDD, pParent), m_bCanThread(false), m_uWaitTime(30), m_bIsThreading(false), m_bInit(false),
	m_pCurListDirPath(NULL)
{
}

CWallChangerDlg::~CWallChangerDlg()
{
}

void CWallChangerDlg::DoSize()
{
	if (!m_bInit)
		return;

	SetRedraw(FALSE);

	//CRect rcWin, rcBase;
	//CPoint cpBase;
	//GetWindowRect(rcBase);
	//cpBase.SetPoint(rcBase.left, rcBase.top);

	//if (m_pCurListDirPath) {
	//	CPoint cpOffset(160, 25);
	//	m_pCurListDirPath->GetClientRect(rcWin);
	//	rcWin.left = rcWin.right - 200;
	//	rcWin.top = rcWin.bottom - 215;
	//	m_pCurListDirPath->MoveWindow(rcWin + cpOffset, FALSE);
	//}

	//m_staticTime.GetWindowRect(rcWin);
	//m_staticTime.MoveWindow(rcWin - cpBase);
	//m_editWaitTime.GetWindowRect(rcWin);

	SetRedraw(TRUE);
	Invalidate();
}

void CWallChangerDlg::SaveIni()
{
	if (!IsIniModify())
		return;

	m_cIni.WriteUInt(_T("General"), _T("uWaitTime"), m_uWaitTime);

	CKDIni::SaveIni();
}

DWORD CWallChangerDlg::ThreadProc()
{
	if (m_bIsThreading)
		return 0;
	m_bIsThreading = true;
	CString sTime;
	int iTime;
	while (m_bCanThread) {
		Sleep(1000);
		if (!m_bCanThread)
			break;
		m_staticTime.GetWindowText(sTime);
		iTime = _ttoi(sTime) - 1;
		if (iTime < 0) {
			OnBnClickedButtonRandpic();
		} else {
			sTime.Format(_T("%d"), iTime);
			m_staticTime.SetWindowText(sTime);
		}
	}
	m_bIsThreading = false;
	return 0;
}

void CWallChangerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_RESTTIME, m_staticTime);
	DDX_Control(pDX, IDC_EDIT_WAITTIME, m_editWaitTime);
	DDX_Control(pDX, IDC_LIST_CLASS, m_listClass);
	DDX_Control(pDX, IDC_LIST_DIRPATH, m_listDirPath);
	DDX_Control(pDX, IDC_EDIT_ADDCLASS, m_editAddClass);
	DDX_Control(pDX, IDC_STATIC_NOWPICPATH, m_staticNowPicPath);
}


BEGIN_MESSAGE_MAP(CWallChangerDlg, CDialog)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_SETWAITTIME, OnBnClickedButtonSetwaittime)
	ON_BN_CLICKED(IDC_BUTTON_ADDCLASSLIST, OnBnClickedButtonAddclasslist)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_CLASS, OnLvnItemchangedListClass)
	ON_BN_CLICKED(IDC_BUTTON_RANDPIC, OnBnClickedButtonRandpic)
	ON_BN_CLICKED(IDC_BUTTON_DELPIC, OnBnClickedButtonDelpic)
END_MESSAGE_MAP()


// CWallChangerDlg 訊息處理常式

BOOL CWallChangerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此加入額外的初始化
	srand(time(NULL));
	m_cIni.SetPathName(theApp.m_sAppDir + _T("WallChanger.ini"));

	m_uWaitTime = m_cIni.GetUInt(_T("General"), _T("uWaitTime"), m_uWaitTime);
	CString sTime;
	sTime.Format(_T("%d"), m_uWaitTime);
	m_editWaitTime.SetWindowText(sTime);
	m_staticTime.SetWindowText(_T("0"));

	CRect rcWin;
	m_pCurListDirPath = &m_listDirPath;
	m_listDirPath.Init(&m_cIni, _T(""));
	m_listDirPath.GetClientRect(rcWin);
	m_listClass.Init(&m_cIni, rcWin);

	m_bCanThread = true;
	CreateThread();

	SetIniModify(false);
	m_bInit = true;
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX 屬性頁應傳回 FALSE
}

void CWallChangerDlg::OnDestroy()
{
	if (m_bIsThreading) {
		m_bCanThread = false;
		WaitForSingleObject(m_hThread, 2000);
	}

	SaveIni();

	CDialog::OnDestroy();

	// TODO: 在此加入您的訊息處理常式程式碼
}

void CWallChangerDlg::OnBnClickedButtonSetwaittime()
{
	CString sNewTime, sRestTime;
	int iNewTime, iRestTime;
	m_editWaitTime.GetWindowText(sNewTime);
	iNewTime = _ttoi(sNewTime);
	if (iNewTime > 0) {
		m_staticTime.GetWindowText(sRestTime);
		iRestTime = _ttoi(sRestTime) - m_uWaitTime + iNewTime;
		if (iRestTime < 0)
			iRestTime = 0;
		sRestTime.Format(_T("%d"), iRestTime);
		m_staticTime.SetWindowText(sRestTime);
		m_uWaitTime = iNewTime;
		sNewTime.Format(_T("%d"), iNewTime);
		m_editWaitTime.SetWindowText(sNewTime);
	} else {
		sNewTime.Format(_T("%d"), m_uWaitTime);
		m_editWaitTime.SetWindowText(sNewTime);
	}

	// TODO: 在此加入控制項告知處理常式程式碼
	SetIniModify();
}

void CWallChangerDlg::OnBnClickedButtonAddclasslist()
{
	CString sClassName;
	m_editAddClass.GetWindowText(sClassName);
	if (sClassName.GetLength() <= 0)
		return;
	m_listClass.AddItem(sClassName);
	m_listClass.SetIniModify();
	m_editAddClass.SetWindowText(_T(""));
	// TODO: 在此加入控制項告知處理常式程式碼
}

void CWallChangerDlg::OnBnClickedButtonRandpic()
{
	m_sNowPicPath = ::slWallChangerEnablePicPath.GetRandPos();
	SetWallpaper(m_sNowPicPath, WPSTYLE_CENTER);
	m_staticNowPicPath.SetWindowText(m_sNowPicPath);

	CString sTime;
	sTime.Format(_T("%d"), m_uWaitTime);
	m_staticTime.SetWindowText(sTime);
	// TODO: 在此加入控制項告知處理常式程式碼
}

void CWallChangerDlg::OnBnClickedButtonDelpic()
{
	TCHAR sNowPicPath[MAX_PATH] = {0};
	_tcscpy(sNowPicPath, m_sNowPicPath);

	if (IsShiftPressed())
		RemoveFileDlg(m_hWnd, sNowPicPath, false);
	else
		RemoveFileDlg(m_hWnd, sNowPicPath);
	// TODO: 在此加入控制項告知處理常式程式碼
}

void CWallChangerDlg::OnOK()
{
	// TODO: 在此加入特定的程式碼和 (或) 呼叫基底類別

	CWnd *wndIDC = GetFocus();
	if (wndIDC == GetDlgItem(IDC_EDIT_WAITTIME)) {
		OnBnClickedButtonSetwaittime();
	} else if (wndIDC == GetDlgItem(IDC_EDIT_ADDCLASS)) {
		OnBnClickedButtonAddclasslist();
	}

//	CDialog::OnOK();
}

void CWallChangerDlg::OnCancel()
{
	// TODO: 在此加入特定的程式碼和 (或) 呼叫基底類別
	((CMagicKDDlg *)GetParent())->DestroyWindow();

//	CDialog::OnCancel();
}

void CWallChangerDlg::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	// TODO: 在此加入您的訊息處理常式程式碼
	DoSize();
}

LRESULT CWallChangerDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_COMMAND:
		{
			UINT nID = LOWORD(wParam);
			switch (nID) {
			// ClassList
			case IDS_WALLADDENABLECLASSLIST:
				m_listClass.SetSelectItemCheckState(true);
				m_listClass.SetIniModify();
				break;
			case IDS_WALLDELCLASSLIST:
				m_listClass.DeleteSelectItem();
				m_listClass.SetIniModify();
				break;
			// DirList
			case IDS_WALLADDDIRPATH:
				if (m_listClass.GetSelectedCount() != 1) {
					MessageBox(CResString(IDS_WALLCANONLYCHOICEONECLASS), NULL, MB_OK|MB_ICONERROR);
					break;
				}
				{
					TCHAR sDirPath[MAX_PATH];
					if (ChooseFolder(sDirPath)) {
						m_pCurListDirPath->AddItem(sDirPath);
						m_pCurListDirPath->SetIniModify();
						m_listClass.SetIniModify();
					}
					Invalidate();
				}
				break;
			case IDS_WALLUPDATEDIRFILEFIND:
				m_pCurListDirPath->UpdateSelectItemFileFindNum();
				m_pCurListDirPath->SetIniModify();
				break;
			case IDS_WALLDELDIRPATH:
				m_pCurListDirPath->DeleteSelectItem();
				m_pCurListDirPath->SetIniModify();
				m_listClass.SetIniModify();
				break;
			}
		}
		break;
	}
	// TODO: 在此加入特定的程式碼和 (或) 呼叫基底類別

	return __super::DefWindowProc(message, wParam, lParam);
}

void CWallChangerDlg::OnLvnItemchangedListClass(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此加入控制項告知處理常式程式碼

	if (m_bInit && (pNMLV->uOldState==INDEXTOSTATEIMAGEMASK(1)) && (pNMLV->uNewState==INDEXTOSTATEIMAGEMASK(2))) {
		// Check this Item
		m_listClass.SetItemEnable(pNMLV->iItem, true);
		m_listClass.SetIniModify();
	} else if (m_bInit && (pNMLV->uOldState==INDEXTOSTATEIMAGEMASK(2)) && (pNMLV->uNewState==INDEXTOSTATEIMAGEMASK(1))) {
		// UnCheck this Item
		m_listClass.SetItemEnable(pNMLV->iItem, false);
		m_listClass.SetIniModify();
	} else {
		// Change Select Items
		int iSelCount = m_listClass.GetSelectedCount();
		if ((iSelCount == 0) && (m_pCurListDirPath != &m_listDirPath)) {
			if (m_pCurListDirPath)
				m_pCurListDirPath->ShowWindow(SW_HIDE);
			m_pCurListDirPath = &m_listDirPath;
			m_pCurListDirPath->ShowWindow(SW_SHOW);
		} else if (iSelCount == 1) {
			CRect rcWin, rcBase;
			CPoint cpBase;
			GetWindowRect(rcBase);
			cpBase = rcBase.TopLeft();
			m_listDirPath.GetWindowRect(rcWin);
			rcWin = rcWin - cpBase;
			if (m_pCurListDirPath)
				m_pCurListDirPath->ShowWindow(SW_HIDE);
			CWallClassListItem *pListItem = (CWallClassListItem *)m_listClass.GetFirstSelectedItemLParam();
			if (pListItem)
				m_pCurListDirPath = (CWallDirListCtrl *)pListItem->GetChildDirList();
			if (m_pCurListDirPath) {
				m_pCurListDirPath->MoveWindow(rcWin);
				m_pCurListDirPath->ShowWindow(SW_SHOW);
			}
		}
	}

	*pResult = 0;
}
