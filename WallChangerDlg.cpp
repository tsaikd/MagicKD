// WallChangerDlg.cpp : 實作檔
//

#include "stdafx.h"
#include "Language.h"
#include "Others.h"
#include "WallOthers.h"
#include "MagicKD.h"
#include "MagicKDDlg.h"

#include "WallChangerDlg.h"

#define DEFAULT_WAITTIME			30
#define DEFAULT_CACHEPICNUM			0
#define DEFAULT_PICPATHHISTORY		10
#define DEFAULT_ENABLETIP			TRUE
#define DEFAULT_SHOWDIRLOADERROR	true
#define DEFAULT_COMBOX_MSG			GetResString(IDS_WALL_COMBOX_ASKUSER)

#define TESTOFFLINECOUNT			30

enum {
	KDT_CHANGEWALL	= 1
};

// GLobal variable
CWallChangerDlg *g_pWallChangerDlg = NULL;
#include "WallEnablePicList.h"
CWallEnablePicList *g_pWallEnablePicList = NULL;
#include "WallThreadImageCache.h"
CWallThreadImageCache *g_pWallThreadImageCache = NULL;
#include "WallThreadFindPic.h"
CWallThreadFindPic *g_pWallThreadFindPic = NULL;

// CWallChangerDlg 對話方塊

IMPLEMENT_DYNAMIC(CWallChangerDlg, CDialog)
CWallChangerDlg::CWallChangerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWallChangerDlg::IDD, pParent), m_uWaitTime(30), m_bInit(false),
	m_pCurListDirPath(NULL), m_uCachePicNum(0), m_uPicPathHistory(0), m_bEnableTip(true),
	m_bSetCachePicNum(false), m_iTestOfflineCount(TESTOFFLINECOUNT), m_uTimer(0), m_bShowDirLoadError(true)
{
}

CWallChangerDlg::~CWallChangerDlg()
{
}

void CWallChangerDlg::DoSize()
{
	if (!m_bInit)
		return;
}

void CWallChangerDlg::SaveIni()
{
	if (!IsIniModify())
		return;

	if (m_sComboxMsg != DEFAULT_COMBOX_MSG)
		m_cIni.WriteString(_T("General"), _T("sComboxMsg"), m_sComboxMsg);
	else
		m_cIni.DeleteKey(_T("General"), _T("sComboxMsg"));

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

	bool bShowDirLoadError = m_checkShowLoadDirError.GetCheck() == BST_CHECKED;
	if (bShowDirLoadError != DEFAULT_SHOWDIRLOADERROR)
		m_cIni.WriteBool(_T("General"), _T("bShowDirLoadError"), bShowDirLoadError);
	else
		m_cIni.DeleteKey(_T("General"), _T("bShowDirLoadError"));

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
}

void CWallChangerDlg::SetWaitTime(UINT uWaitTime)
{
	CString sNewTime, sRestTime;
	int iRestTime;
	if (uWaitTime) {
		m_staticTime.GetWindowText(sRestTime);
		iRestTime = _ttoi(sRestTime) - m_uWaitTime + uWaitTime;
		if (iRestTime < 0)
			iRestTime = 0;
		sRestTime.Format(_T("%d"), iRestTime);
		m_staticTime.SetWindowText(sRestTime);
		m_uWaitTime = uWaitTime;
		sNewTime.Format(_T("%d"), uWaitTime);
		m_editWaitTime.SetWindowText(sNewTime);

		if (m_uWaitTime)
			StartTimer();
	} else {
		m_uWaitTime = 0;
		m_staticTime.SetWindowText(_T("0"));
		m_editWaitTime.SetWindowText(_T("0"));

		ASSERT(StopTimer());
	}
}

