#include "stdafx.h"
#include "Language.h"
#include "Others.h"
#include "WallOthers.h"
#include "MagicKD.h"
#include "MagicKDDlg.h"

#include "WallChangerDlg.h"

#define DEFAULT_ENABLETIP			true
#define DEFAULT_SHOWDIRLOADERROR	true
#define DEFAULT_COMBOX_MSG			CResString(IDS_WALL_COMBOX_ASKUSER)

#define TESTOFFLINECOUNT			30

enum {
	KDT_CHANGEWALL		= 1,
	KDT_OFFLINECOUNT
};

// GLobal variable
CWallChangerDlg *g_pWallChangerDlg = NULL;
#include "WallConf.h"
CWallConf *g_pWallConf = NULL;
#include "WallEnablePicList.h"
CWallEnablePicList *g_pWallEnablePicList = NULL;
#include "WallThreadFindPic.h"
CWallThreadFindPic *g_pWallThreadFindPic = NULL;

IMPLEMENT_DYNAMIC(CWallChangerDlg, CDialog)
CWallChangerDlg::CWallChangerDlg(CWnd* pParent /*=NULL*/)
	:	CDialog(CWallChangerDlg::IDD, pParent), m_bInit(false), m_pCurListDirPath(NULL),
		m_iTestOfflineCount(TESTOFFLINECOUNT), m_uTimer(0), m_bShowDirLoadError(true)
{
}

CWallChangerDlg::~CWallChangerDlg()
{
	if (::g_pWallConf) {
		delete ::g_pWallConf;
		::g_pWallConf = NULL;
	}

	if (::g_pWallChangerDlg)
		::g_pWallChangerDlg = NULL;
}

