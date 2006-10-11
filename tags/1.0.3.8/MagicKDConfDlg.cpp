#include "stdafx.h"
#include "Resource.h"
#include "Language.h"
#include "Others.h"
#include "MagicKD.h"
#include "MagicKDDlg.h"
#include "KDAppVer.h"

#include "MagicKDConfDlg.h"

static enum {
	KDT_UPDATE				= 1
};

IMPLEMENT_DYNAMIC(CMagicKDConfDlg, CDialog)
CMagicKDConfDlg::CMagicKDConfDlg(CWnd* pParent /*=NULL*/)
	:	CDialog(CMagicKDConfDlg::IDD, pParent), m_bInit(false), m_uUpdateTimer(0), m_bOnUpdate(false), m_bOnBTNUpdate(false)
{
}

CMagicKDConfDlg::~CMagicKDConfDlg()
{
}

BOOL CMagicKDConfDlg::OnInitDialog()
{
	g_pTheLog->Log(_T("Init MagicKDConfDlg"), CKDLog::LOGLV_INFO);
	CDialog::OnInitDialog();

	GetDlgItem(IDC_CONF_STATIC_VERSION)->SetWindowText(theApp.GetAppProductVer());

	while (m_combo_Language.GetCount())
		m_combo_Language.DeleteString(0);
	m_combo_Language.InsertString(0, _T("English"));
	m_combo_Language.SetCurSel(0);
	int i;
	for (i=0 ; i<g_iLanguageCount ; i++) {
		if (!g_aLanguages[i].bSupported && g_aLanguages[i].lid != 0 && IsLangFileExists(g_aLanguages[i].lid)) {
			int iCur = m_combo_Language.InsertString(-1, g_aLanguages[i].pszLocale);
			if (i == (int)g_uCurrentLang)
				m_combo_Language.SetCurSel(iCur);
		}
	}

	if (g_pTheConf->m_General_bStartMin)
		m_checkStartMin.SetCheck(BST_CHECKED);
	else
		m_checkStartMin.SetCheck(BST_UNCHECKED);

	if (g_pTheConf->m_General_bShowCloseWindow)
		m_checkShowCloseWindow.SetCheck(BST_CHECKED);
	else
		m_checkShowCloseWindow.SetCheck(BST_UNCHECKED);

	m_sliderTransparency.SetRange(50, 255);
	m_sliderTransparency.SetPos(g_pTheConf->m_General_uTransparency);

	CString sPath;
	sPath.Format(_T("%s%s"), theApp.GetAppDir(), _T("KDUpdater.exe"));
	CKDAppVer KDUpdaterVer((LPCTSTR)CGetFileVersion(sPath));
	if (PathFileExists(sPath) && (KDUpdaterVer >= CKDAppVer(_T("1.0.0.8")))) {
		g_pTheLog->Log(_T("CMagicKDConfDlg::OnInitDialog() Check Update"), CKDLog::LOGLV_DEBUG);
		// Init Update Information
		m_KDUpdater.SetParentWnd(GetSafeHwnd());
		m_KDUpdater.SetKDUpdaterPath(sPath);
		m_KDUpdater.SetAppMainWnd(theApp.m_pMainWnd->GetSafeHwnd());
		m_KDUpdater.SetUpdateWorkDir(theApp.GetAppDir());
		m_KDUpdater.SetUpdateListURL(_T("http://svn.tsaikd.org/tsaikd/MagicKD/ReleaseHistory/UpdateList.ini"));
		m_KDUpdater.SetUpdatePostCmd(theApp.GetAppPath());

		sPath = _T("MagicKD.exe");
		m_KDUpdater.AddUpdateFile(sPath, theApp.GetAppProductVer());
		sPath = _T("lib/cximage.dll");
		m_KDUpdater.AddUpdateFile(sPath, CGetFileVersion(sPath));
		sPath = _T("lib/sqlite.dll");
		m_KDUpdater.AddUpdateFile(sPath, CGetFileVersion(sPath));
		sPath = _T("lib/Icons.dll");
		m_KDUpdater.AddUpdateFile(sPath, CGetFileVersion(sPath));
		sPath = _T("lang/zh-TW.dll");
		if (PathFileExists(sPath))
			m_KDUpdater.AddUpdateFile(sPath, CGetFileVersion(sPath));

		if (g_pTheConf->m_General_bCheckUpdate) {
			((CButton *)GetDlgItem(IDC_CONF_CHECK_CHECKUPDATE))->SetCheck(BST_CHECKED);
			StartUpdateTimer();
		} else {
			((CButton *)GetDlgItem(IDC_CONF_CHECK_CHECKUPDATE))->SetCheck(BST_UNCHECKED);
		}
	} else {
		GetDlgItem(IDC_CONF_BTN_CHECKUPDATE)->EnableWindow(FALSE);
		GetDlgItem(IDC_CONF_CHECK_CHECKUPDATE)->EnableWindow(FALSE);

		if (PathFileExists(sPath)) {
			if (IDOK == MessageBox(CResString(IDS_CONF_MSG_KDUPDATERTOOOLD), NULL, MB_OKCANCEL | MB_ICONINFORMATION)) {
				CKDGetHttpFile fileDL(_T("http://svn.tsaikd.org/tsaikd/KDUpdater/KDUpdater.exe"), sPath);
				::PostMessage(GetParent()->GetSafeHwnd(), WM_COMMAND, IDS_TRAY_RESTART, 0);
			}
		}
	}

	m_bInit = true;
	DoSize();
	Localize();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX 屬性頁應傳回 FALSE
}