// if list number == 0, then set cache number = 0 and return false
// if cache number > list number, then set cache number = list number and return false
// else return true
bool CWallChangerDlg::SetCachePicNum(UINT uCachePicNum) {
	if (uCachePicNum == 0) {
		m_uCachePicNum = 0;
		m_editCacheNum.SetWindowText(_T("0"));
		return true;
	}

	if (!::g_pWallThreadImageCache) {
		CString sBuf;
		sBuf.Format(_T("%d"), m_uCachePicNum);
		m_editCacheNum.SetWindowText(sBuf);
		return false;
	}

	UINT uCachePicNumMax = ::g_pWallEnablePicList->GetCount();

	::g_pWallThreadImageCache->RemoveAll();
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
		if (::g_pWallThreadImageCache->FindPath(sRandPicPath)) {
			uCachePicNum--;
			i--;
			continue;
		}
		::g_pWallThreadImageCache->AddTail(sRandPicPath);
	}

	if (bRes)
		m_uCachePicNum = uCachePicNum;
	sRandPicPath.Format(_T("%d"), m_uCachePicNum);
	m_editCacheNum.SetWindowText(sRandPicPath);

	return bRes;
}

UINT CWallChangerDlg::GetCachePicNum()
{
	return m_uCachePicNum;
}

//bWay:
//	true: From File List
//	false: From Cache List
bool CWallChangerDlg::SetRandWallPager(bool bWay)
{
	if (!m_muxSetRandWallPager.Lock(0))
		return false;

#define RETURN(x) {																	\
	if (x) {																		\
		m_staticNowPicPath.SetWindowText(m_sNowPicPath);							\
		pTheTray->SetTrayTip(m_sNowPicPath);											\
	} else {																		\
		m_sNowPicPath = _T("");														\
		m_staticNowPicPath.SetWindowText(GetResString(IDS_WALL_SETWALLPAGERFAIL));	\
		pTheTray->SetTrayTip(_T(""));													\
	}																				\
	m_muxSetRandWallPager.Unlock(); return (x);										\
}

	m_staticNowPicPath.SetWindowText(GetResString(IDS_WALL_SETTINGWALLPAGER));
	m_sNowPicPath = GetRandPicPath();

	if (m_sNowPicPath.IsEmpty())
		RETURN(false);

	if (bWay){
		if (m_xImgNowPic.Load(m_sNowPicPath)) {
			if (AutoPicSize(m_xImgNowPic)) {
				m_xImgNowPic.Save(m_sTempFilePath, CXIMAGE_FORMAT_JPG);
				SetWallpaper(m_sTempFilePath, WPSTYLE_CENTER);
			} else {
				SetWallpaper(m_sNowPicPath, WPSTYLE_CENTER);
			}
			RETURN(true);
		} else {
			int iRes = IDCANCEL;
			if (m_sComboxMsg == GetResString(IDS_WALL_COMBOX_NULL)) {
				iRes = IDCANCEL;
			} else if (m_sComboxMsg == GetResString(IDS_WALL_COMBOX_DELETE)) {
				DeletePicFile(m_sNowPicPath, false);
				RETURN(false);
			} else if (m_sComboxMsg == GetResString(IDS_WALL_COMBOX_ASKDELETE)) {
				iRes = IDYES;
			} else if (m_sComboxMsg == GetResString(IDS_WALL_COMBOX_EXPLORE)) {
				iRes = IDNO;
			} else {
				CString sMsg;
				sMsg.Format(_T("%s\n%s"), m_sNowPicPath, GetResString(IDS_WALL_MSG_IMAGELOADERROR));
				iRes = MessageBox(sMsg, NULL, MB_YESNOCANCEL | MB_ICONQUESTION);
			}

			if ((iRes == IDYES) && PathFileExists(m_sNowPicPath)) {
				DeletePicFile(m_sNowPicPath, true);
				RETURN(false);
			} else if (iRes == IDNO) {
				CString sBuf = m_sNowPicPath;
				PathRemoveFileSpec(sBuf.GetBuffer());
				sBuf.ReleaseBuffer();
				ShellExecute(m_hWnd, _T("open"), sBuf, NULL, NULL, SW_SHOW);
			}

			RETURN(false);
		}
	} else {
		::g_pWallThreadImageCache->AddTail(m_sNowPicPath);
		if (!(::g_pWallThreadImageCache->GetCount()))
			RETURN(false);

		bool bRes = true;
		m_sNowPicPath = ::g_pWallThreadImageCache->GetHeadPath();
		if (::g_pWallThreadImageCache->IsHeadImageResample()) {
			CString sImageBufPath = ::g_pWallThreadImageCache->GetHeadBufPath();

			bRes = SetWallpaper(sImageBufPath, WPSTYLE_CENTER);
		} else {
			bRes = SetWallpaper(m_sNowPicPath, WPSTYLE_CENTER);
		}

		::g_pWallThreadImageCache->RemoveHead();
		RETURN(bRes);
	}

	RETURN(true);
