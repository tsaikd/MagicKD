// WallChangerDlg.cpp : 實作檔
//

#include "stdafx.h"
#include "Others.h"
#include "Language.h"
#include "MagicKD.h"
#include "MagicKDDlg.h"
#include "WallChangerDlg.h"

// GLobal variable
CKDStringList g_slWallChangerEnablePicPath;
CxImageList *g_pimglCachePic = NULL;

#define DEFAULT_WAITTIME		30
#define DEFAULT_CACHEPICNUM		5
#define DEFAULT_PICPATHHISTORY	10
#define DEFAULT_ENABLETIP		TRUE


// CWallChangerDlg 對話方塊

IMPLEMENT_DYNAMIC(CWallChangerDlg, CDialog)
CWallChangerDlg::CWallChangerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWallChangerDlg::IDD, pParent), m_uWaitTime(30), m_bInit(false),// m_bCanThread(true),
	m_pCurListDirPath(NULL), m_uCachePicNum(0), m_uPicPathHistory(0), m_bEnableTip(true)
{
}

CWallChangerDlg::~CWallChangerDlg()
{
}

DWORD CWallChangerDlg::ThreadProc()
{
	CString sTime;
	int iTime;
	while (IsCanThread()) {
		Sleep(1000);

		if (!IsCanThread())
			return 0;

		m_staticTime.GetWindowText(sTime);
		iTime = _ttoi(sTime) - 1;

		//if (iTime % 5 == 0) {	// 因為 cache pic num 還有一些 bug, 每五秒檢查一次
		//	if (::g_pimglCachePic->GetAllPicCount() > m_uCachePicNum)
		//		::g_pimglCachePic->RemoveTail();
		//	else if (::g_pimglCachePic->GetAllPicCount() < m_uCachePicNum) {
		//		CString sRandPicPath = GetRandPicPath();
		//		if (sRandPicPath.GetLength() && !::g_pimglCachePic->FindPath(sRandPicPath))
		//			::g_pimglCachePic->AddTail(sRandPicPath);
		//	}
		//	sTime.Format(_T("%d"), ::g_pimglCachePic->GetCount());
		//	m_staticCachePicNum.SetWindowText(sTime);
		//}

		if (!IsCanThread())
			return 0;

		if (iTime < 0) {
			OnBnClickedButtonRandpic();
		} else {
			sTime.Format(_T("%d"), iTime);
			m_staticTime.SetWindowText(sTime);
		}
	}
	return 0;
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

	if (m_uWaitTime != DEFAULT_WAITTIME)
		m_cIni.WriteUInt(_T("General"), _T("uWaitTime"), m_uWaitTime);
	else
		m_cIni.DeleteKey(_T("General"), _T("uWaitTime"));

	if (m_uCachePicNum != DEFAULT_CACHEPICNUM)
		m_cIni.WriteUInt(_T("General"), _T("uCachePicNum"), m_uCachePicNum);
	else
		m_cIni.DeleteKey(_T("General"), _T("uCachePicNum"));

	if (m_uPicPathHistory != DEFAULT_PICPATHHISTORY)
		m_cIni.WriteUInt(_T("General"), _T("uPicPathHistory"), m_uPicPathHistory);
	else
		m_cIni.DeleteKey(_T("General"), _T("uPicPathHistory"));

	if (m_bEnableTip != DEFAULT_ENABLETIP)
		m_cIni.WriteBool(_T("General"), _T("bEnableTip"), m_bEnableTip);
	else
		m_cIni.DeleteKey(_T("General"), _T("bEnableTip"));

	CKDIni::SaveIni();
}

void CWallChangerDlg::SetHistoryNum(UINT uNum)
{
	CString sNum;
	if (uNum < 1)
		uNum = 1;
	m_uPicPathHistory = uNum;
	sNum.Format(_T("%d"), uNum);
	m_editHistoryNum.SetWindowText(sNum);
	SetIniModify();
}