void CMagicKDConfDlg::DoSize()
{
	if (!m_bInit)
		return;

	int iMarginRight = 15;
	int iMarginBottom = 15;

	KDMoveDlgItem(GetDlgItem(IDC_CONF_PROGRESS_UPDATE), this,
		KDMOVEDLGITEM_WAY_RIGHT | KDMOVEDLGITEM_WAY_F_INSIDE, iMarginRight, true);

	KDMoveDlgItem(GetDlgItem(IDC_CONF_STATIC_VERSION), this,
		KDMOVEDLGITEM_WAY_RIGHT | KDMOVEDLGITEM_WAY_F_INSIDE, iMarginRight);
	KDMoveDlgItem(GetDlgItem(IDC_CONF_STATIC_VERSION), this,
		KDMOVEDLGITEM_WAY_BOTTOM | KDMOVEDLGITEM_WAY_F_INSIDE, iMarginBottom);

	Invalidate();
}

void CMagicKDConfDlg::Localize()
{
	if (!m_bInit)
		return;

	GetDlgItem(IDC_CONF_BTN_RESTART)->SetWindowText(CResString(IDS_CONF_BTN_RESTART));
	GetDlgItem(IDC_CONF_BTN_CHECKUPDATE)->SetWindowText(CResString(IDS_CONF_BTN_CHECKUPDATE));
	GetDlgItem(IDC_CONF_CHECK_STARTMIN)->SetWindowText(CResString(IDS_CONF_CHECK_STARTMIN));
	GetDlgItem(IDC_CONF_CHECK_SHOWCLOSEWINDOW)->SetWindowText(CResString(IDS_CONF_CHECK_SHOWCLOSEWINDOW));
	GetDlgItem(IDC_CONF_CHECK_CHECKUPDATE)->SetWindowText(CResString(IDS_CONF_CHECK_CHECKUPDATE));
	GetDlgItem(IDC_CONF_STATIC_TRANSPARENCY)->SetWindowText(CResString(IDS_CONF_STATIC_TRANSPARENCY));
}

void CMagicKDConfDlg::UpdateFuncCheck()
{
	if (g_pMagicKDDlg) {
		CButton *pBtn;

		pBtn = (CButton *)GetDlgItem(IDC_CONF_CHECK_WALLCHANGER);
		pBtn->SetCheck(g_pMagicKDDlg->m_pWallChangerDlg ? BST_CHECKED : BST_UNCHECKED);

		pBtn = (CButton *)GetDlgItem(IDC_CONF_CHECK_FINDDF);
		pBtn->SetCheck(g_pMagicKDDlg->m_pFindDupFileDlg ? BST_CHECKED : BST_UNCHECKED);

		pBtn = (CButton *)GetDlgItem(IDC_CONF_CHECK_PICCOLLECTOR);
		pBtn->SetCheck(g_pMagicKDDlg->m_pPicCollectorDlg ? BST_CHECKED : BST_UNCHECKED);
	}
}

