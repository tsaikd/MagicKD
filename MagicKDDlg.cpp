// MagicKDDlg.cpp : 實作檔
//

#include "stdafx.h"
#include "MagicKD.h"
#include "MagicKDDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMagicKDDlg 對話方塊


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


// CMagicKDDlg 訊息處理常式

BOOL CMagicKDDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 設定此對話方塊的圖示。當應用程式的主視窗不是對話方塊時，
	// 框架會自動從事此作業
	SetIcon(m_hIcon, TRUE);			// 設定大圖示
	SetIcon(m_hIcon, FALSE);		// 設定小圖示

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
	// TODO: 在此加入額外的初始設定
	
	return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
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
	// TODO: 在此加入特定的程式碼和 (或) 呼叫基底類別

//	CDialog::OnOK();
}

void CMagicKDDlg::OnCancel()
{
	// TODO: 在此加入特定的程式碼和 (或) 呼叫基底類別

	CDialog::OnCancel();
}

void CMagicKDDlg::OnDestroy()
{
	if (IsIniModify())
		SaveIni();

	CDialog::OnDestroy();

	SetFuncEnable(eFunc_WallChanger, false, false);
	// TODO: 在此加入您的訊息處理常式程式碼
}

void CMagicKDDlg::OnTcnSelchangeMaintab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此加入控制項告知處理常式程式碼
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
	// TODO: 在此加入控制項告知處理常式程式碼
	CDialog *pCurDlg = (CDialog *)m_cMainTab.GetCurItemLParam();
	if (pCurDlg)
		pCurDlg->ShowWindow(SW_HIDE);
	*pResult = 0;
}
