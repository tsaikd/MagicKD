// MagicKDDlg.cpp : 實作檔
//

#include "stdafx.h"
#include "Resource.h"
#include "Others.h"
#include "Language.h"
#include "MagicKD.h"

#include "MagicKDDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CKDTray *pTheTray = NULL;

// CMagicKDDlg 對話方塊


IMPLEMENT_DYNAMIC(CMagicKDDlg, CDialog)
CMagicKDDlg::CMagicKDDlg(CWnd* pParent /*=NULL*/)
	:	CDialog(CMagicKDDlg::IDD, pParent), m_pIni(NULL), m_pWallChangerDlg(NULL), m_bVisiable(true), m_bInit(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CMagicKDDlg::~CMagicKDDlg()
{
	DestroyIcon(m_hIcon);
}

BOOL CMagicKDDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 設定此對話方塊的圖示。當應用程式的主視窗不是對話方塊時，
	// 框架會自動從事此作業
	SetIcon(m_hIcon, TRUE);			// 設定大圖示
	SetIcon(m_hIcon, FALSE);		// 設定小圖示

	if (!pTheTray)
		pTheTray = new CKDTray;
	pTheTray->RegisterTray(GetSafeHwnd(), m_hIcon);
	pTheAppEndDlg->SignWnd(GetSafeHwnd(), 2);

	pTheTray->AppendMenu(MF_STRING, IDS_TRAY_RESTART, CResString(IDS_TRAY_RESTART));
	pTheTray->AppendMenu(MF_STRING, IDS_TRAY_OPENWINDOW, CResString(IDS_TRAY_OPENWINDOW));
	pTheTray->AppendMenu(MF_STRING, IDS_TRAY_CLOSEWINDOW, CResString(IDS_TRAY_CLOSEWINDOW), true);

	m_pIni = &theApp.m_cIni;
	m_cMainConfigDlg.Create(IDD_MAGICKD_CONFIG, this);
	m_cMainTab.InsertItem(TCIF_TEXT|TCIF_PARAM, 0, _T("MagicKD"), 0, (LPARAM)&m_cMainConfigDlg);

	pTheTray->InsertMenu(0, MF_BYPOSITION | MF_STRING | MF_UNCHECKED, IDS_TRAY_WALLCHANGER, CResString(IDS_TRAY_WALLCHANGER));
	if (m_pIni->GetBool(_T("FuncList"), _T("bWallChanger"), false))
		SetFuncEnable(eFunc_WallChanger, true, false);
	else
		SetFuncEnable(eFunc_WallChanger, false, false);

	if (m_cMainConfigDlg.IsStartMin())
		m_bVisiable = false;
	ModifyStyleEx(0, WS_EX_LAYERED);
	SetTransparency(m_cMainConfigDlg.GetSliderTransparency());

//////////////////////////////////////////////////
	if (m_pWallChangerDlg)
		m_cMainTab.SetCurSel(eFunc_WallChanger);
//////////////////////////////////////////////////

	m_bInit = true;
	DoSize();

	return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
}

void CMagicKDDlg::OnDestroy()
{
	if (m_cMainConfigDlg.IsShowCloseWindow())
		pTheAppEndDlg->ShowWindow(SW_SHOW);

	pTheAppEndDlg->ProgressStepIt(GetSafeHwnd(), _T("Closing\tMagicKD\tDialog"));
	SetFuncEnable(eFunc_WallChanger, false, false);

	CDialog::OnDestroy();

	pTheAppEndDlg->ProgressStepIt(GetSafeHwnd(), _T("Deleting\tMagicKD\tTray Menu"));
	pTheTray->RemoveTrayMenuItem(CResString(IDS_TRAY_CLOSEWINDOW));
	pTheTray->RemoveTrayMenuItem(CResString(IDS_TRAY_OPENWINDOW));
	pTheTray->RemoveTrayMenuItem(CResString(IDS_TRAY_RESTART));
	delete pTheTray;
	pTheTray = NULL;
}

void CMagicKDDlg::DoSize()
{
	if (!m_bInit)
		return;

	GetClientRect(m_rcMainTab);
	CRect rcWin = m_rcMainTab;
	rcWin.bottom = 25;
	m_cMainTab.MoveWindow(rcWin, FALSE);

	rcWin = m_rcMainTab;
	rcWin.top += 22;
	CDialog *pCurDlg = (CDialog *)m_cMainTab.GetCurItemLParam();
	if (pCurDlg) {
		pCurDlg->MoveWindow(rcWin, FALSE);
		pCurDlg->ShowWindow(SW_SHOW);
	}

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
				m_pWallChangerDlg->Create(IDD_MAGICKD_WALLCHANGER, this);
				m_pWallChangerDlg->ShowWindow(SW_HIDE);
				m_cMainTab.InsertItem(TCIF_TEXT|TCIF_PARAM, eFunc_WallChanger, _T("WallChanger"), 0, (LPARAM)m_pWallChangerDlg);

				int nItem = pTheTray->FindTrayMenuItem(_T("WallChanger"));
				if (nItem >= 0)
					pTheTray->CheckMenuItem(nItem, MF_BYPOSITION | MF_CHECKED);
			} else {
				if (m_pWallChangerDlg){
					m_cMainTab.DeleteItem(eFunc_WallChanger);
					m_pWallChangerDlg->DestroyWindow();
                    delete m_pWallChangerDlg;
					m_pWallChangerDlg = NULL;

					int nItem = pTheTray->FindTrayMenuItem(_T("WallChanger"));
					if (nItem >= 0)
						pTheTray->CheckMenuItem(nItem, MF_BYPOSITION | MF_UNCHECKED);
				}
			}
			break;
	}
	if (bRedraw)
		Invalidate();
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