BOOL CWallChangerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (!::g_pWallChangerDlg)
		g_pWallChangerDlg = this;
	if (!::g_pWallEnablePicList)
		::g_pWallEnablePicList = new CWallEnablePicList;
	if (!::g_pWallThreadFindPic)
		::g_pWallThreadFindPic = new CWallThreadFindPic;
	::g_pWallThreadFindPic->SetMsgWnd(m_hWnd);

	srand((UINT)time(NULL));
	pTheTray->InsertMenu(0, MF_STRING | MF_BYPOSITION, IDS_TRAY_MOVEPIC		, CResString(IDS_TRAY_MOVEPIC));
	pTheTray->InsertMenu(0, MF_STRING | MF_BYPOSITION, IDS_TRAY_DELNOWPIC	, CResString(IDS_TRAY_DELNOWPIC));
	pTheTray->InsertMenu(0, MF_STRING | MF_BYPOSITION, IDS_TRAY_RANDPIC		, CResString(IDS_TRAY_RANDPIC), true);
	pTheAppEndDlg->SignWnd(GetSafeHwnd(), 6);

	m_cIni.SetPathName(theApp.m_sAppDir + _T("WallChanger.ini"));
	if (!::g_pWallConf)
		::g_pWallConf = new CWallConf;
	::g_pWallConf->Init(&m_cIni);

	m_sTempFilePath = CTempFilePath(NULL, NULL, _T(".jpg"));
	GetDlgItem(IDC_WALL_STATIC_WAITTIME)	->SetWindowText(CResString(IDS_WALL_STATIC_WAITTIME));
	GetDlgItem(IDC_WALL_STATIC_PICTOTALNUM)	->SetWindowText(CResString(IDS_WALL_STATIC_PICTOTALNUM));
	GetDlgItem(IDC_WALL_BTN_RANDPIC)		->SetWindowText(CResString(IDS_WALL_BTN_RANDPIC));
	GetDlgItem(IDC_WALL_BTN_DELPIC)			->SetWindowText(CResString(IDS_WALL_BTN_DELPIC));
	GetDlgItem(IDC_WALL_BTN_MOVEPIC)		->SetWindowText(CResString(IDS_WALL_BTN_MOVEPIC));
	GetDlgItem(IDC_WALL_BTN_ADDCLASS)		->SetWindowText(CResString(IDS_WALL_BTN_ADDCLASS));
	m_btn_EnableToolTip.SetWindowText(CResString(::g_pWallConf->m_General_bEnableTip ? IDS_ALL_BTN_DISABLETIP : IDS_ALL_BTN_ENABLETIP));
	((CComboBox *)GetDlgItem(IDC_WALL_COMBO_IMAGELOADERROR))->AddString(CResString(IDS_WALL_COMBOX_ASKUSER));
	((CComboBox *)GetDlgItem(IDC_WALL_COMBO_IMAGELOADERROR))->AddString(CResString(IDS_WALL_COMBOX_ASKDELETE));
	((CComboBox *)GetDlgItem(IDC_WALL_COMBO_IMAGELOADERROR))->AddString(CResString(IDS_WALL_COMBOX_DELETE));
	((CComboBox *)GetDlgItem(IDC_WALL_COMBO_IMAGELOADERROR))->AddString(CResString(IDS_WALL_COMBOX_EXPLORE));
	((CComboBox *)GetDlgItem(IDC_WALL_COMBO_IMAGELOADERROR))->AddString(CResString(IDS_WALL_COMBOX_NULL));

	m_iTestOfflineCount = TESTOFFLINECOUNT;
	SetTimer(KDT_OFFLINECOUNT, 1000, NULL);

	m_comboxImageLoadError.SetCurSel(m_comboxImageLoadError.FindString(-1, ::g_pWallConf->m_General_sComboxMsg));

	GetDlgItem(IDC_WALL_CHECK_SHOWDIRLOADERROR)->SetWindowText(CResString(IDS_WALL_CHECK_SHOWDIRLOADERROR));
	m_bShowDirLoadError = ::g_pWallConf->m_General_bShowDirLoadError;
	if (m_bShowDirLoadError)
		m_checkShowLoadDirError.SetCheck(BST_CHECKED);
	else
		m_checkShowLoadDirError.SetCheck(BST_UNCHECKED);

	CString sBuf;
	sBuf.Format(_T("%d"), (UINT)::g_pWallConf->m_General_uWaitTime);
	m_editWaitTime.SetWindowText(sBuf);
	m_staticTime.SetWindowText(sBuf);

	CRect rcWin;
	m_listDirPath.Init(&m_cIni, _T(""), &m_bShowDirLoadError);
	m_listDirPath.ShowWindow(SW_SHOW);
	m_listDirPath.GetClientRect(rcWin);
	m_listClass.Init(&m_cIni, rcWin, &m_bShowDirLoadError);

	SetHistoryNum(::g_pWallConf->m_General_uPicPathHistory);
	EnableToolTips(::g_pWallConf->m_General_bEnableTip);

	m_bInit = true;
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX 屬性頁應傳回 FALSE
}

void CWallChangerDlg::OnDestroy()
{
	pTheAppEndDlg->ProgressStepIt(GetSafeHwnd(), _T("Closing\tWallChanger\tDialog"));

	pTheAppEndDlg->ProgressStepIt(GetSafeHwnd(), _T("Stoping\tWallChanger\tTimer"));
	StopTimer();
	KillTimer(KDT_OFFLINECOUNT);

	pTheAppEndDlg->ProgressStepIt(GetSafeHwnd(), _T("Closing\tWallChanger\tFind Picture Thread"));
	if (::g_pWallThreadFindPic) {
		delete ::g_pWallThreadFindPic;
		::g_pWallThreadFindPic = NULL;
	}

	pTheAppEndDlg->ProgressStepIt(GetSafeHwnd(), _T("Closing\tWallChanger\tEnable Picture List Thread"));
	if (::g_pWallEnablePicList) {
		delete ::g_pWallEnablePicList;
		::g_pWallEnablePicList = NULL;
	}

	CDialog::OnDestroy();

	pTheAppEndDlg->ProgressStepIt(GetSafeHwnd(), _T("Deleting\tWallChanger\tTemp File"));
	if (PathFileExists(m_sTempFilePath))
		DeleteFile(m_sTempFilePath);

	pTheAppEndDlg->ProgressStepIt(GetSafeHwnd(), _T("Deleting\tWallChanger\tTray Menu"));
	pTheTray->RemoveTrayMenuItem(CResString(IDS_TRAY_RANDPIC));
	pTheTray->RemoveTrayMenuItem(CResString(IDS_TRAY_DELNOWPIC));
	pTheTray->RemoveTrayMenuItem(CResString(IDS_TRAY_MOVEPIC));

	pTheAppEndDlg->UnsignWnd(GetSafeHwnd());
}

