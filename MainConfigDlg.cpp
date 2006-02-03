// MainConfigDlg.cpp : 實作檔
//

#include "stdafx.h"
#include "MagicKD.h"
#include "MagicKDDlg.h"
#include "MainConfigDlg.h"


#define DEFAULT_WALLCHANGER false

// CMainConfigDlg 對話方塊

IMPLEMENT_DYNAMIC(CMainConfigDlg, CDialog)
CMainConfigDlg::CMainConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainConfigDlg::IDD, pParent), m_pIni(NULL), m_bModify(false)
{
}

CMainConfigDlg::~CMainConfigDlg()
{
}

void CMainConfigDlg::DoSize()
{
}

bool CMainConfigDlg::IsIniModify()
{
	return m_bModify;
}

void CMainConfigDlg::SetIniModify(bool bModify/* = true*/)
{
	m_bModify = bModify;
}

void CMainConfigDlg::SaveIni()
{
	bool bWallChanger = m_cbWallChanger.GetCheck()==BST_CHECKED;
	if (bWallChanger != DEFAULT_WALLCHANGER)
		m_pIni->WriteBool(_T("FuncList"), _T("bWallChanger"), bWallChanger);
	else
		m_pIni->DeleteKey(_T("FuncList"), _T("bWallChanger"));

	SetIniModify(false);
}

void CMainConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WALLCHANGERCHECK, m_cbWallChanger);
}


BEGIN_MESSAGE_MAP(CMainConfigDlg, CDialog)
	ON_BN_CLICKED(IDC_WALLCHANGERCHECK, OnBnClickedWallchangercheck)
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
	// TODO:  在此加入額外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX 屬性頁應傳回 FALSE
}

void CMainConfigDlg::OnBnClickedWallchangercheck()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	((CMagicKDDlg *)GetParent())->SetFuncEnable(CMagicKDDlg::eFunc_WallChanger, m_cbWallChanger.GetCheck()==BST_CHECKED);
	((CMagicKDDlg *)GetParent())->SetIniModify();
	m_cbWallChanger.SetFocus();
}