#undef RETURN
}

LPCTSTR CWallChangerDlg::GetRandPicPath()
{
	int iCountEnablePicPath = ::g_pWallEnablePicList->GetCount();
	if (iCountEnablePicPath < 1)
		return _T("");
	else if (iCountEnablePicPath == 1)
		return ::g_pWallEnablePicList->GetRandPic();

	CString sTailHistory;
	if (m_slPicPathHistory.GetCount())
		sTailHistory = m_slPicPathHistory.GetTail();
	CString sRandPicPath = ::g_pWallEnablePicList->GetRandPic();

	while (sRandPicPath == sTailHistory) {
		sRandPicPath = ::g_pWallEnablePicList->GetRandPic();
		if ((m_slPicPathHistory.Find(sRandPicPath)) && (m_slPicPathHistory.GetCount()>iCountEnablePicPath))
			continue;
	}

	if ((UINT)m_slPicPathHistory.GetCount() > m_uPicPathHistory)
		m_slPicPathHistory.RemoveHead();
	m_slPicPathHistory.AddTail(sRandPicPath);

	return sRandPicPath;
}

// Delete Picture File and Remove from Enable List if posiable
// If Delete success, then return true
bool CWallChangerDlg::DeletePicFile(LPCTSTR sFilePath, bool bAskForSure/* = true*/)
{
	if (!PathFileExists(sFilePath))
		return false;

	bool bRes;
	if (bAskForSure)
		bRes = RemoveFileDlg(m_hWnd, sFilePath, !IsShiftPressed());
	else
		bRes = (DeleteFile(sFilePath) == TRUE);

	if (!PathFileExists(sFilePath)) {
		CStringArray *psaRes;
		if (psaRes = ::g_pWallEnablePicList->RemoveFind(sFilePath)) {
			CWallClassListItem *pClassListItem;
			CWallDirListCtrl *pDirListCtrl;
			CWallDirListItem *pDirListItem;
			int i, iCount = m_listClass.GetItemCount();
			int j, jCount;

			for (i=0 ; i<iCount ; i++) {
				if (pClassListItem = (CWallClassListItem *)m_listClass.GetItemData(i)) {
					if (pDirListCtrl = (CWallDirListCtrl *)pClassListItem->GetChildDirList()) {
						jCount = pDirListCtrl->GetItemCount();
						for (j=0 ; j<jCount ; j++) {
							if (pDirListItem = (CWallDirListItem *)pDirListCtrl->GetItemData(j)) {
								if (pDirListItem->GetItemPicPathArray() == psaRes){
									pDirListItem->SetItemFileFindNum(psaRes->GetCount());
									pDirListItem->Invalidate();
									i = iCount;
									break;
								}
							}
						}
					}
				}
			}
		}
	}
	return bRes;
}

UINT CWallChangerDlg::StartTimer()
{
	if (m_uTimer)
		return 0;

	m_uTimer = SetTimer(KDT_CHANGEWALL, 1000, NULL);
	return m_uTimer;
}

