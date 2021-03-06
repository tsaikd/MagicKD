#include "stdafx.h"
#include "Resource.h"
#include "Language.h"
#include "Others.h"
#include "MagicKD.h"
#include "MagicKDDlg.h"

#include "MagicKDConfDlg.h"

enum {
	KDT_UPDATE		= 1
};

IMPLEMENT_DYNAMIC(CMagicKDConfDlg, CDialog)
CMagicKDConfDlg::CMagicKDConfDlg(CWnd* pParent /*=NULL*/)
	:	CDialog(CMagicKDConfDlg::IDD, pParent), m_bInit(false), m_uUpdateTimer(0), m_bUpdateLastest(false)
{
}

CMagicKDConfDlg::~CMagicKDConfDlg()
{
}

BOOL CMagicKDConfDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set Update Infomation
	int i;
	CString sCheckFileName;
	const int iArraySize = 4;
	m_saNowVersion.SetSize(iArraySize);
	m_aiQueryVerSize.SetSize(iArraySize);
	m_saOldAppPath.SetSize(iArraySize);
	m_saNewAppPath.SetSize(iArraySize);

	m_saNowVersion[0] = theApp.GetAppProductVer();
	m_aiQueryVerSize[0] = 7;
	m_saOldAppPath[0].Format(_T("%s.exe"), theApp.GetAppName());
	m_saNewAppPath[0].Format(_T("%s_Update.exe"), theApp.GetAppName());

	sCheckFileName = _T("lib/cximage.dll");
	i = 1;
	m_saNowVersion[i] = CGetFileVersion(sCheckFileName);
	m_aiQueryVerSize[i] = 7;
	m_saOldAppPath[i] = sCheckFileName;
	m_saNewAppPath[i] = sCheckFileName + _T(".Update");

	sCheckFileName = _T("lib/sqlite.dll");
	i = 2;
	m_saNowVersion[i] = CGetFileVersion(sCheckFileName);
	m_aiQueryVerSize[i] = 7;
	m_saOldAppPath[i] = sCheckFileName;
	m_saNewAppPath[i] = sCheckFileName + _T(".Update");

	sCheckFileName = _T("lang/zh-TW.dll");
	if (PathFileExists(sCheckFileName)) {
		i = 3;
		m_saNowVersion[i] = CGetFileVersion(sCheckFileName);
		m_aiQueryVerSize[i] = 7;
		m_saOldAppPath[i] = sCheckFileName;
		m_saNewAppPath[i] = sCheckFileName + _T(".Update");
	}

	GetDlgItem(IDC_CONF_STATIC_VERSION)->SetWindowText(theApp.GetAppProductVer());

	while (m_combo_Language.GetCount())
		m_combo_Language.DeleteString(0);
	m_combo_Language.InsertString(0, _T("English"));
	m_combo_Language.SetCurSel(0);
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

	if (!g_pTheConf->m_General_bUpdateLastest && IsAppNeedUpdate()) {
		DoAppUpdate();
		theApp.SetUpdateAppShowMsg(false);
		g_pTheConf->m_General_bUpdateLastest = m_bUpdateLastest = true;
	} else {
		if (g_pTheConf->m_General_bCheckUpdate) {
			m_check_CheckUpdate.SetCheck(BST_CHECKED);
			StartUpdateTimer();
		} else {
			m_check_CheckUpdate.SetCheck(BST_UNCHECKED);
			StopUpdateTimer();
		}
	}

	m_sliderTransparency.SetRange(50, 255);
	m_sliderTransparency.SetPos(g_pTheConf->m_General_uTransparency);

	m_bInit = true;
	DoSize();
	Localize();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX �ݩʭ����Ǧ^ FALSE
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
	CMagicKDDlg *pParentDlg = (CMagicKDDlg *)GetParent();
	if (pParentDlg) {
		CButton *pBtn;

		pBtn = (CButton *)GetDlgItem(IDC_CONF_CHECK_WALLCHANGER);
		pBtn->SetCheck(pParentDlg->m_pWallChangerDlg ? BST_CHECKED : BST_UNCHECKED);

		pBtn = (CButton *)GetDlgItem(IDC_CONF_CHECK_FINDDF);
		pBtn->SetCheck(pParentDlg->m_pFindDupFileDlg ? BST_CHECKED : BST_UNCHECKED);

		pBtn = (CButton *)GetDlgItem(IDC_CONF_CHECK_PICCOLLECTOR);
		pBtn->SetCheck(pParentDlg->m_pPicCollectorDlg ? BST_CHECKED : BST_UNCHECKED);
	}
}

int CMagicKDConfDlg::CmpFileVer(LPCTSTR lpVer1, LPCTSTR lpVer2)
{
	CString sVer1(lpVer1);
	CString sVer2(lpVer2);
	CString sBuf1, sBuf2;
	int iBuf1 = 0, iBuf2 = 0;
	int iPos1 = 0, iPos2 = 0;

	sVer1.Replace(_T(','), _T('.'));
	sVer2.Replace(_T(','), _T('.'));

	while (1) {
		sBuf1 = sVer1.Tokenize(_T("."), iPos1);
		sBuf2 = sVer2.Tokenize(_T("."), iPos2);
		if (sBuf1.IsEmpty() || sBuf2.IsEmpty())
			break;

		iBuf1 = _ttoi(sBuf1);
		iBuf2 = _ttoi(sBuf2);

		if (iBuf1 > iBuf2)
			return 1;
		if (iBuf1 < iBuf2)
			return -1;
	}

	if (!sBuf1.IsEmpty())
		return 1;
	if (!sBuf2.IsEmpty())
		return -1;

	return 0;
}

