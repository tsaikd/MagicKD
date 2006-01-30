// MainConfigDlg.cpp : ��@��
//

#include "stdafx.h"
#include "MagicKD.h"
#include "MagicKDDlg.h"
#include "MainConfigDlg.h"


// CMainConfigDlg ��ܤ��

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
	m_pIni->WriteBool(_T("FuncList"), _T("bWallChanger"), m_cbWallChanger.GetCheck()==BST_CHECKED);

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


// CMainConfigDlg �T���B�z�`��

BOOL CMainConfigDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pIni = &theApp.m_cIni;
	if (m_pIni->GetBool(_T("FuncList"), _T("bWallChanger"), false))
		m_cbWallChanger.SetCheck(BST_CHECKED);
	else
		m_cbWallChanger.SetCheck(BST_UNCHECKED);
	// TODO:  �b���[�J�B�~����l��

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX �ݩʭ����Ǧ^ FALSE
}

void CMainConfigDlg::OnBnClickedWallchangercheck()
{
	// TODO: �b���[�J����i���B�z�`���{���X
	((CMagicKDDlg *)GetParent())->SetFuncEnable(CMagicKDDlg::eFunc_WallChanger, m_cbWallChanger.GetCheck()==BST_CHECKED);
	((CMagicKDDlg *)GetParent())->SetIniModify();
	m_cbWallChanger.SetFocus();
}