void CWallChangerDlg::DoSize()
{
	if (!m_bInit)
		return;

	int iMarginRight = 15;
	int iMarginBottom = 15;

	KDMoveDlgItem(GetDlgItem(IDC_WALL_BTN_ENABLETOOLTIP), this,
		KDMOVEDLGITEM_WAY_RIGHT | KDMOVEDLGITEM_WAY_F_INSIDE, iMarginRight);
	KDMoveDlgItem(GetDlgItem(IDC_WALL_EDIT_WAITTIME), GetDlgItem(IDC_WALL_BTN_ENABLETOOLTIP),
		KDMOVEDLGITEM_WAY_LEFT | KDMOVEDLGITEM_WAY_F_OUTSIDE, 5);
	KDMoveDlgItem(GetDlgItem(IDC_WALL_STATIC_RESTTIME), GetDlgItem(IDC_WALL_EDIT_WAITTIME),
		KDMOVEDLGITEM_WAY_LEFT | KDMOVEDLGITEM_WAY_F_OUTSIDE, 5);
	KDMoveDlgItem(GetDlgItem(IDC_WALL_STATIC_WAITTIME), GetDlgItem(IDC_WALL_STATIC_RESTTIME),
		KDMOVEDLGITEM_WAY_LEFT | KDMOVEDLGITEM_WAY_F_OUTSIDE, 0);
	KDMoveDlgItem(GetDlgItem(IDC_WALL_EDIT_HISTORYNUM), GetDlgItem(IDC_WALL_STATIC_WAITTIME),
		KDMOVEDLGITEM_WAY_LEFT | KDMOVEDLGITEM_WAY_F_OUTSIDE, 5);
	KDMoveDlgItem(GetDlgItem(IDC_WALL_COMBO_IMAGELOADERROR), GetDlgItem(IDC_WALL_EDIT_HISTORYNUM),
		KDMOVEDLGITEM_WAY_LEFT | KDMOVEDLGITEM_WAY_F_OUTSIDE, 5);

	KDMoveDlgItem(GetDlgItem(IDC_WALL_CHECK_SHOWDIRLOADERROR), this,
		KDMOVEDLGITEM_WAY_RIGHT | KDMOVEDLGITEM_WAY_F_INSIDE, iMarginRight);
	KDMoveDlgItem(GetDlgItem(IDC_WALL_STATIC_NOWPICPATH), this,
		KDMOVEDLGITEM_WAY_RIGHT | KDMOVEDLGITEM_WAY_F_INSIDE, iMarginRight, true);

	KDMoveDlgItem(GetDlgItem(IDC_WALL_EDIT_ADDCLASS), this,
		KDMOVEDLGITEM_WAY_BOTTOM | KDMOVEDLGITEM_WAY_F_INSIDE, iMarginBottom);
	KDMoveDlgItem(GetDlgItem(IDC_WALL_BTN_ADDCLASS), this,
		KDMOVEDLGITEM_WAY_BOTTOM | KDMOVEDLGITEM_WAY_F_INSIDE, iMarginBottom);
	KDMoveDlgItem(GetDlgItem(IDC_WALL_LIST_CLASS), GetDlgItem(IDC_WALL_EDIT_ADDCLASS),
		KDMOVEDLGITEM_WAY_TOP | KDMOVEDLGITEM_WAY_F_OUTSIDE, 10, true);

	KDMoveDlgItem(GetDlgItem(IDC_WALL_LIST_DIRPATH), this,
		KDMOVEDLGITEM_WAY_RIGHT | KDMOVEDLGITEM_WAY_F_INSIDE, iMarginRight, true);
	KDMoveDlgItem(GetDlgItem(IDC_WALL_LIST_DIRPATH), this,
		KDMOVEDLGITEM_WAY_BOTTOM | KDMOVEDLGITEM_WAY_F_INSIDE, iMarginBottom, true);
	if (m_pCurListDirPath) {
		CRect rcDirList;
		GetDlgItem(IDC_WALL_LIST_DIRPATH)->GetWindowRect(rcDirList);
		ScreenToClient(rcDirList);
		m_pCurListDirPath->MoveWindow(rcDirList, FALSE);
	}

	Invalidate();
}

