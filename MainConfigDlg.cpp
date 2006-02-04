// MainConfigDlg.cpp : 實作檔
//

#include "stdafx.h"
#include "MagicKD.h"
#include "MagicKDDlg.h"
#include "MainConfigDlg.h"


#define DEFAULT_STARTMIN false
#define DEFAULT_WALLCHANGER false
#define DEFAULT_TRANSPARENCY 255

// CMainConfigDlg 對話方塊

IMPLEMENT_DYNAMIC(CMainConfigDlg, CDialog)
CMainConfigDlg::CMainConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainConfigDlg::IDD, pParent), m_pIni(NULL), m_uTransparency(DEFAULT_TRANSPARENCY)
{
}

CMainConfigDlg::~CMainConfigDlg()
{
}

void CMainConfigDlg::DoSize()
{
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

bool CMainConfigDlg::IsStartMin()
{
	return m_checkStartMin.GetCheck()==BST_CHECKED;
}

bool CMainConfigDlg::IsWallChanger()
{
	return m_cbWallChanger.GetCheck()==BST_CHECKED;
}

UINT CMainConfigDlg::GetSliderTransparency()
{
	return m_uTransparency;
}

void CMainConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CONF_CHECK_WALLCHANGER, m_cbWallChanger);
	DDX_Control(pDX, IDC_CONF_CHECK_STARTMIN, m_checkStartMin);
	DDX_Control(pDX, IDC_CONF_SLIDER_TRANSPARENCY, m_sliderTransparency);
}


BEGIN_MESSAGE_MAP(CMainConfigDlg, CDialog)
	ON_BN_CLICKED(IDC_CONF_CHECK_WALLCHANGER, OnBnClickedWallchangercheck)
	ON_BN_CLICKED(IDC_CONF_CHECK_STARTMIN, OnBnClickedCheckConfStartmin)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CMainConfigDlg 訊息處理常式

BOOL CMainConfigDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pIni = &theApp.m_cIni;

	if (m_pIni->GetBool(_T("FuncList"), _T("bWallChanger"), DEFAULT_WALLCHANGER))
		m_cbWallChanger.SetCheck(BST_CHECKED);
	else
		m_cbWallChanger.SetCheck(BST_UNCHECKED);

	GetDlgItem(IDC_CONF_CHECK_STARTMIN)->SetWindowText(CResString(IDS_CONF_CHECK_STARTMIN));
	if (m_pIni->GetBool(_T("General"), _T("bStartMin"), DEFAULT_STARTMIN))
		m_checkStartMin.SetCheck(BST_CHECKED);
	else
		m_checkStartMin.SetCheck(BST_UNCHECKED);

	m_sliderTransparency.SetWindowText(CResString(IDS_CONF_STATIC_TRANSPARENCY));
	m_sliderTransparency.SetRange(50, 255);
	m_uTransparency = m_pIni->GetUInt(_T("General"), _T("uTransparency"), DEFAULT_TRANSPARENCY);
	m_sliderTransparency.SetPos(m_uTransparency);

	// TODO:  在此加入額外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX 屬性頁應傳回 FALSE
}

void CMainConfigDlg::OnDestroy()
{
	SaveIni();

	__super::OnDestroy();

	// TODO: 在此加入您的訊息處理常式程式碼
}

void CMainConfigDlg::OnBnClickedCheckConfStartmin()
{
	SetIniModify();
	// TODO: 在此加入控制項告知處理常式程式碼
}

void CMainConfigDlg::OnBnClickedWallchangercheck()
{
	CMagicKDDlg *pParentDlg = (CMagicKDDlg *)GetParent();
	if (pParentDlg)
		pParentDlg->SetFuncEnable(CMagicKDDlg::eFunc_WallChanger, IsWallChanger());

	SetIniModify();
	m_cbWallChanger.SetFocus();
	// TODO: 在此加入控制項告知處理常式程式碼
}

LRESULT CMainConfigDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此加入特定的程式碼和 (或) 呼叫基底類別
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
