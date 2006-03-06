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
	:	CDialog(CMagicKDConfDlg::IDD, pParent), m_bInit(false), m_uUpdateTimer(0)
{
}

CMagicKDConfDlg::~CMagicKDConfDlg()
{
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

BOOL CMagicKDConfDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	GetDlgItem(IDC_CONF_STATIC_VERSION)->SetWindowText(theApp.GetAppProductVer());

	GetDlgItem(IDC_CONF_BTN_RESTART)->SetWindowText(CResString(IDS_CONF_BTN_RESTART));

	GetDlgItem(IDC_CONF_CHECK_STARTMIN)->SetWindowText(CResString(IDS_CONF_CHECK_STARTMIN));
	if (::pTheConf->m_General_bStartMin)
		m_checkStartMin.SetCheck(BST_CHECKED);
	else
		m_checkStartMin.SetCheck(BST_UNCHECKED);

	GetDlgItem(IDC_CONF_CHECK_SHOWCLOSEWINDOW)->SetWindowText(CResString(IDS_CONF_CHECK_SHOWCLOSEWINDOW));
	if (::pTheConf->m_General_bShowCloseWindow)
		m_checkShowCloseWindow.SetCheck(BST_CHECKED);
	else
		m_checkShowCloseWindow.SetCheck(BST_UNCHECKED);

	GetDlgItem(IDC_CONF_CHECK_CHECKUPDATE)->SetWindowText(CResString(IDS_CONF_CHECK_CHECKUPDATE));
	if (::pTheConf->m_General_bCheckUpdate) {
		m_check_CheckUpdate.SetCheck(BST_CHECKED);
		StartUpdateTimer();
	} else {
		m_check_CheckUpdate.SetCheck(BST_UNCHECKED);
		StopUpdateTimer();
	}

	GetDlgItem(IDC_CONF_STATIC_TRANSPARENCY)->SetWindowText(CResString(IDS_CONF_STATIC_TRANSPARENCY));
	m_sliderTransparency.SetRange(50, 255);
	m_sliderTransparency.SetPos(::pTheConf->m_General_uTransparency);

	m_bInit = true;
	DoSize();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX 屬性頁應傳回 FALSE
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
	}
}