void CWallChangerDlg::SetHistoryNum(UINT uNum)
{
	CString sNum;
	if (uNum < 1)
		uNum = 1;
	::g_pWallConf->m_General_uPicPathHistory = uNum;
	sNum.Format(_T("%d"), uNum);
	m_editHistoryNum.SetWindowText(sNum);
}

void CWallChangerDlg::SetWaitTime(UINT uWaitTime)
{
	CString sNewTime, sRestTime;
	int iRestTime;
	if (uWaitTime) {
		m_staticTime.GetWindowText(sRestTime);
		iRestTime = _ttoi(sRestTime) - ::g_pWallConf->m_General_uWaitTime + uWaitTime;
		if (iRestTime < 0)
			iRestTime = 0;
		sRestTime.Format(_T("%d"), iRestTime);
		m_staticTime.SetWindowText(sRestTime);
		::g_pWallConf->m_General_uWaitTime = uWaitTime;
		sNewTime.Format(_T("%d"), uWaitTime);
		m_editWaitTime.SetWindowText(sNewTime);

		if (::g_pWallConf->m_General_uWaitTime)
			StartTimer();
	} else {
		::g_pWallConf->m_General_uWaitTime = 0;
		m_staticTime.SetWindowText(_T("0"));
		m_editWaitTime.SetWindowText(_T("0"));

		ASSERT(StopTimer());
	}
}