// if list number == 0, then set cache number = 0 and return false
// if cache number > list number, then set cache number = list number and return false
// else return true
bool CWallChangerDlg::SetCachePicNum(UINT uCachePicNum) {
	UINT uCachePicNumMax = ::g_slWallChangerEnablePicPath.GetCount();

	::g_pimglCachePic->RemoveAll();
	if (uCachePicNumMax == 0) {
		m_uCachePicNum = 0;
		m_editCacheNum.SetWindowText(_T("0"));
		return false;
	}

	CString sRandPicPath;
	bool bRes = true;
	if (uCachePicNum > uCachePicNumMax) {
		bRes = false;
		uCachePicNum = uCachePicNumMax;
	}

	for (UINT i=0 ; i<uCachePicNum ; i++) {
		sRandPicPath = GetRandPicPath();
		if (::g_pimglCachePic->FindPath(sRandPicPath)) {
			uCachePicNum--;
			i--;
			continue;
		}
		::g_pimglCachePic->AddTail(sRandPicPath);
	}

	m_uCachePicNum = uCachePicNum;
	sRandPicPath.Format(_T("%d"), m_uCachePicNum);
	m_editCacheNum.SetWindowText(sRandPicPath);
	SetIniModify();

	return bRes;
}

UINT CWallChangerDlg::GetCachePicNum()
{
	return m_uCachePicNum;
}

const CString CWallChangerDlg::GetRandPicPath()
{
	int iCountEnablePicPath = ::g_slWallChangerEnablePicPath.GetCount();
	if (iCountEnablePicPath < 1)
		return CString();
	else if (iCountEnablePicPath == 1)
		return ::g_slWallChangerEnablePicPath.GetRandPos();

	CString sTailHistory;
	if (m_slPicPathHistory.GetCount())
		sTailHistory = m_slPicPathHistory.GetTail();
	CString sRandPicPath = ::g_slWallChangerEnablePicPath.GetRandPos();

	while (sRandPicPath == sTailHistory) {
		if (!IsCanThread())
			return CString();
		sRandPicPath = ::g_slWallChangerEnablePicPath.GetRandPos();
		if ((m_slPicPathHistory.Find(sRandPicPath)) && (m_slPicPathHistory.GetCount()>iCountEnablePicPath))
			continue;
	}

	if (!IsCanThread())
		return CString();
	if ((UINT)m_slPicPathHistory.GetCount() > m_uPicPathHistory)
		m_slPicPathHistory.RemoveHead();
	m_slPicPathHistory.AddTail(sRandPicPath);

	return sRandPicPath;
}

BOOL CWallChangerDlg::EnableToolTips(BOOL bEnable/* = TRUE*/)
{
	m_editWaitTime.EnableToolTips(CResString(IDS_WALL_TOOLTIP_SETWAITTIME), bEnable);
	m_editAddClass.EnableToolTips(CResString(IDS_WALL_TOOLTIP_ADDCLASS), bEnable);
	m_editHistoryNum.EnableToolTips(CResString(IDS_WALL_TOOLTIP_SETHISTORYNUM), bEnable);
	m_editCacheNum.EnableToolTips(CResString(IDS_WALL_TOOLTIP_SETCACHEPICNUM), bEnable);

	m_btn_RandPic.EnableToolTips(CResString(IDS_WALL_TOOLTIP_RANDPIC), bEnable);
	m_btn_DelPic.EnableToolTips(CResString(IDS_WALL_TOOLTIP_DELNOWPIC), bEnable);

	m_listClass.EnableToolTips(NULL, bEnable);
	m_listDirPath.EnableToolTips(NULL, bEnable);

	return CDialog::EnableToolTips(bEnable);
}

CPoint CWallChangerDlg::_AutoPicSize(CPoint &cpSizeSrc, CPoint const &cpSizeMax)
{
	if ((cpSizeMax.x >= cpSizeSrc.x) && (cpSizeMax.y >= cpSizeSrc.y))
		return cpSizeSrc;

	double dRatioX, dRatioY;
	dRatioX = (double)cpSizeMax.x / (double)cpSizeSrc.x;
	dRatioY = (double)cpSizeMax.y / (double)cpSizeSrc.y;

	double dRatio = (dRatioX<dRatioY) ? dRatioX : dRatioY;
	cpSizeSrc.SetPoint((int)((double)cpSizeSrc.x * dRatio), (int)((double)cpSizeSrc.y * dRatio));
	return cpSizeSrc;
}

void CWallChangerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WALL_STATIC_RESTTIME, m_staticTime);
	DDX_Control(pDX, IDC_WALL_EDIT_WAITTIME, m_editWaitTime);
	DDX_Control(pDX, IDC_WALL_LIST_CLASS, m_listClass);
	DDX_Control(pDX, IDC_WALL_LIST_DIRPATH, m_listDirPath);
	DDX_Control(pDX, IDC_WALL_EDIT_ADDCLASS, m_editAddClass);
	DDX_Control(pDX, IDC_WALL_STATIC_NOWPICPATH, m_staticNowPicPath);
	DDX_Control(pDX, IDC_WALL_EDIT_HISTORYNUM, m_editHistoryNum);
	DDX_Control(pDX, IDC_WALL_EDIT_CACHENUM, m_editCacheNum);
	DDX_Control(pDX, IDC_WALL_BTN_RANDPIC, m_btn_RandPic);
	DDX_Control(pDX, IDC_WALL_BTN_DELPIC, m_btn_DelPic);
	DDX_Control(pDX, IDC_WALL_BTN_ENABLETOOLTIP, m_btn_EnableToolTip);
	DDX_Control(pDX, IDC_WALL_BTN_ADDCLASSLIST, m_btn_AddClass);
	DDX_Control(pDX, IDC_WALL_STATIC_CACHENUM, m_staticCachePicNum);
}


BEGIN_MESSAGE_MAP(CWallChangerDlg, CDialog)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_WALL_BTN_ADDCLASSLIST, OnBnClickedButtonAddclasslist)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_WALL_LIST_CLASS, OnLvnItemchangedListClass)
	ON_BN_CLICKED(IDC_WALL_BTN_RANDPIC, OnBnClickedButtonRandpic)
	ON_BN_CLICKED(IDC_WALL_BTN_DELPIC, OnBnClickedButtonDelpic)
	ON_BN_CLICKED(IDC_WALL_BTN_ENABLETOOLTIP, OnBnClickedButtonEnabletooltip)
END_MESSAGE_MAP()

// CWallChangerDlg 訊息處理常式

BOOL CWallChangerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此加入額外的初始化
	if (!::g_pimglCachePic)
		::g_pimglCachePic = new CxImageList;
	::g_pimglCachePic->SetCacheNumWnd(m_hWnd);
	CreateThread(THREAD_PRIORITY_ABOVE_NORMAL);

	theTray.InsertMenu(0, MF_STRING | MF_BYPOSITION, IDS_TRAY_DELNOWPIC, CResString(IDS_TRAY_DELNOWPIC));
	theTray.InsertMenu(0, MF_STRING | MF_BYPOSITION, IDS_TRAY_RANDPIC, CResString(IDS_TRAY_RANDPIC), true);

	srand(time(NULL));
	m_cIni.SetPathName(theApp.m_sAppDir + _T("WallChanger.ini"));
	m_sTempFilePath.Format(_T("%s.jpg"), GetTempFilePath());
	GetDlgItem(IDC_WALL_STATIC_CACHEDNUM)->SetWindowText(CResString(IDS_WALL_STATIC_CACHEDNUM));
	GetDlgItem(IDC_WALL_STATIC_WAITTIME)->SetWindowText(CResString(IDS_WALL_STATIC_WAITTIME));

	m_uWaitTime = m_cIni.GetUInt(_T("General"), _T("uWaitTime"), DEFAULT_WAITTIME);
	CString sBuf;
	sBuf.Format(_T("%d"), m_uWaitTime);
	m_editWaitTime.SetWindowText(sBuf);
	m_staticTime.SetWindowText(_T("0"));

	CRect rcWin;
	m_pCurListDirPath = &m_listDirPath;
	m_listDirPath.Init(&m_cIni, _T(""));
	m_listDirPath.ShowWindow(SW_SHOW);
	m_listDirPath.GetClientRect(rcWin);
	m_listClass.Init(&m_cIni, rcWin);

	m_uPicPathHistory = m_cIni.GetUInt(_T("General"), _T("uPicPathHistory"), DEFAULT_PICPATHHISTORY);
	SetHistoryNum(m_uPicPathHistory);

	m_uCachePicNum = m_cIni.GetUInt(_T("General"), _T("uCachePicNum"), DEFAULT_CACHEPICNUM);
	SetCachePicNum(m_uCachePicNum);

	m_bEnableTip = m_cIni.GetBool(_T("General"), _T("bEnableTip"), DEFAULT_ENABLETIP);
	EnableToolTips(m_bEnableTip);

	m_btn_RandPic.SetWindowText(CResString(IDS_WALL_BTN_RANDPIC));
	m_btn_DelPic.SetWindowText(CResString(IDS_WALL_BTN_DELPIC));
	m_btn_EnableToolTip.SetWindowText(CResString(m_bEnableTip ? IDS_WALL_BTN_DISABLETIP : IDS_WALL_BTN_ENABLETIP));
	m_btn_AddClass.SetWindowText(CResString(IDS_WALL_BTN_ADDCLASS));

	SetIniModify(false);
	m_bInit = true;
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX 屬性頁應傳回 FALSE
}