bool CMagicKDConfDlg::IsAppNeedUpdate()
{
	if (theApp.GetUpdateAppOnLineVer(_T("http://svn.tsaikd.org/tsaikd/MagicKD/ReleaseHistory/UpdateList.txt"),
		m_saOldAppPath, m_aiQueryVerSize, m_saReturnVer, m_saReturnUrl)) {
		int i, iCount = m_saNowVersion.GetCount();

		for (i=0 ; i<iCount ; i++) {
			if (!m_saReturnUrl[i].IsEmpty() && (CmpFileVer(m_saNowVersion[i], m_saReturnVer[i]) < 0))
				return true;
		}
	}

	return false;
}

void CMagicKDConfDlg::DoAppUpdate()
{
	bool bUpdate = true;
	double dPercent = 0;
	m_progress_Update.SetPos(0);
	m_progress_Update.SetRange32(0, 10000);

	CStringArray saOldAppPath;
	CStringArray saNewAppPath;

	int i, iCount = m_saReturnUrl.GetCount();
	for (i=0 ; i<iCount ; i++) {
		if (!m_saReturnUrl[i].IsEmpty() && (m_saNowVersion[i] != m_saReturnVer[i])) {
			if (!m_GetHttpFile.AddFileList(m_saReturnUrl[i], m_saNewAppPath[i])) {
				MessageBox(CResString(IDS_CONF_MSG_UPDATEFAILED), NULL, MB_OK | MB_ICONERROR);
				bUpdate = false;
				break;
			}
			saOldAppPath.Add(m_saOldAppPath[i]);
			saNewAppPath.Add(m_saNewAppPath[i]);
		}
	}

	if (bUpdate) {
		while (!m_GetHttpFile.IsDownloadAllOver()) {
			dPercent = m_GetHttpFile.GetPercentOfTotalDL();
			m_progress_Update.SetPos((int)(dPercent * 10000));
			Sleep(350);
		}
		m_progress_Update.SetPos(10000);
		g_pTheConf->m_General_bUpdateLastest = false;
		theApp.SetUpdateApp(saOldAppPath, saNewAppPath);
		theApp.Quit();
	} else {
		while (!m_GetHttpFile.IsDownloadAllOver())
			Sleep(350);

		for (i=0 ; i<iCount ; i++) {
			if (PathFileExists(saNewAppPath[i]))
				DeleteFile(saNewAppPath[i]);
		}
	}
}

UINT CMagicKDConfDlg::StartUpdateTimer()
{
	if (m_uUpdateTimer)
		return 0;

	if (IsAppNeedUpdate())
		OnBnClickedConfBtnCheckupdate();
	m_uUpdateTimer = SetTimer(KDT_UPDATE, 86400, NULL);
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
		if (IsAppNeedUpdate())
			OnBnClickedConfBtnCheckupdate();
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
	if (GetOnInternet() != 0) {
		MessageBox(CResString(IDS_CONF_MSG_UPDATECONNECTFAILED), NULL, MB_OK | MB_ICONERROR);
	} else if (IsAppNeedUpdate()) {
		if (IDYES == MessageBox(CResString(IDS_CONF_MSG_WANTUPDATEORNOT), NULL, MB_YESNO | MB_ICONQUESTION))
			DoAppUpdate();
	} else {
		MessageBox(CResString(IDS_CONF_MSG_NOUPDATE), NULL, MB_OK | MB_ICONINFORMATION);
	}
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
	CMagicKDDlg *pParentDlg = (CMagicKDDlg *)GetParent();
	if (pParentDlg)
		pParentDlg->SetFuncEnable(CMagicKDDlg::eFunc_WallChanger, bEnable);

	g_pTheConf->m_FuncList_bWallChanger = bEnable;
}

void CMagicKDConfDlg::OnBnClickedConfCheckFinddf()
{
	bool bEnable = ((CButton*)GetDlgItem(IDC_CONF_CHECK_FINDDF))->GetCheck()==BST_CHECKED;
	CMagicKDDlg *pParentDlg = (CMagicKDDlg *)GetParent();
	if (pParentDlg)
		pParentDlg->SetFuncEnable(CMagicKDDlg::eFunc_FindDupFile, bEnable);

	g_pTheConf->m_FuncList_bFindDupFile = bEnable;
}

void CMagicKDConfDlg::OnBnClickedConfPiccollector()
{
	bool bEnable = ((CButton*)GetDlgItem(IDC_CONF_CHECK_PICCOLLECTOR))->GetCheck()==BST_CHECKED;
	CMagicKDDlg *pParentDlg = (CMagicKDDlg *)GetParent();
	if (pParentDlg)
		pParentDlg->SetFuncEnable(CMagicKDDlg::eFunc_PicCollector, bEnable);

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
			CMagicKDDlg *pParentDlg = (CMagicKDDlg *)GetParent();
			if (pParentDlg)
				pParentDlg->Localize();
		}
	}
}

LRESULT CMagicKDConfDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_HSCROLL:
		{
		CMagicKDDlg *pParentDlg = (CMagicKDDlg *)GetParent();
		if (pParentDlg) {
			g_pTheConf->m_General_uTransparency = m_sliderTransparency.GetPos();
			pParentDlg->SetTransparency((BYTE)g_pTheConf->m_General_uTransparency);
		}
		}
		break;
	}

	return __super::DefWindowProc(message, wParam, lParam);
}
