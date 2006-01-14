// MagicKDConfigDlg.cpp : 實作檔
//

#include "stdafx.h"
#include "MagicKD.h"
#include "MagicKDDlg.h"
#include "MagicKDConfigDlg.h"


// CMagicKDConfigDlg 對話方塊

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


// CMagicKDConfigDlg 訊息處理常式

void CMagicKDConfigDlg::OnOK()
{
	// TODO: 在此加入特定的程式碼和 (或) 呼叫基底類別

//	CDialog::OnOK();
}

void CMagicKDConfigDlg::OnCancel()
{
	// TODO: 在此加入特定的程式碼和 (或) 呼叫基底類別

//	CDialog::OnCancel();
}

void CMagicKDConfigDlg::OnBnClickedCheckWallchanger()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	if (m_cbWallChanger.GetCheck() == BST_CHECKED)
		((CMagicKDDlg*)GetParent())->SetEnableFunc(CMagicKDDlg::eFunc_WallChanger, true);
	else
		((CMagicKDDlg*)GetParent())->SetEnableFunc(CMagicKDDlg::eFunc_WallChanger, false);
}
