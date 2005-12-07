// WallChanger.cpp : 實作檔
//

#include "stdafx.h"
#include "MagicKD.h"
#include "WallChanger.h"


// CWallChanger 對話方塊

IMPLEMENT_DYNAMIC(CWallChanger, CDialog)
CWallChanger::CWallChanger(CWnd* pParent /*=NULL*/)
	: CDialog(CWallChanger::IDD, pParent), m_bInit(false)
{
}

CWallChanger::~CWallChanger()
{
}

void CWallChanger::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_WALLTREE, m_FileTree);
	DDX_Control(pDX, IDC_WALLLIST, m_FileList);
	DDX_Control(pDX, IDC_WALLSTATIC1, m_TreeStatic);
	DDX_Control(pDX, IDC_WALLSTATIC2, m_ListStatic);
}


BEGIN_MESSAGE_MAP(CWallChanger, CDialog)
	ON_WM_MOVE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CWallChanger 訊息處理常式

void CWallChanger::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if (!m_bInit)
		return;

	CRect rcWin;

	GetClientRect(rcWin);
	rcWin.top = 20;
	rcWin.left = 10;
	rcWin.right = rcWin.right/2 - 10;
	m_FileTree.MoveWindow(rcWin);
	rcWin.top = 0;
	rcWin.bottom = 20;
	m_TreeStatic.MoveWindow(rcWin);

	GetClientRect(rcWin);
	rcWin.top = 20;
	rcWin.left = rcWin.right/2 + 10;
	m_FileList.MoveWindow(rcWin);
	rcWin.top = 0;
	rcWin.bottom = 20;
	m_ListStatic.MoveWindow(rcWin);
}

BOOL CWallChanger::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_bInit = true;

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX 屬性頁應傳回 FALSE
}