void CWallChangerDlg::OnDestroy()
{
	SetCanThread(false);
	if (WAIT_TIMEOUT == WaitForThread(10000)) {
		MessageBox(_T("WallChanger Thread is running!!"), _T("ERROR"), MB_OK | MB_ICONERROR);
		TerminateThread(0);
	}
	if (::g_pimglCachePic) {
		delete ::g_pimglCachePic;
		::g_pimglCachePic = NULL;
	}

	SaveIni();

	CDialog::OnDestroy();

	if (PathFileExists(m_sTempFilePath))
		DeleteFile(m_sTempFilePath);

	theTray.RemoveTrayMenuItem(CResString(IDS_TRAY_RANDPIC));
	theTray.RemoveTrayMenuItem(CResString(IDS_TRAY_DELNOWPIC));

	// TODO: 在此加入您的訊息處理常式程式碼
}

void CWallChangerDlg::OnOK()
{
	// TODO: 在此加入特定的程式碼和 (或) 呼叫基底類別

	CWnd *wndIDC = GetFocus();
	if (wndIDC == GetDlgItem(IDC_WALL_EDIT_WAITTIME)) {
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
		SetIniModify();
	} else if (wndIDC == GetDlgItem(IDC_WALL_EDIT_ADDCLASS)) {
		OnBnClickedButtonAddclasslist();
	} else if (wndIDC == GetDlgItem(IDC_WALL_EDIT_HISTORYNUM)) {
		CString sBuf;
		m_editHistoryNum.GetWindowText(sBuf);
		UINT uNum = (UINT)_ttoi(sBuf);
		SetHistoryNum(uNum);
	} else if (wndIDC == GetDlgItem(IDC_WALL_EDIT_CACHENUM)) {
		CString sBuf;
		m_editCacheNum.GetWindowText(sBuf);
		UINT uNum = (UINT)_ttoi(sBuf);
		SetCachePicNum(uNum);
	}

//	CDialog::OnOK();
}

void CWallChangerDlg::OnCancel()
{
	// TODO: 在此加入特定的程式碼和 (或) 呼叫基底類別
	((CMagicKDDlg *)GetParent())->DestroyWindow();

//	CDialog::OnCancel();
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
	if (!m_muxRandPic.Lock(0))
		return;

#define RETURN { m_muxRandPic.Unlock(); return; }

	bool bRes = true;

	m_sNowPicPath = GetRandPicPath();
	if (!m_sNowPicPath.GetLength()) {
		m_staticNowPicPath.SetWindowText(CResString(IDS_WALL_SETWALLPAGERFAIL));
		RETURN;
	}
	if (!IsCanThread())
		RETURN;

	if (m_uCachePicNum) {
		::g_pimglCachePic->AddTail(m_sNowPicPath);

		if (::g_pimglCachePic->GetCount()) {
			m_sNowPicPath = ::g_pimglCachePic->GetHeadPath();
			if (::g_pimglCachePic->IsHeadImageResample()) {
				CString sImageBufPath = ::g_pimglCachePic->GetHeadBufPath();

				bRes = bRes && SetWallpaper(sImageBufPath, WPSTYLE_CENTER);
			} else {
				bRes = bRes && SetWallpaper(m_sNowPicPath, WPSTYLE_CENTER);
			}

			if (!IsCanThread())
				RETURN;

			::g_pimglCachePic->RemoveHead();
		} else {
			bRes = false;
		}
	} else {
		if (!IsCanThread())
			RETURN;
		if (m_xImgNowPic.Load(m_sNowPicPath)) {
			if (!IsCanThread())
				RETURN;

			if (g_pimglCachePic->AutoPicSize(m_xImgNowPic)) {
				if (!IsCanThread())
					RETURN;

				m_xImgNowPic.Save(m_sTempFilePath, CXIMAGE_FORMAT_JPG);

				if (!IsCanThread())
					RETURN;
				bRes = bRes && SetWallpaper(m_sTempFilePath, WPSTYLE_CENTER);
			} else {
				bRes = bRes && SetWallpaper(m_sNowPicPath, WPSTYLE_CENTER);
			}
		} else {
			bRes = false;
			m_sNowPicPath = _T("");
		}
	}

	if (!IsCanThread())
		RETURN;

	if (bRes) {
		m_staticNowPicPath.SetWindowText(m_sNowPicPath);
		theTray.SetTrayTip(m_sNowPicPath);
	} else {
		m_staticNowPicPath.SetWindowText(CResString(IDS_WALL_SETWALLPAGERFAIL));
		theTray.SetTrayTip(_T(""));
	}

	CString sTime;
	sTime.Format(_T("%d"), m_uWaitTime);
	m_staticTime.SetWindowText(sTime);

	RETURN;
	// TODO: 在此加入控制項告知處理常式程式碼
}
#undef RETURN