BOOL CMagicKDDlg::ShowWindow(int nCmdShow)
{
	if (nCmdShow == SW_SHOW)
		m_bVisiable = true;
	else if (nCmdShow == SW_HIDE)
		m_bVisiable = false;

	BOOL bRes = CWnd::ShowWindow(nCmdShow);

	if (nCmdShow == SW_SHOW)
		SetForegroundWindow();
	return bRes;
}

BEGIN_MESSAGE_MAP(CMagicKDDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MAIN, OnTcnSelchangeMaintab)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB_MAIN, OnTcnSelchangingMaintab)
	ON_WM_SIZE()
	ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()

void CMagicKDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_MAIN, m_cMainTab);
}

// 如果將最小化按鈕加入您的對話方塊，您需要下列的程式碼，以便繪製圖示。
// 對於使用文件/檢視模式的 MFC 應用程式，框架會自動完成此作業。

void CMagicKDDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 繪製的裝置內容

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 將圖示置中於用戶端矩形
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 描繪圖示
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//當使用者拖曳最小化視窗時，系統呼叫這個功能取得游標顯示。
HCURSOR CMagicKDDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMagicKDDlg::OnOK()
{
//	CDialog::OnOK();
}

void CMagicKDDlg::OnCancel()
{
	DestroyWindow();

//	CDialog::OnCancel();
}

void CMagicKDDlg::OnSize(UINT nType, int cx, int cy)
{
	if (nType == SIZE_MINIMIZED) {
		ShowWindow(SW_RESTORE);
		ShowWindow(SW_HIDE);
	}

	__super::OnSize(nType, cx, cy);

	DoSize();
}

void CMagicKDDlg::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	if (!m_bVisiable)
		lpwndpos->flags &= ~SWP_SHOWWINDOW;

	__super::OnWindowPosChanging(lpwndpos);
}

void CMagicKDDlg::OnTcnSelchangeMaintab(NMHDR *pNMHDR, LRESULT *pResult)
{
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
	CDialog *pCurDlg = (CDialog *)m_cMainTab.GetCurItemLParam();
	if (pCurDlg)
		pCurDlg->ShowWindow(SW_HIDE);
	*pResult = 0;
}

LRESULT CMagicKDDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WMU_ARE_YOU_APP) {
		ShowWindow(SW_SHOW);
		return WMU_I_AM_APP;
	} else if (message == WMU_TRAY_CALLBACK) {
		switch (lParam) {
		case WM_LBUTTONDOWN:
			if (IsWindowVisible())
				SetForegroundWindow();
			if (m_pWallChangerDlg) {
				m_pWallChangerDlg->OnBnClickedButtonRandpic();
			} else {
				if (IsWindowVisible())
					ShowWindow(SW_HIDE);
				else
					ShowWindow(SW_SHOW);
			}
			break;
		case WM_RBUTTONUP:
			SetForegroundWindow();
			pTheTray->TrackPopupMenu(this);
			Invalidate();
			break;
		}
	} else {
		switch (message) {
		case WM_COMMAND:
			{
				UINT nID = LOWORD(wParam);
				switch (nID) {
				case IDS_TRAY_RESTART:
					theApp.SetRestart();
					DestroyWindow();
					break;
				case IDS_TRAY_OPENWINDOW:
					ShowWindow(SW_SHOW);
					break;
				case IDS_TRAY_CLOSEWINDOW:
					DestroyWindow();
					break;
				case IDS_TRAY_WALLCHANGER:
					if (m_pWallChangerDlg)
						SetFuncEnable(eFunc_WallChanger, false);
					else
						SetFuncEnable(eFunc_WallChanger, true);
					m_cMainConfigDlg.UpdateFuncCheck();
					m_cMainConfigDlg.SetIniModify();
					if (-1 == m_cMainTab.GetCurSel())
						m_cMainTab.SetCurSel(0);
					DoSize();
					break;
				default:
					if (m_pWallChangerDlg && m_pWallChangerDlg->m_hWnd)
						::SendMessage(m_pWallChangerDlg->m_hWnd, message, wParam, lParam);
					break;
				}
			}
			break;
		}
	}

	return __super::DefWindowProc(message, wParam, lParam);
}