UINT CWallChangerDlg::StopTimer()
{
	if (!m_uTimer)
		return 0;

	UINT uRes = KillTimer(m_uTimer);
	m_uTimer = 0;

	return uRes;
}

BOOL CWallChangerDlg::EnableToolTips(BOOL bEnable/* = TRUE*/)
{
	m_editWaitTime.EnableToolTips			(CResString(IDS_WALL_TOOLTIP_SETWAITTIME), bEnable);
	m_editAddClass.EnableToolTips			(CResString(IDS_WALL_TOOLTIP_ADDCLASS), bEnable);
	m_editHistoryNum.EnableToolTips			(CResString(IDS_WALL_TOOLTIP_SETHISTORYNUM), bEnable);
	m_editCacheNum.EnableToolTips			(CResString(IDS_WALL_TOOLTIP_SETCACHEPICNUM), bEnable);

	m_btn_RandPic.EnableToolTips			(CResString(IDS_WALL_TOOLTIP_RANDPIC), bEnable);
	m_btn_DelPic.EnableToolTips				(CResString(IDS_WALL_TOOLTIP_DELNOWPIC), bEnable);

	m_listClass.EnableToolTips				(NULL, bEnable);
	m_listDirPath.EnableToolTips			(NULL, bEnable);

	m_comboxImageLoadError.EnableToolTips	(GetResString(IDS_WALL_TOOLTIP_IMAGELOADERROR), bEnable);

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
	DDX_Control(pDX, IDC_WALL_STATIC_CACHENUM		, m_staticCachePicNum);
	DDX_Control(pDX, IDC_WALL_STATIC_RESTTIME		, m_staticTime);
	DDX_Control(pDX, IDC_WALL_STATIC_PICTOTALNUMSHOW, m_staticPicTotalNum);
	DDX_Control(pDX, IDC_WALL_STATIC_NOWPICPATH		, m_staticNowPicPath);
	DDX_Control(pDX, IDC_WALL_LIST_CLASS			, m_listClass);
	DDX_Control(pDX, IDC_WALL_LIST_DIRPATH			, m_listDirPath);
	DDX_Control(pDX, IDC_WALL_EDIT_WAITTIME			, m_editWaitTime);
	DDX_Control(pDX, IDC_WALL_EDIT_ADDCLASS			, m_editAddClass);
	DDX_Control(pDX, IDC_WALL_EDIT_HISTORYNUM		, m_editHistoryNum);
	DDX_Control(pDX, IDC_WALL_EDIT_CACHENUM			, m_editCacheNum);
	DDX_Control(pDX, IDC_WALL_BTN_RANDPIC			, m_btn_RandPic);
	DDX_Control(pDX, IDC_WALL_BTN_DELPIC			, m_btn_DelPic);
	DDX_Control(pDX, IDC_WALL_BTN_ENABLETOOLTIP		, m_btn_EnableToolTip);
	DDX_Control(pDX, IDC_WALL_BTN_ADDCLASS			, m_btn_AddClass);
	DDX_Control(pDX, IDC_WALL_CHECK_SHOWDIRLOADERROR, m_checkShowLoadDirError);
	DDX_Control(pDX, IDC_WALL_COMBO_IMAGELOADERROR	, m_comboxImageLoadError);
}


BEGIN_MESSAGE_MAP(CWallChangerDlg, CDialog)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_WALL_BTN_ADDCLASS, OnBnClickedButtonAddclasslist)
	ON_BN_CLICKED(IDC_WALL_BTN_RANDPIC, OnBnClickedButtonRandpic)
	ON_BN_CLICKED(IDC_WALL_BTN_DELPIC, OnBnClickedButtonDelpic)
	ON_BN_CLICKED(IDC_WALL_BTN_ENABLETOOLTIP, OnBnClickedButtonEnabletooltip)
	ON_BN_CLICKED(IDC_WALL_CHECK_SHOWDIRLOADERROR, OnBnClickedWallCheckShowdirloaderror)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_WALL_LIST_CLASS, OnLvnItemchangedListClass)
	ON_CBN_SELCHANGE(IDC_COMBO_IMAGELOADERROR, OnCbnSelchangeComboImageloaderror)
