// MagicKDDlg.cpp : ��@��
//

#include "stdafx.h"
#include "MagicKD.h"
#include "MagicKDDlg.h"
#include ".\magickddlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// �� App About �ϥ� CAboutDlg ��ܤ��

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ��ܤ�����
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �䴩

// �{���X��@
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


// CMagicKDDlg ��ܤ��



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


// CMagicKDDlg �T���B�z�`��

BOOL CMagicKDDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �N "����..." �\���[�J�t�Υ\���C

	// IDM_ABOUTBOX �����b�t�ΩR�O�d�򤧤��C
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

	// �]�w����ܤ�����ϥܡC�����ε{�����D�������O��ܤ���ɡA
	// �ج[�|�۰ʱq�Ʀ��@�~
	SetIcon(m_hIcon, TRUE);			// �]�w�j�ϥ�
	SetIcon(m_hIcon, FALSE);		// �]�w�p�ϥ�

	// TODO: �b���[�J�B�~����l�]�w
	m_MainTab.InsertItem(TCIF_PARAM | TCIF_TEXT, 1, _T("WallChanger"), 0, (LPARAM) NULL);
	m_WallChanger.Create(IDD_WALLCHANGER, this);
	m_WallChanger.ShowWindow(SW_SHOW);

	m_bInit = true;
	return TRUE;  // �Ǧ^ TRUE�A���D�z�ﱱ��]�w�J�I
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