//bWay:
//	true: From File List
bool CWallChangerDlg::SetRandWallPager()
{
	if (!m_muxSetRandWallPager.Lock(0))
		return false;

#define RETURN(x) {																	\
	if (x) {																		\
		m_staticNowPicPath.SetWindowText(m_sNowPicPath);							\
		pTheTray->SetTrayTip(m_sNowPicPath);										\
	} else {																		\
		m_staticNowPicPath.SetWindowText(m_sNowPicPath + _T(" -- ") +				\
			CResString(IDS_WALL_SETWALLPAGERFAIL));									\
		m_sNowPicPath = _T("");														\
		pTheTray->SetTrayTip(_T(""));												\
	}																				\
	m_muxSetRandWallPager.Unlock(); return (x);										\
}

	m_staticNowPicPath.SetWindowText(CResString(IDS_WALL_SETTINGWALLPAGER));
	m_sNowPicPath = GetRandPicPath();

	if (m_sNowPicPath.IsEmpty())
		RETURN(false);
	m_staticNowPicPath.SetWindowText(m_sNowPicPath + _T(" -- ") + CResString(IDS_WALL_SETTINGWALLPAGER));

	m_imgNowPic.Destroy();
	if (m_imgNowPic.Load(m_sNowPicPath)) {
		if (AutoPicSize(m_imgNowPic)) {
			m_imgNowPic.Save(m_sTempFilePath, CXIMAGE_FORMAT_JPG);
			SetWallpaper(m_sTempFilePath, WPSTYLE_CENTER);
		} else {
			SetWallpaper(m_sNowPicPath, WPSTYLE_CENTER);
		}
		RETURN(true);
	} else {
		// IDYES:		delete picture file
		// IDNO:		goto directory
		// IDCANCEL:	do nothing
		int iRes = IDCANCEL;
		CString sComboxMsg = ::g_pWallConf->m_General_sComboxMsg;
		if (sComboxMsg == CResString(IDS_WALL_COMBOX_NULL)) {
			iRes = IDCANCEL;
		} else if (sComboxMsg == CResString(IDS_WALL_COMBOX_DELETE)) {
			DeletePicFile(m_sNowPicPath, false);
			RETURN(false);
		} else if (sComboxMsg == CResString(IDS_WALL_COMBOX_ASKDELETE)) {
			iRes = IDYES;
		} else if (sComboxMsg == CResString(IDS_WALL_COMBOX_EXPLORE)) {
			iRes = IDNO;
		} else {
			CString sMsg;
			sMsg.Format(_T("%s\n%s"), m_sNowPicPath, CResString(IDS_WALL_MSG_IMAGELOADERROR));

			iRes = MessageBox(sMsg, NULL, MB_YESNOCANCEL | MB_ICONQUESTION);

			//CStringArray saBtnText;
			//saBtnText.SetSize(3);
			//saBtnText[0] = CResString(IDS_WALL_MSG_IMAGELOADERROR_BTN1);
			//saBtnText[1] = CResString(IDS_WALL_MSG_IMAGELOADERROR_BTN2);
			//saBtnText[2] = CResString(IDS_WALL_MSG_IMAGELOADERROR_BTN3);
			//switch(CKDMessageBox(_T("WallChanger"), sMsg, saBtnText, this, 2, 3, KDMSG_F_CANCANCEL)) {
			//case 1:
			//	iRes = IDYES;
			//	break;
			//case 2:
			//	iRes = IDNO;
			//	break;
			//default:
			//	iRes = IDCANCEL;
			//	break;
			//}
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

	if ((UINT)m_slPicPathHistory.GetCount() > (UINT)::g_pWallConf->m_General_uPicPathHistory)
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
		bRes = RemoveFileDlg(sFilePath, GetSafeHwnd(), !IsShiftPressed());
	else
		bRes = (DeleteFile(sFilePath) == TRUE);

	if (!PathFileExists(sFilePath))
		::g_pWallEnablePicList->RemoveFind(sFilePath);
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

bool CWallChangerDlg::EnableToolTips(bool bEnable/* = true*/)
{
	m_editWaitTime.EnableToolTips			(CResString(IDS_WALL_TOOLTIP_SETWAITTIME), bEnable);
	m_editAddClass.EnableToolTips			(CResString(IDS_WALL_TOOLTIP_ADDCLASS), bEnable);
	m_editHistoryNum.EnableToolTips			(CResString(IDS_WALL_TOOLTIP_SETHISTORYNUM), bEnable);

	m_btn_RandPic.EnableToolTips			(CResString(IDS_WALL_TOOLTIP_RANDPIC), bEnable);
	m_btn_DelPic.EnableToolTips				(CResString(IDS_WALL_TOOLTIP_DELNOWPIC), bEnable);
	m_btn_MovePic.EnableToolTips			(CResString(IDS_WALL_TOOLTIP_MOVEPIC), bEnable);

	m_listClass.EnableToolTips				(NULL, bEnable);
	m_listDirPath.EnableToolTips			(NULL, bEnable);

	m_comboxImageLoadError.EnableToolTips	(CResString(IDS_WALL_TOOLTIP_IMAGELOADERROR), bEnable);

	return CDialog::EnableToolTips(bEnable) != FALSE;
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
	DDX_Control(pDX, IDC_WALL_STATIC_RESTTIME		, m_staticTime);
	DDX_Control(pDX, IDC_WALL_STATIC_PICTOTALNUMSHOW, m_staticPicTotalNum);
	DDX_Control(pDX, IDC_WALL_STATIC_NOWPICPATH		, m_staticNowPicPath);
	DDX_Control(pDX, IDC_WALL_LIST_CLASS			, m_listClass);
	DDX_Control(pDX, IDC_WALL_LIST_DIRPATH			, m_listDirPath);
	DDX_Control(pDX, IDC_WALL_EDIT_WAITTIME			, m_editWaitTime);
	DDX_Control(pDX, IDC_WALL_EDIT_ADDCLASS			, m_editAddClass);
	DDX_Control(pDX, IDC_WALL_EDIT_HISTORYNUM		, m_editHistoryNum);
	DDX_Control(pDX, IDC_WALL_BTN_RANDPIC			, m_btn_RandPic);
	DDX_Control(pDX, IDC_WALL_BTN_DELPIC			, m_btn_DelPic);
	DDX_Control(pDX, IDC_WALL_BTN_ENABLETOOLTIP		, m_btn_EnableToolTip);
	DDX_Control(pDX, IDC_WALL_BTN_ADDCLASS			, m_btn_AddClass);
	DDX_Control(pDX, IDC_WALL_CHECK_SHOWDIRLOADERROR, m_checkShowLoadDirError);
	DDX_Control(pDX, IDC_WALL_COMBO_IMAGELOADERROR	, m_comboxImageLoadError);
	DDX_Control(pDX, IDC_WALL_BTN_MOVEPIC, m_btn_MovePic);
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
	ON_CBN_SELCHANGE(IDC_WALL_COMBO_IMAGELOADERROR, OnCbnSelchangeComboImageloaderror)
	ON_BN_CLICKED(IDC_WALL_BTN_MOVEPIC, OnBnClickedWallBtnMovepic)
END_MESSAGE_MAP()

void CWallChangerDlg::OnOK()
{
	CWnd *wndIDC = GetFocus();
	if (wndIDC == GetDlgItem(IDC_WALL_EDIT_WAITTIME)) {
		CString sNewTime;
		int iNewTime;
		m_editWaitTime.GetWindowText(sNewTime);
		iNewTime = _ttoi(sNewTime);
		SetWaitTime(iNewTime);
	} else if (wndIDC == GetDlgItem(IDC_WALL_EDIT_ADDCLASS)) {
		OnBnClickedButtonAddclasslist();
	} else if (wndIDC == GetDlgItem(IDC_WALL_EDIT_HISTORYNUM)) {
		CString sBuf;
		m_editHistoryNum.GetWindowText(sBuf);
		UINT uNum = (UINT)_ttoi(sBuf);
		SetHistoryNum(uNum);
	}

//	CDialog::OnOK();
}

void CWallChangerDlg::OnCancel()
{
	((CDialog *)GetParent())->EndDialog(IDCANCEL);

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

	while (!SetRandWallPager()) {
		if (::g_pWallEnablePicList->IsEmpty())
			break;
	}

	CString sTime;
	sTime.Format(_T("%u"), (UINT)::g_pWallConf->m_General_uWaitTime);
	m_staticTime.SetWindowText(sTime);

	m_muxRandPic.Unlock();
	return;
}

void CWallChangerDlg::OnBnClickedButtonDelpic()
{
	CString sFile = m_sNowPicPath;
	DeletePicFile(sFile, true);

	if (!PathFileExists(sFile) && (sFile==m_sNowPicPath)) {
		::g_pWallEnablePicList->RemoveFind(sFile);
		OnBnClickedButtonRandpic();
	}
}

void CWallChangerDlg::OnBnClickedWallBtnMovepic()
{
	CString sFile = m_sNowPicPath;
	CString sExt = PathFindExtension(sFile);
	CString sFilter;
	sFilter.Format(_T("Image (*%s)|*%s|All Files (*.*)|*.*|"), sExt, sExt);
	CFileDialog fileDlg(FALSE, sExt, PathFindFileName(sFile),
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, sFilter, this);
	if (IDOK == fileDlg.DoModal())
		MoveFileDlg(sFile, fileDlg.GetPathName(), GetSafeHwnd());

	if (!PathFileExists(sFile) && (sFile==m_sNowPicPath)) {
		::g_pWallEnablePicList->RemoveFind(sFile);
		OnBnClickedButtonRandpic();
	}
}

void CWallChangerDlg::OnBnClickedButtonEnabletooltip()
{
	if (::g_pWallConf->m_General_bEnableTip) {
		::g_pWallConf->m_General_bEnableTip = false;
		m_btn_EnableToolTip.SetWindowText(CResString(IDS_ALL_BTN_ENABLETIP));
	} else {
		::g_pWallConf->m_General_bEnableTip = true;
		m_btn_EnableToolTip.SetWindowText(CResString(IDS_ALL_BTN_DISABLETIP));
	}

	EnableToolTips(::g_pWallConf->m_General_bEnableTip);
}

void CWallChangerDlg::OnCbnSelchangeComboImageloaderror()
{
	m_comboxImageLoadError.GetLBText(m_comboxImageLoadError.GetCurSel(), ::g_pWallConf->m_General_sComboxMsg);
	::g_pWallConf->m_General_sComboxMsg.SetDirty();
}

void CWallChangerDlg::OnBnClickedWallCheckShowdirloaderror()
{
	m_bShowDirLoadError = m_checkShowLoadDirError.GetCheck() == BST_CHECKED;
	::g_pWallConf->m_General_bShowDirLoadError = m_bShowDirLoadError;
}

void CWallChangerDlg::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	DoSize();
}

void CWallChangerDlg::OnLvnItemchangedListClass(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if (!pNMLV)
		return;

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
		int iSelCount = m_listClass.GetSelectedCount();
		if (iSelCount == 1) {
			if (m_pCurListDirPath)
				m_pCurListDirPath->ShowWindow(SW_HIDE);
			else
				m_listDirPath.ShowWindow(SW_HIDE);

			CWallClassListItem *pListItem = (CWallClassListItem *)m_listClass.GetFirstSelectedItemLParam();
			if (pListItem)
				m_pCurListDirPath = (CWallDirListCtrl *)pListItem->GetChildDirList();
			if (m_pCurListDirPath) {
				CRect rcDirList;
				m_listDirPath.GetWindowRect(rcDirList);

				ScreenToClient(rcDirList);
				m_pCurListDirPath->MoveWindow(rcDirList);
				m_pCurListDirPath->ShowWindow(SW_SHOW);
			}
		} else {
			if (m_pCurListDirPath) {
				m_pCurListDirPath->ShowWindow(SW_HIDE);
				m_pCurListDirPath = NULL;
			}
			m_listDirPath.ShowWindow(SW_SHOW);
		}
	}

	*pResult = 0;
}

void CWallChangerDlg::OnTimer(UINT nIDEvent)
{
	switch (nIDEvent) {
	case KDT_CHANGEWALL:
		{
			CString sTime;
			int iTime;

			m_staticTime.GetWindowText(sTime);
			iTime = _ttoi(sTime) - 1;

			if (iTime < 0) {
				OnBnClickedButtonRandpic();
			} else {
				sTime.Format(_T("%d"), iTime);
				m_staticTime.SetWindowText(sTime);
			}
		}
		break;
	case KDT_OFFLINECOUNT:
		m_iTestOfflineCount--;
		if (m_iTestOfflineCount < 0) {
			m_iTestOfflineCount = TESTOFFLINECOUNT;
			::g_pWallThreadFindPic->TestOfflineDirItem();
		}
		break;
	}

	__super::OnTimer(nIDEvent);
}

LRESULT CWallChangerDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WMU_UPDATE_TOTALNUM:
		{
			CString sNum;
			sNum.Format(_T("%d"), ::g_pWallEnablePicList->GetCount());
			m_staticPicTotalNum.SetWindowText(sNum);
		}
		break;
	case WMU_FIRST_FIND_PIC:
		SetRandWallPager();
		if (::g_pWallConf->m_General_uWaitTime)
			StartTimer();
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
			case IDS_TRAY_MOVEPIC:
				OnBnClickedWallBtnMovepic();
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