END_MESSAGE_MAP()

// CWallChangerDlg 訊息處理常式

BOOL CWallChangerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (!::g_pWallChangerDlg)
		g_pWallChangerDlg = this;
	if (!::g_pWallEnablePicList)
		::g_pWallEnablePicList = new CWallEnablePicList;
	//if (!::g_pWallThreadImageCache)
	//	::g_pWallThreadImageCache = new CWallThreadImageCache;
	//::g_pWallThreadImageCache->SetMsgWnd(m_hWnd);
	if (!::g_pWallThreadFindPic)
		::g_pWallThreadFindPic = new CWallThreadFindPic;
	::g_pWallThreadFindPic->SetMsgWnd(m_hWnd);

	srand(time(NULL));
	pTheTray->InsertMenu(0, MF_STRING | MF_BYPOSITION, IDS_TRAY_DELNOWPIC, CResString(IDS_TRAY_DELNOWPIC));
	pTheTray->InsertMenu(0, MF_STRING | MF_BYPOSITION, IDS_TRAY_RANDPIC, CResString(IDS_TRAY_RANDPIC), true);
	pTheAppEndDlg->SignWnd(m_hWnd, 7);

	m_cIni.SetPathName(theApp.m_sAppDir + _T("WallChanger.ini"));
	m_sTempFilePath.Format(_T("%s.jpg"), GetTempFilePath());
	GetDlgItem(IDC_WALL_STATIC_CACHEDNUM)	->SetWindowText(GetResString(IDS_WALL_STATIC_CACHEDNUM));
	GetDlgItem(IDC_WALL_STATIC_WAITTIME)	->SetWindowText(GetResString(IDS_WALL_STATIC_WAITTIME));
	GetDlgItem(IDC_WALL_STATIC_PICTOTALNUM)	->SetWindowText(GetResString(IDS_WALL_STATIC_PICTOTALNUM));
	GetDlgItem(IDC_WALL_BTN_RANDPIC)		->SetWindowText(GetResString(IDS_WALL_BTN_RANDPIC));
	GetDlgItem(IDC_WALL_BTN_DELPIC)			->SetWindowText(GetResString(IDS_WALL_BTN_DELPIC));
	GetDlgItem(IDC_WALL_BTN_ADDCLASS)		->SetWindowText(GetResString(IDS_WALL_BTN_ADDCLASS));
	m_btn_EnableToolTip.SetWindowText(CResString(m_bEnableTip ? IDS_WALL_BTN_DISABLETIP : IDS_WALL_BTN_ENABLETIP));
	((CComboBox *)GetDlgItem(IDC_COMBO_IMAGELOADERROR))->AddString(GetResString(IDS_WALL_COMBOX_ASKUSER));
	((CComboBox *)GetDlgItem(IDC_COMBO_IMAGELOADERROR))->AddString(GetResString(IDS_WALL_COMBOX_ASKDELETE));
	((CComboBox *)GetDlgItem(IDC_COMBO_IMAGELOADERROR))->AddString(GetResString(IDS_WALL_COMBOX_DELETE));
	((CComboBox *)GetDlgItem(IDC_COMBO_IMAGELOADERROR))->AddString(GetResString(IDS_WALL_COMBOX_EXPLORE));
	((CComboBox *)GetDlgItem(IDC_COMBO_IMAGELOADERROR))->AddString(GetResString(IDS_WALL_COMBOX_NULL));

	m_iTestOfflineCount = TESTOFFLINECOUNT;
	m_uWaitTime = m_cIni.GetUInt(_T("General"), _T("uWaitTime"), DEFAULT_WAITTIME);

	m_sComboxMsg = m_cIni.GetString(_T("General"), _T("sComboxMsg"), DEFAULT_COMBOX_MSG);
	m_comboxImageLoadError.SetCurSel(m_comboxImageLoadError.FindString(-1, m_sComboxMsg));

	GetDlgItem(IDC_WALL_CHECK_SHOWDIRLOADERROR)->SetWindowText(GetResString(IDS_WALL_CHECK_SHOWDIRLOADERROR));
	m_bShowDirLoadError = m_cIni.GetBool(_T("General"), _T("bShowDirLoadError"), DEFAULT_SHOWDIRLOADERROR);
	if (m_bShowDirLoadError)
		m_checkShowLoadDirError.SetCheck(BST_CHECKED);
	else
		m_checkShowLoadDirError.SetCheck(BST_UNCHECKED);

	CString sBuf;
	sBuf.Format(_T("%d"), m_uWaitTime);
	m_editWaitTime.SetWindowText(sBuf);
	m_staticTime.SetWindowText(sBuf);

	CRect rcWin;
	m_pCurListDirPath = &m_listDirPath;
	m_listDirPath.Init(&m_cIni, _T(""), &m_bShowDirLoadError);
	m_listDirPath.ShowWindow(SW_SHOW);
	m_listDirPath.GetClientRect(rcWin);
	m_listClass.Init(&m_cIni, rcWin, &m_bShowDirLoadError);

	m_uCachePicNum = m_cIni.GetUInt(_T("General"), _T("uCachePicNum"), DEFAULT_CACHEPICNUM);
	SetCachePicNum(m_uCachePicNum);

	m_uPicPathHistory = m_cIni.GetUInt(_T("General"), _T("uPicPathHistory"), DEFAULT_PICPATHHISTORY);
	SetHistoryNum(m_uPicPathHistory);

	m_bEnableTip = m_cIni.GetBool(_T("General"), _T("bEnableTip"), DEFAULT_ENABLETIP);
	EnableToolTips(m_bEnableTip);

	// TODO:  在此加入額外的初始化

	SetIniModify(false);
	m_bInit = true;
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX 屬性頁應傳回 FALSE
}

