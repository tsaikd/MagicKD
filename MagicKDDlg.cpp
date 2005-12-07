// MagicKDDlg.cpp : 實作檔
//

#include "stdafx.h"
#include "MagicKD.h"
#include "MagicKDDlg.h"
#include ".\magickddlg.h"

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
	: CDialog(CMagicKDDlg::IDD, pParent), m_bInit(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
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
	m_MainTab.InsertItem(TCIF_PARAM | TCIF_TEXT, 1, _T("WallChanger"), 0, (LPARAM) NULL);
	m_WallChanger.Create(IDD_WALLCHANGER, this);
	m_WallChanger.ShowWindow(SW_SHOW);

	m_bInit = true;
	return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
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

	CRect rcWin;
	GetClientRect(rcWin);
	m_MainTab.MoveWindow(rcWin);
	m_MainTab.GetClientRect(rcWin);
	rcWin.top += 35;
	rcWin.right -= 10;
	rcWin.bottom -= 10;
	m_WallChanger.MoveWindow(rcWin);
}