void CWallChangerDlg::OnBnClickedButtonDelpic()
{
	RemoveFileDlg(m_hWnd, m_sNowPicPath, !IsShiftPressed());

	if (!PathFileExists(m_sNowPicPath)) {
		POSITION pos;
		while (pos = ::g_slWallChangerEnablePicPath.Find(m_sNowPicPath))
			::g_slWallChangerEnablePicPath.RemoveAt(pos);
		OnBnClickedButtonRandpic();
	}
	// TODO: 在此加入控制項告知處理常式程式碼
}

void CWallChangerDlg::OnBnClickedButtonEnabletooltip()
{
	if (m_bEnableTip) {
		m_bEnableTip = FALSE;
		m_btn_EnableToolTip.SetWindowText(CResString(IDS_WALL_BTN_ENABLETIP));
	} else {
		m_bEnableTip = TRUE;
		m_btn_EnableToolTip.SetWindowText(CResString(IDS_WALL_BTN_DISABLETIP));
	}

	EnableToolTips(m_bEnableTip);

	SetIniModify();
	// TODO: 在此加入控制項告知處理常式程式碼
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
	case WMU_UPDATE_CACHENUM:
		{
			CString sNum;
			sNum.Format(_T("%d"), ::g_pimglCachePic->GetCount());
			m_staticCachePicNum.SetWindowText(sNum);
		}
		break;
	case WM_COMMAND:
		{
			UINT nID = LOWORD(wParam);
			switch (nID) {
			// Tray Menu
			case IDS_TRAY_RANDPIC:
				OnBnClickedButtonRandpic();
				return 0;
			case IDS_TRAY_DELNOWPIC:
				OnBnClickedButtonDelpic();
				return 0;
			// ClassList
			case IDS_WALL_MENU_ADDENABLECLASSLIST:
				m_listClass.SetSelectItemCheckState(true);
				m_listClass.SetIniModify();
				return 0;
			case IDS_WALL_MENU_DELCLASSLIST:
				m_listClass.DeleteSelectItem();
				m_listClass.SetIniModify();
				return 0;
			case IDS_WALL_MENU_UPDATEALLDIRFILEFIND:
				m_listClass.UpdateSelectItemFileFindNum();
				return 0;
			// DirList
			case IDS_WALL_MENU_ADDDIRPATH:
				if (m_listClass.GetSelectedCount() != 1) {
					MessageBox(CResString(IDS_WALL_CANONLYCHOICEONECLASS), NULL, MB_OK|MB_ICONERROR);
					return 0;
				} else {
					CString sDirPath;
					if (ChooseFolder(sDirPath, m_hWnd)) {
						m_pCurListDirPath->AddItem(sDirPath);
						m_pCurListDirPath->SetIniModify();
						m_listClass.SetIniModify();
					}
					Invalidate();
				}
				return 0;
			case IDS_WALL_MENU_UPDATEDIRFILEFIND:
				m_pCurListDirPath->UpdateSelectItemFileFindNum();
				m_pCurListDirPath->SetIniModify();
				return 0;
			case IDS_WALL_MENU_DELDIRPATH:
				m_pCurListDirPath->DeleteSelectItem();
				m_pCurListDirPath->SetIniModify();
				m_listClass.SetIniModify();
				return 0;
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
		Invalidate(FALSE);
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
		Invalidate();
	}

	*pResult = 0;
}