void CWallChangerDlg::OnDestroy()
{
	pTheAppEndDlg->ProgressStepIt(m_hWnd, _T("Closing\tWallChanger\tDialog"));
	SaveIni();

	pTheAppEndDlg->ProgressStepIt(m_hWnd, _T("Stoping\tWallChanger\tTimer"));
	StopTimer();

	pTheAppEndDlg->ProgressStepIt(m_hWnd, _T("Closing\tWallChanger\tFind Picture Thread"));
	if (::g_pWallThreadFindPic) {
		delete ::g_pWallThreadFindPic;
		::g_pWallThreadFindPic = NULL;
	}

	pTheAppEndDlg->ProgressStepIt(m_hWnd, _T("Closing\tWallChanger\tImage Cache Thread"));
	if (::g_pWallThreadImageCache) {
		delete ::g_pWallThreadImageCache;
		::g_pWallThreadImageCache = NULL;
	}

	pTheAppEndDlg->ProgressStepIt(m_hWnd, _T("Closing\tWallChanger\tEnable Picture List Thread"));
	if (::g_pWallEnablePicList) {
		delete ::g_pWallEnablePicList;
		::g_pWallEnablePicList = NULL;
	}

	if (::g_pWallChangerDlg)
		::g_pWallChangerDlg = NULL;

	CDialog::OnDestroy();

	pTheAppEndDlg->ProgressStepIt(m_hWnd, _T("Deleting\tWallChanger\tTemp File"));
	if (PathFileExists(m_sTempFilePath))
		DeleteFile(m_sTempFilePath);

	pTheAppEndDlg->ProgressStepIt(m_hWnd, _T("Deleting\tWallChanger\tTray Menu"));
	pTheTray->RemoveTrayMenuItem(GetResString(IDS_TRAY_RANDPIC));
	pTheTray->RemoveTrayMenuItem(GetResString(IDS_TRAY_DELNOWPIC));

	pTheAppEndDlg->UnsignWnd(m_hWnd);
}

