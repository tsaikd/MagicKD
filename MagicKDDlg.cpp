// MagicKDDlg.cpp : ��@��
//

#include "stdafx.h"
#include "MagicKD.h"
#include "MagicKDDlg.h"


// CMagicKDDlg ��ܤ��

IMPLEMENT_DYNAMIC(CMagicKDDlg, CDialog)
CMagicKDDlg::CMagicKDDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMagicKDDlg::IDD, pParent)
{
}

CMagicKDDlg::~CMagicKDDlg()
{
}

void CMagicKDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CMagicKDDlg::OnInitDialog()
{
	CWallChanger *pWallChanger;
	CDialog::OnInitDialog();
	m_MainTab.InsertItem(TCIF_PARAM | TCIF_TEXT, 1, _T("AA"), 0, (LPARAM) pWallChanger);

	return TRUE;
}

BEGIN_MESSAGE_MAP(CMagicKDDlg, CDialog)
END_MESSAGE_MAP()


// CMagicKDDlg �T���B�z�`��
