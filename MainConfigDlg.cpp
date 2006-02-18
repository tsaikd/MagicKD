// MainConfigDlg.cpp : 實作檔
//

#include "stdafx.h"
#include "Resource.h"
#include "Language.h"
#include "MagicKD.h"
#include "MagicKDDlg.h"

#include "MainConfigDlg.h"

#define DEFAULT_STARTMIN			false
#define DEFAULT_SHOWCLOSEWINDOW		true
#define DEFAULT_WALLCHANGER			false
#define DEFAULT_TRANSPARENCY		255

// CMainConfigDlg 對話方塊

IMPLEMENT_DYNAMIC(CMainConfigDlg, CDialog)
CMainConfigDlg::CMainConfigDlg(CWnd* pParent /*=NULL*/)
	:	CDialog(CMainConfigDlg::IDD, pParent), m_pIni(NULL), m_uTransparency(DEFAULT_TRANSPARENCY), m_bInit(false)
{
}

CMainConfigDlg::~CMainConfigDlg()
{
}

void CMainConfigDlg::DoSize()
{
	if (!m_bInit)
		return;

	Invalidate();
}

void CMainConfigDlg::SaveIni()
{
	if (!IsIniModify())
		return;

	bool bStartMin = IsStartMin();
	if (bStartMin != DEFAULT_STARTMIN)
		m_pIni->WriteBool(_T("General"), _T("bStartMin"), bStartMin);
	else
		m_pIni->DeleteKey(_T("General"), _T("bStartMin"));

	bool bShowCloseWindow = IsShowCloseWindow();
	if (bShowCloseWindow != DEFAULT_SHOWCLOSEWINDOW)
		m_pIni->WriteBool(_T("General"), _T("bShowCloseWindow"), bShowCloseWindow);
	else
		m_pIni->DeleteKey(_T("General"), _T("bShowCloseWindow"));

	bool bWallChanger = IsWallChanger();
	if (bWallChanger != DEFAULT_WALLCHANGER)
		m_pIni->WriteBool(_T("FuncList"), _T("bWallChanger"), bWallChanger);
	else
		m_pIni->DeleteKey(_T("FuncList"), _T("bWallChanger"));

	if (m_uTransparency != DEFAULT_TRANSPARENCY)
		m_pIni->WriteUInt(_T("General"), _T("uTransparency"), m_uTransparency);
	else
		m_pIni->DeleteKey(_T("General"), _T("uTransparency"));

	CKDIni::SaveIni();
}

BOOL CMainConfigDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pIni = &theApp.m_cIni;

	GetDlgItem(IDC_CONF_BTN_RESTART)->SetWindowText(CResString(IDS_CONF_BTN_RESTART));

	GetDlgItem(IDC_CONF_CHECK_STARTMIN)->SetWindowText(CResString(IDS_CONF_CHECK_STARTMIN));
	if (m_pIni->GetBool(_T("General"), _T("bStartMin"), DEFAULT_STARTMIN))
		m_checkStartMin.SetCheck(BST_CHECKED);
	else
		m_checkStartMin.SetCheck(BST_UNCHECKED);

	GetDlgItem(IDC_CONF_CHECK_SHOWCLOSEWINDOW)->SetWindowText(CResString(IDS_CONF_CHECK_SHOWCLOSEWINDOW));
	if (m_pIni->GetBool(_T("General"), _T("bShowCloseWindow"), DEFAULT_SHOWCLOSEWINDOW))
		m_checkShowCloseWindow.SetCheck(BST_CHECKED);
	else
		m_checkShowCloseWindow.SetCheck(BST_UNCHECKED);

	if (m_pIni->GetBool(_T("FuncList"), _T("bWallChanger"), DEFAULT_WALLCHANGER))
		m_checkWallChanger.SetCheck(BST_CHECKED);
	else
		m_checkWallChanger.SetCheck(BST_UNCHECKED);

	GetDlgItem(IDC_CONF_STATIC_TRANSPARENCY)->SetWindowText(CResString(IDS_CONF_STATIC_TRANSPARENCY));
	m_sliderTransparency.SetRange(50, 255);
	m_uTransparency = m_pIni->GetUInt(_T("General"), _T("uTransparency"), DEFAULT_TRANSPARENCY);
	m_sliderTransparency.SetPos(m_uTransparency);

	m_bInit = true;
	DoSize();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX 屬性頁應傳回 FALSE
}

void CMainConfigDlg::OnDestroy()
{
	SaveIni();

	__super::OnDestroy();
}

bool CMainConfigDlg::IsStartMin()
{
	return m_checkStartMin.GetCheck()==BST_CHECKED;
}

bool CMainConfigDlg::IsShowCloseWindow()
{
	return m_checkShowCloseWindow.GetCheck()==BST_CHECKED;
}

bool CMainConfigDlg::IsWallChanger()
{
	return m_checkWallChanger.GetCheck()==BST_CHECKED;
}

UINT CMainConfigDlg::GetSliderTransparency()
{
	return m_uTransparency;
}

void CMainConfigDlg::UpdateFuncCheck()
{
	CMagicKDDlg *pParentDlg = (CMagicKDDlg *)GetParent();
	if (pParentDlg)
		m_checkWallChanger.SetCheck(pParentDlg->m_pWallChangerDlg ? BST_CHECKED : BST_UNCHECKED);
}

BEGIN_MESSAGE_MAP(CMainConfigDlg, CDialog)
	ON_BN_CLICKED(IDC_CONF_CHECK_STARTMIN		, OnBnClickedCheckConfStartmin)
	ON_BN_CLICKED(IDC_CONF_CHECK_SHOWCLOSEWINDOW, OnBnClickedConfCheckShowclosewindow)
	ON_BN_CLICKED(IDC_CONF_CHECK_WALLCHANGER	, OnBnClickedWallchangercheck)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CONF_BTN_RESTART, OnBnClickedConfBtnRestart)
	ON_WM_SIZE()
END_MESSAGE_MAP()

void CMainConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CONF_CHECK_STARTMIN		, m_checkStartMin);
	DDX_Control(pDX, IDC_CONF_CHECK_SHOWCLOSEWINDOW	, m_checkShowCloseWindow);
	DDX_Control(pDX, IDC_CONF_CHECK_WALLCHANGER		, m_checkWallChanger);
	DDX_Control(pDX, IDC_CONF_SLIDER_TRANSPARENCY	, m_sliderTransparency);
}

void CMainConfigDlg::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	DoSize();
}

void CMainConfigDlg::OnBnClickedConfBtnRestart()
{
	::SendMessage(GetParent()->GetSafeHwnd(), WM_COMMAND, IDS_TRAY_RESTART, 0);
}

void CMainConfigDlg::OnBnClickedCheckConfStartmin()
{
	SetIniModify();
}

void CMainConfigDlg::OnBnClickedConfCheckShowclosewindow()
{
	SetIniModify();
}

void CMainConfigDlg::OnBnClickedWallchangercheck()
{
	CMagicKDDlg *pParentDlg = (CMagicKDDlg *)GetParent();
	if (pParentDlg)
		pParentDlg->SetFuncEnable(CMagicKDDlg::eFunc_WallChanger, IsWallChanger());

	SetIniModify();
}

LRESULT CMainConfigDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_HSCROLL) {
		CMagicKDDlg *pParentDlg = (CMagicKDDlg *)GetParent();
		if (pParentDlg) {
			m_uTransparency = m_sliderTransparency.GetPos();
			pParentDlg->SetTransparency(m_uTransparency);
			SetIniModify();
		}
	}

	return __super::DefWindowProc(message, wParam, lParam);
}