void CWallChangerDlg::OnOK()
{
	CWnd *wndIDC = GetFocus();
	if (wndIDC == GetDlgItem(IDC_WALL_EDIT_WAITTIME)) {
		CString sNewTime;
		int iNewTime;
		m_editWaitTime.GetWindowText(sNewTime);
		iNewTime = _ttoi(sNewTime);
		SetWaitTime(iNewTime);
		SetIniModify();
	} else if (wndIDC == GetDlgItem(IDC_WALL_EDIT_ADDCLASS)) {
		OnBnClickedButtonAddclasslist();
	} else if (wndIDC == GetDlgItem(IDC_WALL_EDIT_HISTORYNUM)) {
		CString sBuf;
		m_editHistoryNum.GetWindowText(sBuf);
		UINT uNum = (UINT)_ttoi(sBuf);
		SetHistoryNum(uNum);
		SetIniModify();
	} else if (wndIDC == GetDlgItem(IDC_WALL_EDIT_CACHENUM)) {
		CString sBuf;
		m_editCacheNum.GetWindowText(sBuf);
		UINT uNum = (UINT)_ttoi(sBuf);
		SetCachePicNum(uNum);
		SetIniModify();
	}

//	CDialog::OnOK();
}

void CWallChangerDlg::OnCancel()
{
	theApp.GetMainWnd()->DestroyWindow();

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
}

void CWallChangerDlg::OnBnClickedButtonRandpic()
{
	if (!m_muxRandPic.Lock(0))
		return;

	if (m_uCachePicNum && ::g_pWallThreadImageCache->GetCount()) {
		SetRandWallPager(false);
	} else {
		while (!SetRandWallPager(true)) {
			if (::g_pWallEnablePicList->IsEmpty()) {
				StopTimer();
				break;
			}
		}
	}

	CString sTime;
	sTime.Format(_T("%d"), m_uWaitTime);
	m_staticTime.SetWindowText(sTime);

	m_muxRandPic.Unlock();
	return;
}

void CWallChangerDlg::OnBnClickedButtonDelpic()
{
	DeletePicFile(m_sNowPicPath, true);

	if (!PathFileExists(m_sNowPicPath))
		OnBnClickedButtonRandpic();
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
}

void CWallChangerDlg::OnCbnSelchangeComboImageloaderror()
{
	m_comboxImageLoadError.GetLBText(m_comboxImageLoadError.GetCurSel(), m_sComboxMsg);
	SetIniModify();
}

void CWallChangerDlg::OnBnClickedWallCheckShowdirloaderror()
{
	m_bShowDirLoadError = m_checkShowLoadDirError.GetCheck() == BST_CHECKED;
	SetIniModify();
}

void CWallChangerDlg::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	DoSize();
}