UINT CMagicKDConfDlg::StartUpdateTimer()
{
	GetDlgItem(IDC_CONF_BTN_CHECKUPDATE)->EnableWindow(FALSE);
	if (m_uUpdateTimer)
		StopUpdateTimer();

	m_uUpdateTimer = SetTimer(KDT_UPDATE, 86400*1000, NULL);
	CreateThread(NULL, 0, &CMagicKDConfDlg::_Init_CheckUpdate, (LPVOID) this, 0, NULL);
	return m_uUpdateTimer;
}

UINT CMagicKDConfDlg::StopUpdateTimer()
{
	if (!m_uUpdateTimer)
		return 0;

	UINT uRes = KillTimer(m_uUpdateTimer);
	m_uUpdateTimer = 0;

	return uRes;
}

void CMagicKDConfDlg::_DoCheckUpdate()
{
	if (m_bOnUpdate)
		return;

	GetDlgItem(IDC_CONF_BTN_CHECKUPDATE)->EnableWindow(FALSE);
	m_bOnUpdate = true;

	if (GetOnInternet() != 0) {
		m_KDUpdater.CloseKDUpdater();
		MessageBox(CResString(IDS_CONF_MSG_UPDATECONNECTFAILED), NULL, MB_OK | MB_ICONERROR);
	} else if (m_KDUpdater.IsNeedUpdate()) {
		if (IDYES == MessageBox(CResString(IDS_CONF_MSG_WANTUPDATEORNOT), NULL, MB_YESNO | MB_ICONQUESTION)) {
			m_KDUpdater.DoAppUpdate(1000, theApp.GetMainWnd()->GetSafeHwnd());
			theApp.Quit();
		} else {
			m_KDUpdater.CloseKDUpdater();
		}
	} else if (m_KDUpdater.m_bNeedRevert) {
		if (IDYES == MessageBox(CResString(IDS_CONF_MSG_WANTREVERTORNOT), NULL, MB_YESNO | MB_ICONQUESTION)) {
			m_KDUpdater.DoAppUpdate(1000, theApp.GetMainWnd()->GetSafeHwnd());
			theApp.Quit();
		} else {
			m_KDUpdater.CloseKDUpdater();
		}
	} else {
		m_KDUpdater.CloseKDUpdater();
		if (m_bOnBTNUpdate) {
			MessageBox(CResString(IDS_CONF_MSG_NOUPDATE), NULL, MB_OK | MB_ICONINFORMATION);
			m_bOnBTNUpdate = false;
		}
	}

	m_bOnUpdate = false;
	if (!theApp.IsOnQuit())
		GetDlgItem(IDC_CONF_BTN_CHECKUPDATE)->EnableWindow();
}

BEGIN_MESSAGE_MAP(CMagicKDConfDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CONF_CHECK_STARTMIN		, OnBnClickedCheckConfStartmin)
	ON_BN_CLICKED(IDC_CONF_CHECK_CHECKUPDATE	, OnBnClickedConfCheckCheckupdate)
	ON_BN_CLICKED(IDC_CONF_CHECK_SHOWCLOSEWINDOW, OnBnClickedConfCheckShowclosewindow)
	ON_BN_CLICKED(IDC_CONF_CHECK_WALLCHANGER	, OnBnClickedWallchangercheck)
	ON_BN_CLICKED(IDC_CONF_CHECK_FINDDF			, OnBnClickedConfCheckFinddf)
	ON_BN_CLICKED(IDC_CONF_CHECK_PICCOLLECTOR	, &CMagicKDConfDlg::OnBnClickedConfPiccollector)
	ON_BN_CLICKED(IDC_CONF_BTN_RESTART			, OnBnClickedConfBtnRestart)
	ON_BN_CLICKED(IDC_CONF_BTN_CHECKUPDATE		, OnBnClickedConfBtnCheckupdate)
	ON_CBN_SELCHANGE(IDC_CONF_COMBO_LANGUAGE	, OnCbnSelchangeConfComboLanguage)
END_MESSAGE_MAP()

void CMagicKDConfDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CONF_COMBO_LANGUAGE		, m_combo_Language);
	DDX_Control(pDX, IDC_CONF_CHECK_STARTMIN		, m_checkStartMin);
	DDX_Control(pDX, IDC_CONF_CHECK_SHOWCLOSEWINDOW	, m_checkShowCloseWindow);
	DDX_Control(pDX, IDC_CONF_CHECK_CHECKUPDATE		, m_check_CheckUpdate);
	DDX_Control(pDX, IDC_CONF_SLIDER_TRANSPARENCY	, m_sliderTransparency);
	DDX_Control(pDX, IDC_CONF_PROGRESS_UPDATE		, m_progress_Update);
}

void CMagicKDConfDlg::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	DoSize();
}

void CMagicKDConfDlg::OnTimer(UINT nIDEvent)
{
	switch (nIDEvent) {
	case KDT_UPDATE:
		CreateThread(NULL, 0, &CMagicKDConfDlg::_Init_CheckUpdate, (LPVOID) this, 0, NULL);
		break;
	}

	CDialog::OnTimer(nIDEvent);
}

void CMagicKDConfDlg::OnBnClickedConfBtnRestart()
{
	::PostMessage(GetParent()->GetSafeHwnd(), WM_COMMAND, IDS_TRAY_RESTART, 0);
}

void CMagicKDConfDlg::OnBnClickedConfBtnCheckupdate()
{
	GetDlgItem(IDC_CONF_BTN_CHECKUPDATE)->EnableWindow(FALSE);
	m_bOnBTNUpdate = true;
	StartUpdateTimer();
}

void CMagicKDConfDlg::OnBnClickedCheckConfStartmin()
{
	g_pTheConf->m_General_bStartMin = m_checkStartMin.GetCheck()==BST_CHECKED;
}

void CMagicKDConfDlg::OnBnClickedConfCheckShowclosewindow()
{
	g_pTheConf->m_General_bShowCloseWindow = m_checkShowCloseWindow.GetCheck()==BST_CHECKED;
}

void CMagicKDConfDlg::OnBnClickedConfCheckCheckupdate()
{
	g_pTheConf->m_General_bCheckUpdate = m_check_CheckUpdate.GetCheck()==BST_CHECKED;
	if (g_pTheConf->m_General_bCheckUpdate)
		StartUpdateTimer();
	else
		StopUpdateTimer();
}

void CMagicKDConfDlg::OnBnClickedWallchangercheck()
{
	bool bEnable = ((CButton*)GetDlgItem(IDC_CONF_CHECK_WALLCHANGER))->GetCheck()==BST_CHECKED;
	g_pMagicKDDlg->SetFuncEnable(CMagicKDDlg::eFunc_WallChanger, bEnable);
	g_pTheConf->m_FuncList_bWallChanger = bEnable;
}

void CMagicKDConfDlg::OnBnClickedConfCheckFinddf()
{
	bool bEnable = ((CButton*)GetDlgItem(IDC_CONF_CHECK_FINDDF))->GetCheck()==BST_CHECKED;
	g_pMagicKDDlg->SetFuncEnable(CMagicKDDlg::eFunc_FindDupFile, bEnable);
	g_pTheConf->m_FuncList_bFindDupFile = bEnable;
}

void CMagicKDConfDlg::OnBnClickedConfPiccollector()
{
	bool bEnable = ((CButton*)GetDlgItem(IDC_CONF_CHECK_PICCOLLECTOR))->GetCheck()==BST_CHECKED;
	g_pMagicKDDlg->SetFuncEnable(CMagicKDDlg::eFunc_PicCollector, bEnable);
	g_pTheConf->m_FuncList_bPicCollector = bEnable;
}

void CMagicKDConfDlg::OnCbnSelchangeConfComboLanguage()
{
	CString sLang;
	m_combo_Language.GetLBText(m_combo_Language.GetCurSel(), sLang);

	int i;
	for (i=0 ; i<g_iLanguageCount ; i++) {
		if (CString(g_aLanguages[i].pszLocale) == sLang) {
			g_pTheConf->m_General_uLangID = (UINT)g_aLanguages[i].lid;
			g_pMagicKDDlg->Localize();
		}
	}
}

LRESULT CMagicKDConfDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_HSCROLL:
		{
		g_pTheConf->m_General_uTransparency = m_sliderTransparency.GetPos();
		g_pMagicKDDlg->SetTransparency((BYTE)g_pTheConf->m_General_uTransparency);
		}
		break;
	}

	return __super::DefWindowProc(message, wParam, lParam);
}
