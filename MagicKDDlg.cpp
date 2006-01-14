// MagicKDDlg.cpp : 實作檔
//

#include "StdAfx.h"
#include "MagicKD.h"
#include "MagicKDDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 對 App About 使用 CAboutDlg 對話方塊

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 對話方塊資料
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

// 程式碼實作
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CMagicKDDlg 對話方塊



CMagicKDDlg::CMagicKDDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMagicKDDlg::IDD, pParent), m_bInit(false), m_pWallChanger(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CMagicKDDlg::~CMagicKDDlg()
{
	if (m_pWallChanger)
		delete m_pWallChanger;
}

void CMagicKDDlg::InitWindowRect()
{
	CRect rcWin;
	GetClientRect(rcWin);
	m_MainTab.MoveWindow(rcWin);
	m_MainTab.GetClientRect(rcWin);
	rcWin.top += 35;
	rcWin.right -= 10;
	rcWin.bottom -= 10;
	m_rcTabWindow = rcWin;
}

void CMagicKDDlg::MainConfigSyncTabEnable()
{
	if (m_pWallChanger)
		m_MainConfigDlg.m_cbWallChanger.SetCheck(m_pWallChanger->m_EnableWallChanger.GetCheck());
}

void CMagicKDDlg::SetEnableFunc(FuncList eFunc, bool bEnable)
{
	switch (eFunc) {
	case eFunc_WallChanger:
		if (bEnable) {
			if (m_pWallChanger) {
				m_pWallChanger->SetEnableWallChanger(bEnable);
			} else {
				m_pWallChanger = new CWallChanger;
				m_pWallChanger->Create(IDD_WALLCHANGER, this);
			}
			m_MainTab.InsertItem(TCIF_TEXT|TCIF_PARAM, eFunc_WallChanger, _T("WallChanger"), 0, (LPARAM)m_pWallChanger);
		} else {
			m_pWallChanger->ShowWindow(SW_HIDE);
			m_MainTab.DeleteItem(eFunc_WallChanger);
			m_MainTab.SetCurSel(0);
			m_pWallChanger->DestroyWindow();
			delete m_pWallChanger;
			m_pWallChanger = NULL;
			m_MainConfigDlg.ShowWindow(SW_HIDE);
			m_MainConfigDlg.ShowWindow(SW_SHOW);
		}
		m_cIni.WriteBool(_T("FuncList"), _T("bWallChanger"), bEnable);
		break;
	}
}

void CMagicKDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_MAINTAB, m_MainTab);
}

BEGIN_MESSAGE_MAP(CMagicKDDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_WM_MOVE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_MAINTAB, OnTcnSelchangeMaintab)
	ON_NOTIFY(TCN_SELCHANGING, IDC_MAINTAB, OnTcnSelchangingMaintab)
END_MESSAGE_MAP()


// CMagicKDDlg 訊息處理常式

BOOL CMagicKDDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 將 "關於..." 功能表加入系統功能表。

	// IDM_ABOUTBOX 必須在系統命令範圍之中。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 設定此對話方塊的圖示。當應用程式的主視窗不是對話方塊時，
	// 框架會自動從事此作業
	SetIcon(m_hIcon, TRUE);			// 設定大圖示
	SetIcon(m_hIcon, FALSE);		// 設定小圖示

	// TODO: 在此加入額外的初始設定
	InitWindowRect();
	m_cIni.SetPathName(_T(".\\MagicKD.ini"));

	m_MainConfigDlg.Create(IDD_MAIN_CONFIG_DIALOG, this);
	m_MainConfigDlg.MoveWindow(m_rcTabWindow);
	m_MainTab.InsertItem(TCIF_TEXT|TCIF_PARAM, 0, _T("MagicKD"), 0, (LPARAM)&m_MainConfigDlg);

	if (m_cIni.GetBool(_T("FuncList"), _T("bWallChanger"), false)) {
		m_pWallChanger = new CWallChanger;
		m_pWallChanger->Create(IDD_WALLCHANGER, this);
		m_MainTab.InsertItem(TCIF_TEXT|TCIF_PARAM, eFunc_WallChanger, _T("WallChanger"), 0, (LPARAM)m_pWallChanger);
	}

//	m_pWallChanger = new CWallChanger;
//	m_pWallChanger->Create(IDD_WALLCHANGER, this);
//	m_MainTab.InsertItem(TCIF_TEXT|TCIF_PARAM, 2, _T("WallChanger2"), 0, (LPARAM)m_pWallChanger);

	MainConfigSyncTabEnable();
	m_MainConfigDlg.ShowWindow(SW_SHOW);
	m_bInit = true;
	return FALSE;  // 傳回 TRUE，除非您對控制項設定焦點
}

void CMagicKDDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
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

void CMagicKDDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if ( !m_bInit )
		return;

	OnMove(cx, cy);
}

void CMagicKDDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	if ( !m_bInit )
		return;
	InitWindowRect();
}

void CMagicKDDlg::OnOK()
{
	m_pWallChanger->NewClassList();

//	CDialog::OnOK();
}

void CMagicKDDlg::OnTcnSelchangeMaintab(NMHDR *pNMHDR, LRESULT *pResult)
{
	CDialog *pCurDlg = (CDialog*)m_MainTab.GetCurItemLParam();
	if (pCurDlg) {
		pCurDlg->ShowWindow(SW_SHOW);
		pCurDlg->SetFocus();
		pCurDlg->MoveWindow(m_rcTabWindow);
	}

	*pResult = 0;
}

void CMagicKDDlg::OnTcnSelchangingMaintab(NMHDR *pNMHDR, LRESULT *pResult)
{
	CDialog *pCurDlg = (CDialog*)m_MainTab.GetCurItemLParam();
	if (pCurDlg) {
		pCurDlg->ShowWindow(SW_HIDE);
	}

	*pResult = 0;
}