bool CMagicKDConfDlg::IsAppNeedUpdate()
{
	m_sLastVer = theApp.GetUpdateAppOnLineVer(_T("http://svn.tsaikd.org/tsaikd/MagicKD/ReleaseHistory/"), _T("</ul>"), -19, 7);
	CString sNowVer(theApp.GetAppProductVer());
	sNowVer.Replace(_T(","), _T("."));
	if ((m_sLastVer == sNowVer) || (m_sLastVer.IsEmpty())) {
		return false;
	} else {
		m_sLastUrl.Format(_T("http://svn.tsaikd.org/tsaikd/MagicKD/ReleaseHistory/%s/MagicKD.exe"), m_sLastVer);
		return true;
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
	ON_BN_CLICKED(IDC_CONF_CHECK_SHOWCLOSEWINDOW, OnBnClickedConfCheckShowclosewindow)
	ON_BN_CLICKED(IDC_CONF_CHECK_WALLCHANGER	, OnBnClickedWallchangercheck)
	ON_BN_CLICKED(IDC_CONF_BTN_RESTART			, OnBnClickedConfBtnRestart)
	ON_BN_CLICKED(IDC_CONF_CHECK_FINDDF			, OnBnClickedConfCheckFinddf)
	ON_BN_CLICKED(IDC_CONF_BTN_CHECKUPDATE		, OnBnClickedConfBtnCheckupdate)
	ON_BN_CLICKED(IDC_CONF_CHECK_CHECKUPDATE, OnBnClickedConfCheckCheckupdate)
END_MESSAGE_MAP()

void CMagicKDConfDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CONF_CHECK_STARTMIN		, m_checkStartMin);
	DDX_Control(pDX, IDC_CONF_CHECK_SHOWCLOSEWINDOW	, m_checkShowCloseWindow);
	DDX_Control(pDX, IDC_CONF_SLIDER_TRANSPARENCY	, m_sliderTransparency);
	DDX_Control(pDX, IDC_CONF_PROGRESS_UPDATE, m_progress_Update);
	DDX_Control(pDX, IDC_CONF_CHECK_CHECKUPDATE, m_check_CheckUpdate);
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
	if (IsAppNeedUpdate()) {
		CString sMsg;
		sMsg.Format(CResString(IDS_CONF_MSG_WANTUPDATEORNOT), m_sLastVer);
		if (IDYES == MessageBox(sMsg, NULL, MB_YESNO | MB_ICONQUESTION)) {
			double dPercent = 0;
			CString sNewAppPath;
			sNewAppPath.Format(_T("%sMagicKD_Update.exe"), theApp.GetAppDir());
			m_progress_Update.SetPos(0);
			m_progress_Update.SetRange32(0, 10000);
			if (m_GetHttpFile.AddFileList(m_sLastUrl, sNewAppPath)) {
				while (!m_GetHttpFile.IsDownloadAllOver()) {
					dPercent = m_GetHttpFile.GetPercentOfTotalDL();
					m_progress_Update.SetPos((int)(dPercent * 10000));
					Sleep(350);
				}
				m_progress_Update.SetPos(10000);
				theApp.SetUpdateApp(theApp.GetAppPath(), sNewAppPath);
				theApp.Quit();
			} else {
				MessageBox(CResString(IDS_CONF_MSG_UPDATEFAILED), NULL, MB_OK | MB_ICONERROR);
			}
		}
	} else if (m_sLastVer.IsEmpty()) {
		MessageBox(CResString(IDS_CONF_MSG_UPDATECONNECTFAILED), NULL, MB_OK | MB_ICONERROR);
	} else {
		MessageBox(CResString(IDS_CONF_MSG_NOUPDATE), NULL, MB_OK | MB_ICONINFORMATION);
	}
}

void CMagicKDConfDlg::OnBnClickedCheckConfStartmin()
{
	::pTheConf->m_General_bStartMin = m_checkStartMin.GetCheck()==BST_CHECKED;
}

void CMagicKDConfDlg::OnBnClickedConfCheckShowclosewindow()
{
	::pTheConf->m_General_bShowCloseWindow = m_checkShowCloseWindow.GetCheck()==BST_CHECKED;
}

void CMagicKDConfDlg::OnBnClickedConfCheckCheckupdate()
{
	::pTheConf->m_General_bCheckUpdate = m_check_CheckUpdate.GetCheck()==BST_CHECKED;
	if (::pTheConf->m_General_bCheckUpdate)
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

	::pTheConf->m_FuncList_bWallChanger = bEnable;
}

void CMagicKDConfDlg::OnBnClickedConfCheckFinddf()
{
	bool bEnable = ((CButton*)GetDlgItem(IDC_CONF_CHECK_FINDDF))->GetCheck()==BST_CHECKED;
	CMagicKDDlg *pParentDlg = (CMagicKDDlg *)GetParent();
	if (pParentDlg)
		pParentDlg->SetFuncEnable(CMagicKDDlg::eFunc_FindDupFile, bEnable);

	::pTheConf->m_FuncList_bFindDupFile = bEnable;
}

LRESULT CMagicKDConfDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_HSCROLL:
		{
		CMagicKDDlg *pParentDlg = (CMagicKDDlg *)GetParent();
		if (pParentDlg) {
			::pTheConf->m_General_uTransparency = m_sliderTransparency.GetPos();
			pParentDlg->SetTransparency((BYTE)::pTheConf->m_General_uTransparency);
		}
		}
		break;
	}

	return __super::DefWindowProc(message, wParam, lParam);
}
