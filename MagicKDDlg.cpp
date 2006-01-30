// MagicKDDlg.cpp : ��@��
//

#include "stdafx.h"
#include "MagicKD.h"
#include "MagicKDDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMagicKDDlg ��ܤ��


CMagicKDDlg::CMagicKDDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMagicKDDlg::IDD, pParent), m_pIni(NULL), m_pWallChangerDlg(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMagicKDDlg::InitTabRect()
{
	GetClientRect(m_rcMainTab);
}

void CMagicKDDlg::DoSize()
{
	m_cMainTab.MoveWindow(m_rcMainTab, FALSE);

	CRect rcWin = m_rcMainTab;
	rcWin.top += 20;
	CDialog *pCurDlg = (CDialog *)m_cMainTab.GetCurItemLParam();
	if (pCurDlg)
		pCurDlg->MoveWindow(rcWin, FALSE);

	Invalidate();
}

void CMagicKDDlg::SetFuncEnable(FuncList eFunc, bool bEnable, bool bRedraw/* = true*/)
{
	switch (eFunc) {
		case eFunc_WallChanger:
			if (bEnable) {
				if (m_pWallChangerDlg)
					SetFuncEnable(eFunc_WallChanger, false, false);
				m_pWallChangerDlg = new CWallChangerDlg;
				m_pWallChangerDlg->Create(IDD_WALLCHANGERDIALOG, this);
				m_pWallChangerDlg->ShowWindow(SW_HIDE);
				m_cMainTab.InsertItem(TCIF_TEXT|TCIF_PARAM, eFunc_WallChanger, _T("WallChanger"), 0, (LPARAM)m_pWallChangerDlg);
			} else {
				if (m_pWallChangerDlg){
					m_cMainTab.DeleteItem(eFunc_WallChanger);
					m_pWallChangerDlg->DestroyWindow();
                    delete m_pWallChangerDlg;
					m_pWallChangerDlg = NULL;
				}
			}
			break;
	}
	if (bRedraw)
		Invalidate();
}

void CMagicKDDlg::SaveIni()
{
	m_cMainConfigDlg.SaveIni();

	CKDIni::SaveIni();
}

void CMagicKDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MAINTAB, m_cMainTab);
}

BEGIN_MESSAGE_MAP(CMagicKDDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_NOTIFY(TCN_SELCHANGE, IDC_MAINTAB, OnTcnSelchangeMaintab)
	ON_NOTIFY(TCN_SELCHANGING, IDC_MAINTAB, OnTcnSelchangingMaintab)
END_MESSAGE_MAP()


// CMagicKDDlg �T���B�z�`��

BOOL CMagicKDDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �]�w����ܤ�����ϥܡC�����ε{�����D�������O��ܤ���ɡA
	// �ج[�|�۰ʱq�Ʀ��@�~
	SetIcon(m_hIcon, TRUE);			// �]�w�j�ϥ�
	SetIcon(m_hIcon, FALSE);		// �]�w�p�ϥ�

	InitTabRect();

	m_pIni = &theApp.m_cIni;
	m_cMainConfigDlg.Create(IDD_MAGICCONFIGDIALOG, this);
	m_cMainConfigDlg.ShowWindow(SW_SHOW);
	m_cMainTab.InsertItem(TCIF_TEXT|TCIF_PARAM, 0, _T("MagicKD"), 0, (LPARAM)&m_cMainConfigDlg);

	if (m_pIni->GetBool(_T("FuncList"), _T("bWallChanger"), false))
		SetFuncEnable(eFunc_WallChanger, true, false);

	DoSize();
	if (m_pWallChangerDlg) {
		CDialog *pCurDlg = (CDialog*)m_cMainTab.GetCurItemLParam();
		if (pCurDlg)
			pCurDlg->ShowWindow(SW_HIDE);
		m_cMainTab.SetCurSel(eFunc_WallChanger);
		pCurDlg = (CDialog*)m_cMainTab.GetCurItemLParam();
		if (pCurDlg) {
			pCurDlg->ShowWindow(SW_SHOW);
			pCurDlg->SetFocus();

			DoSize();
		}
	}
	// TODO: �b���[�J�B�~����l�]�w
	
	return TRUE;  // �Ǧ^ TRUE�A���D�z�ﱱ��]�w�J�I
}

// �p�G�N�̤p�ƫ��s�[�J�z����ܤ���A�z�ݭn�U�C���{���X�A�H�Kø�s�ϥܡC
// ���ϥΤ��/�˵��Ҧ��� MFC ���ε{���A�ج[�|�۰ʧ������@�~�C

void CMagicKDDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ø�s���˸m���e

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// �N�ϥܸm����Τ�ݯx��
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �yø�ϥ�
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//��ϥΪ̩즲�̤p�Ƶ����ɡA�t�ΩI�s�o�ӥ\����o�����ܡC
HCURSOR CMagicKDDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMagicKDDlg::OnOK()
{
	// TODO: �b���[�J�S�w���{���X�M (��) �I�s�����O

//	CDialog::OnOK();
}

void CMagicKDDlg::OnCancel()
{
	// TODO: �b���[�J�S�w���{���X�M (��) �I�s�����O

	CDialog::OnCancel();
}

void CMagicKDDlg::OnDestroy()
{
	if (IsIniModify())
		SaveIni();

	CDialog::OnDestroy();

	SetFuncEnable(eFunc_WallChanger, false, false);
	// TODO: �b���[�J�z���T���B�z�`���{���X
}

void CMagicKDDlg::OnTcnSelchangeMaintab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �b���[�J����i���B�z�`���{���X
	CDialog *pCurDlg = (CDialog*)m_cMainTab.GetCurItemLParam();
	if (pCurDlg) {
		pCurDlg->ShowWindow(SW_SHOW);
		pCurDlg->SetFocus();

		DoSize();
	}

	*pResult = 0;
}

void CMagicKDDlg::OnTcnSelchangingMaintab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �b���[�J����i���B�z�`���{���X
	CDialog *pCurDlg = (CDialog *)m_cMainTab.GetCurItemLParam();
	if (pCurDlg)
		pCurDlg->ShowWindow(SW_HIDE);
	*pResult = 0;
}