void CWallChangerDlg::OnLvnItemchangedListClass(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if (m_bInit && (pNMLV->uOldState==INDEXTOSTATEIMAGEMASK(1)) && (pNMLV->uNewState==INDEXTOSTATEIMAGEMASK(2))) {
		// Check this Item
		bool bEmptyEnable = (::g_pWallEnablePicList->GetCount() == 0);
		m_listClass.SetItemEnable(pNMLV->iItem, true);
		if (bEmptyEnable && ::g_pWallEnablePicList->GetCount())
			StartTimer();
		m_listClass.SetIniModify();
	} else if (m_bInit && (pNMLV->uOldState==INDEXTOSTATEIMAGEMASK(2)) && (pNMLV->uNewState==INDEXTOSTATEIMAGEMASK(1))) {
		// UnCheck this Item
		m_listClass.SetItemEnable(pNMLV->iItem, false);
		if (::g_pWallEnablePicList->GetCount() == 0)
			StopTimer();
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

void CWallChangerDlg::OnTimer(UINT nIDEvent)
{
	switch (nIDEvent) {
	case KDT_CHANGEWALL:
		CString sTime;
		int iTime;
		if (m_uCachePicNum && (!m_bSetCachePicNum) && (::g_pWallEnablePicList->GetCount() > m_uCachePicNum))
			m_bSetCachePicNum = SetCachePicNum(m_uCachePicNum);

		m_iTestOfflineCount--;
		if (m_iTestOfflineCount < 0) {
			m_iTestOfflineCount = TESTOFFLINECOUNT;
			::g_pWallThreadFindPic->TestOfflineDirItem();
		}

		m_staticTime.GetWindowText(sTime);
		iTime = _ttoi(sTime) - 1;

		if (iTime < 0) {
			OnBnClickedButtonRandpic();
		} else {
			sTime.Format(_T("%d"), iTime);
			m_staticTime.SetWindowText(sTime);
		}
		break;
	}

	__super::OnTimer(nIDEvent);
}

LRESULT CWallChangerDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WMU_UPDATE_CACHENUM:
		{
			CString sNum;
			sNum.Format(_T("%d"), ::g_pWallThreadImageCache->GetCount());
			m_staticCachePicNum.SetWindowText(sNum);
		}
		break;
	case WMU_UPDATE_TOTALNUM:
		{
			CString sNum;
			sNum.Format(_T("%d"), ::g_pWallEnablePicList->GetCount());
			m_staticPicTotalNum.SetWindowText(sNum);
		}
		break;
	case WMU_FIRST_FIND_PIC:
		m_bSetCachePicNum = SetCachePicNum(m_uCachePicNum);
		SetRandWallPager(true);
		if (m_uWaitTime)
			StartTimer();
		break;
	case WM_TIMER:
		break;
	case WM_COMMAND:
		{
			UINT nID = LOWORD(wParam);
			switch (nID) {
			// Tray Menu
			case IDS_TRAY_RANDPIC:
				OnBnClickedButtonRandpic();
				break;
			case IDS_TRAY_DELNOWPIC:
				OnBnClickedButtonDelpic();
				break;
			// ClassList
			case IDS_WALL_MENU_ADDENABLECLASSLIST:
				m_listClass.SetSelectItemCheckState(true);
				m_listClass.SetIniModify();
				break;
			case IDS_WALL_MENU_DELCLASSLIST:
				m_listClass.DeleteSelectItem();
				m_listClass.SetIniModify();
				break;
			case IDS_WALL_MENU_UPDATEALLDIRFILEFIND:
				m_listClass.UpdateSelectItemFileFindNum();
				break;
			// DirList
			case IDS_WALL_MENU_ADDDIRPATH:
				if (m_listClass.GetSelectedCount() != 1) {
					MessageBox(CResString(IDS_WALL_CANONLYCHOICEONECLASS), NULL, MB_OK|MB_ICONERROR);
					break;
				} else {
					CString sDirPath;
					if (ChooseFolder(sDirPath, m_hWnd)) {
						m_pCurListDirPath->AddItem(sDirPath);
						m_pCurListDirPath->SetIniModify();
						m_listClass.SetIniModify();
					}
					Invalidate();
				}
				break;
			case IDS_WALL_MENU_UPDATEDIRFILEFIND:
				m_pCurListDirPath->UpdateSelectItemFileFindNum();
				m_pCurListDirPath->SetIniModify();
				break;
			case IDS_WALL_MENU_DELDIRPATH:
				m_pCurListDirPath->DeleteSelectItem();
				m_pCurListDirPath->SetIniModify();
				m_listClass.SetIniModify();
				break;
			}
		}
		break;
	}

	return __super::DefWindowProc(message, wParam, lParam);
}
