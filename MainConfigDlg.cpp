#include "stdafx.h"
#include "Resource.h"
#include "Language.h"
#include "MagicKD.h"
#include "MagicKDDlg.h"

#include "MainConfigDlg.h"

IMPLEMENT_DYNAMIC(CMainConfigDlg, CDialog)
CMainConfigDlg::CMainConfigDlg(CWnd* pParent /*=NULL*/)
	:	CDialog(CMainConfigDlg::IDD, pParent), m_bInit(false)
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

BOOL CMainConfigDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	GetDlgItem(IDC_CONF_BTN_RESTART)->SetWindowText(CResString(IDS_CONF_BTN_RESTART));

	GetDlgItem(IDC_CONF_CHECK_STARTMIN)->SetWindowText(CResString(IDS_CONF_CHECK_STARTMIN));
	if (theConf.m_General_bStartMin)
		m_checkStartMin.SetCheck(BST_CHECKED);
	else
		m_checkStartMin.SetCheck(BST_UNCHECKED);

	GetDlgItem(IDC_CONF_CHECK_SHOWCLOSEWINDOW)->SetWindowText(CResString(IDS_CONF_CHECK_SHOWCLOSEWINDOW));
	if (theConf.m_General_bShowCloseWindow)
		m_checkShowCloseWindow.SetCheck(BST_CHECKED);
	else
		m_checkShowCloseWindow.SetCheck(BST_UNCHECKED);

	if (theConf.m_FuncList_bWallChanger)
		m_checkWallChanger.SetCheck(BST_CHECKED);
	else
		m_checkWallChanger.SetCheck(BST_UNCHECKED);

	GetDlgItem(IDC_CONF_STATIC_TRANSPARENCY)->SetWindowText(CResString(IDS_CONF_STATIC_TRANSPARENCY));
	m_sliderTransparency.SetRange(50, 255);
	m_sliderTransparency.SetPos(theConf.m_General_uTransparency);

	m_bInit = true;
	DoSize();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX 屬性頁應傳回 FALSE
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
	theConf.m_General_bStartMin = m_checkStartMin.GetCheck()==BST_CHECKED;
}

void CMainConfigDlg::OnBnClickedConfCheckShowclosewindow()
{
	theConf.m_General_bShowCloseWindow = m_checkShowCloseWindow.GetCheck()==BST_CHECKED;
}

void CMainConfigDlg::OnBnClickedWallchangercheck()
{
	bool bWallChanger = m_checkWallChanger.GetCheck()==BST_CHECKED;
	CMagicKDDlg *pParentDlg = (CMagicKDDlg *)GetParent();
	if (pParentDlg)
		pParentDlg->SetFuncEnable(CMagicKDDlg::eFunc_WallChanger, bWallChanger);

	theConf.m_FuncList_bWallChanger = bWallChanger;
}

LRESULT CMainConfigDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_HSCROLL:
		{
			CMagicKDDlg *pParentDlg = (CMagicKDDlg *)GetParent();
			if (pParentDlg) {
				theConf.m_General_uTransparency = m_sliderTransparency.GetPos();
				pParentDlg->SetTransparency(theConf.m_General_uTransparency);
			}
		}
		break;
	}

	return __super::DefWindowProc(message, wParam, lParam);
}
