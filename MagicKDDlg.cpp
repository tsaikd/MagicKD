// MagicKDDlg.cpp : ��@��
//

#include "stdafx.h"
#include "Others.h"
#include "MagicKD.h"
#include "MagicKDDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CKDTray theTray;

// CMagicKDDlg ��ܤ��


CMagicKDDlg::CMagicKDDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMagicKDDlg::IDD, pParent), m_pIni(NULL), m_pWallChangerDlg(NULL), m_bVisiable(true)
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

bool CMagicKDDlg::SetTransparency(UINT uAlpha)
{
	bool bRes = true;
	ASSERT(INRANGE(uAlpha, 50, 255));

	if (!(GetExStyle() && WS_EX_LAYERED))
		bRes = bRes && ModifyStyleEx(0, WS_EX_LAYERED);
	bRes = bRes && SetLayeredWindowAttributes(0, uAlpha, LWA_ALPHA);
	return bRes;
}

void CMagicKDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_MAIN, m_cMainTab);
}

BEGIN_MESSAGE_MAP(CMagicKDDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MAIN, OnTcnSelchangeMaintab)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB_MAIN, OnTcnSelchangingMaintab)
	ON_WM_SIZE()
	ON_WM_WINDOWPOSCHANGING()
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
	theTray.RegisterTray(m_hWnd, m_hIcon);

	theTray.AppendMenu(MF_STRING, IDS_TRAY_OPENWINDOW, GetResString(IDS_TRAY_OPENWINDOW));
	theTray.AppendMenu(MF_STRING, IDS_TRAY_CLOSEWINDOW, GetResString(IDS_TRAY_CLOSEWINDOW), true);

	m_pIni = &theApp.m_cIni;
	m_cMainConfigDlg.Create(IDD_MAGICCONFIGDIALOG, this);
	m_cMainTab.InsertItem(TCIF_TEXT|TCIF_PARAM, 0, _T("MagicKD"), 0, (LPARAM)&m_cMainConfigDlg);

	if (m_pIni->GetBool(_T("FuncList"), _T("bWallChanger"), false))
		SetFuncEnable(eFunc_WallChanger, true, false);

	if (m_cMainConfigDlg.IsStartMin())
		m_bVisiable = false;

	ModifyStyleEx(0, WS_EX_LAYERED);
	SetTransparency(m_cMainConfigDlg.GetSliderTransparency());

//////////////////////////////////////////////////
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

	DoSize();
	// TODO: �b���[�J�B�~����l�]�w
	
	return TRUE;  // �Ǧ^ TRUE�A���D�z�ﱱ��]�w�J�I
}

void CMagicKDDlg::OnDestroy()
{
	if (IsIniModify())
		SaveIni();

	CDialog::OnDestroy();

	SetFuncEnable(eFunc_WallChanger, false, false);
	theTray.RemoveTrayMenuItem(GetResString(IDS_TRAY_CLOSEWINDOW));
	theTray.UnRegisterTray();
	// TODO: �b���[�J�z���T���B�z�`���{���X
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

void CMagicKDDlg::OnSize(UINT nType, int cx, int cy)
{
	if (nType == SIZE_MINIMIZED) {
		ShowWindow(SW_RESTORE);
		ShowWindow(SW_HIDE);
		return;
	}
	__super::OnSize(nType, cx, cy);

	// TODO: �b���[�J�z���T���B�z�`���{���X
}

void CMagicKDDlg::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	if (!m_bVisiable)
		lpwndpos->flags &= ~SWP_SHOWWINDOW;

	__super::OnWindowPosChanging(lpwndpos);

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

LRESULT CMagicKDDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WMU_ARE_YOU_APP) {
		ShowWindow(SW_SHOW);
		SetForegroundWindow();
		return WMU_I_AM_APP;
	} else if (message == WMU_TRAY_CALLBACK) {
		switch (lParam) {
		case WM_LBUTTONDOWN:
			if (IsWindowVisible())
				SetForegroundWindow();
			if (m_pWallChangerDlg)
				m_pWallChangerDlg->OnBnClickedButtonRandpic();
			else
				SendMessage(message, wParam, WM_LBUTTONDBLCLK);
			return 0;
		case WM_LBUTTONDBLCLK:
			if (IsWindowVisible()) {
				m_bVisiable = false;
				ShowWindow(SW_HIDE);
			} else {
				m_bVisiable = true;
				ShowWindow(SW_SHOW);
				SetForegroundWindow();
			}
			return 0;
		case WM_RBUTTONUP:
			SetForegroundWindow();
			theTray.TrackPopupMenu(this);
			Invalidate();
			return 0;
		}
		return 0;
	}
	switch (message) {
	case WM_COMMAND:
		{
			UINT nID = LOWORD(wParam);
			switch (nID) {
			case IDS_TRAY_OPENWINDOW:
				ShowWindow(SW_SHOW);
				SetForegroundWindow();
				return 0;
			case IDS_TRAY_CLOSEWINDOW:
				DestroyWindow();
				return 0;
			}
			if (m_pWallChangerDlg && m_pWallChangerDlg->m_hWnd)
				::SendMessage(m_pWallChangerDlg->m_hWnd, message, wParam, lParam);
		}
		break;
	}

	return __super::DefWindowProc(message, wParam, lParam);
}
