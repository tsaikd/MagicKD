// MagicKDConfigDlg.cpp : ��@��
//

#include "stdafx.h"
#include "MagicKD.h"
#include "MagicKDDlg.h"
#include "MagicKDConfigDlg.h"


// CMagicKDConfigDlg ��ܤ��

IMPLEMENT_DYNAMIC(CMagicKDConfigDlg, CDialog)
CMagicKDConfigDlg::CMagicKDConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMagicKDConfigDlg::IDD, pParent)
{
}

CMagicKDConfigDlg::~CMagicKDConfigDlg()
{
}

void CMagicKDConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_WALLCHANGER, m_cbWallChanger);
}


BEGIN_MESSAGE_MAP(CMagicKDConfigDlg, CDialog)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_CHECK_WALLCHANGER, OnBnClickedCheckWallchanger)
END_MESSAGE_MAP()


// CMagicKDConfigDlg �T���B�z�`��

void CMagicKDConfigDlg::OnOK()
{
	// TODO: �b���[�J�S�w���{���X�M (��) �I�s�����O

//	CDialog::OnOK();
}

void CMagicKDConfigDlg::OnCancel()
{
	// TODO: �b���[�J�S�w���{���X�M (��) �I�s�����O

//	CDialog::OnCancel();
}

void CMagicKDConfigDlg::OnBnClickedCheckWallchanger()
{
	// TODO: �b���[�J����i���B�z�`���{���X
	if (m_cbWallChanger.GetCheck() == BST_CHECKED)
		((CMagicKDDlg*)GetParent())->SetEnableFunc(CMagicKDDlg::eFunc_WallChanger, true);
	else
		((CMagicKDDlg*)GetParent())->SetEnableFunc(CMagicKDDlg::eFunc_WallChanger, false);
}
