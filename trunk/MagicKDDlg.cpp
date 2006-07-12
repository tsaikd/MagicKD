#include "stdafx.h"
#include "Resource.h"
#include "Others.h"
#include "Language.h"
#include "MagicKD.h"
#include "WallConf.h"

#include "MagicKDDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CMagicKDDlg, CDialog)
CMagicKDDlg::CMagicKDDlg(CWnd* pParent /*=NULL*/)
	:	CDialog(CMagicKDDlg::IDD, pParent), m_bVisiable(true), m_bInit(false),
		m_pWallChangerDlg(NULL), m_pFindDupFileDlg(NULL), m_pPicCollectorDlg(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CMagicKDDlg::~CMagicKDDlg()
{
	DestroyIcon(m_hIcon);
	DEL(g_pTheTray);
}

BOOL CMagicKDDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 設定此對話方塊的圖示。當應用程式的主視窗不是對話方塊時，
	// 框架會自動從事此作業
	SetIcon(m_hIcon, TRUE);			// 設定大圖示
	SetIcon(m_hIcon, FALSE);		// 設定小圖示

	if (!g_pTheConf)
		g_pTheConf = new CMagicKDConf;
	g_pTheConf->Init(&theApp.m_cIni);
	LoadStringLib((LANGID)(UINT)g_pTheConf->m_General_uLangID);
	g_pTheAppEndDlg->SignWnd(GetSafeHwnd(), 2);
	if (!g_pTheTray)
		g_pTheTray = new CKDTray;

	m_cMainConfigDlg.Create(IDD_MAGICKD_CONFIG, this);
	m_cMainTab.InsertItem(TCIF_TEXT|TCIF_PARAM, 0, _T("MagicKD"), 0, (LPARAM)&m_cMainConfigDlg);

	g_pTheTray->AppendMenu(MF_STRING, IDS_TRAY_RESTART, CResString(IDS_TRAY_RESTART));
	g_pTheTray->AppendMenu(MF_STRING, IDS_TRAY_OPENWINDOW, CResString(IDS_TRAY_OPENWINDOW));
	g_pTheTray->AppendMenu(MF_STRING, IDS_TRAY_CLOSEWINDOW, CResString(IDS_TRAY_CLOSEWINDOW), true);

	// Check Necessary Library
	if (!theApp.IsOnQuit()) {
		HMODULE hLibTest = NULL;
		CStringList slTestLibName;
		CString sPath;
		CString sName;
		slTestLibName.AddTail(_T("cximage.dll"));
		slTestLibName.AddTail(_T("sqlite.dll"));

		while (!slTestLibName.IsEmpty()) {
			sName = slTestLibName.RemoveHead();
			sPath.Format(_T("%s%s"), theApp.GetAppDllDir(), sName);
			hLibTest = LoadLibrary(sPath);
			if (hLibTest) {
				FreeLibrary(hLibTest);
			} else {
				CString sMsg;
				sMsg.Format(CResString(IDS_MSG_NONECESSARYDLL), sPath);
				MessageBox(sMsg, NULL, MB_OK | MB_ICONERROR);
				m_bVisiable = false;
				theApp.Quit();
			}
		}

		if (!g_pTheTray->RegisterTray(GetSafeHwnd(), m_hIcon)) {
			if (IDYES == MessageBox(CResString(IDS_MSG_TRAYREGERROR), NULL, MB_YESNO | MB_ICONQUESTION)) {
				theApp.SetRestart();
				theApp.Quit();
			}
		}

		SetFuncEnable(eFunc_WallChanger, g_pTheConf->m_FuncList_bWallChanger, false);
		SetFuncEnable(eFunc_FindDupFile, g_pTheConf->m_FuncList_bFindDupFile, false);
		SetFuncEnable(eFunc_PicCollector, g_pTheConf->m_FuncList_bPicCollector, false);
	}

	m_cMainConfigDlg.UpdateFuncCheck();

	if (theApp.IsOnQuit() || g_pTheConf->m_General_bStartMin)
		m_bVisiable = false;

	ModifyStyleEx(0, WS_EX_LAYERED);
	SetTransparency((BYTE)(UINT)g_pTheConf->m_General_uTransparency);

//////////////////////////////////////////////////
#ifdef DEBUG
	m_cMainTab.SetCurSel(1);
#endif //DEBUG
	if (m_pWallChangerDlg)
		m_cMainTab.SetCurSel(eFunc_WallChanger);
#ifdef DEBUG
//	if (m_pFindDupFileDlg)
//		m_cMainTab.SetCurSel(eFunc_FindDupFile);
//	if (m_pPicCollectorDlg)
//		m_cMainTab.SetCurSel(eFunc_PicCollector);
#endif //DEBUG
//////////////////////////////////////////////////

	m_bInit = true;
	DoSize();

	return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
}

void CMagicKDDlg::OnDestroy()
{
	if (g_pTheConf->m_General_bShowCloseWindow)
		g_pTheAppEndDlg->ShowWindow(SW_SHOW);

	g_pTheAppEndDlg->ProgressStepIt(GetSafeHwnd(), _T("Closing\tMagicKD\t\tDialog"));
	SetFuncEnable(eFunc_WallChanger, false, false);
	SetFuncEnable(eFunc_FindDupFile, false, false);
	SetFuncEnable(eFunc_PicCollector, false, false);

	CDialog::OnDestroy();

	g_pTheAppEndDlg->ProgressStepIt(GetSafeHwnd(), _T("Deleting\tMagicKD\t\tTray Menu"));
	g_pTheTray->RemoveTrayMenuItem(IDS_TRAY_CLOSEWINDOW);
	g_pTheTray->RemoveTrayMenuItem(IDS_TRAY_OPENWINDOW);
	g_pTheTray->RemoveTrayMenuItem(IDS_TRAY_RESTART);

	DEL(g_pTheConf);
}

void CMagicKDDlg::DoSize()
{
	if (!m_bInit)
		return;

	GetClientRect(m_rcMainTab);
	m_rcMainTab.bottom += 2;
	m_rcMainTab.right += 2;
	CRect rcWin = m_rcMainTab;
	rcWin.bottom = rcWin.top + 22;
	m_cMainTab.MoveWindow(rcWin, FALSE);

	rcWin = m_rcMainTab;
	rcWin.top += 23;
	rcWin.bottom -= 5;
	rcWin.left += 2;
	rcWin.right -= 5;

	CDialog *pCurDlg = (CDialog *)m_cMainTab.GetCurItemLParam();
	if (pCurDlg) {
		pCurDlg->MoveWindow(rcWin, FALSE);
		pCurDlg->ShowWindow(SW_SHOW);
		pCurDlg->SetFocus();
	}

	Invalidate();
}

void CMagicKDDlg::Localize()
{
	LoadStringLib((LANGID)(UINT)g_pTheConf->m_General_uLangID);

	g_pTheTray->ModifyMenu(IDS_TRAY_RESTART, CResString(IDS_TRAY_RESTART));
	g_pTheTray->ModifyMenu(IDS_TRAY_OPENWINDOW, CResString(IDS_TRAY_OPENWINDOW));
	g_pTheTray->ModifyMenu(IDS_TRAY_CLOSEWINDOW, CResString(IDS_TRAY_CLOSEWINDOW));

	m_cMainConfigDlg.Localize();

	if (m_pWallChangerDlg)
		m_pWallChangerDlg->Localize();
	if (m_pFindDupFileDlg)
		m_pFindDupFileDlg->Localize();
	if (m_pPicCollectorDlg)
		m_pPicCollectorDlg->Localize();
}

void CMagicKDDlg::SetFuncEnable(FuncList eFunc, bool bEnable, bool bRedraw/* = true*/)
{
	CString sTabName;
	CDialog *pDlg = NULL;
	int nID = 0;
	int iItemPos = -1;

	switch (eFunc) {
	case eFunc_NULL:
		break;
	case eFunc_WallChanger:
		sTabName = _T("WallChanger");
		nID = IDD_MAGICKD_WALLCHANGER;
		if (bEnable) {
			if (m_pWallChangerDlg)
				SetFuncEnable(eFunc, false, false);
			pDlg = m_pWallChangerDlg = new CWallChangerDlg;
		} else {
			pDlg = m_pWallChangerDlg;
			m_pWallChangerDlg = NULL;
		}
		break;
	case eFunc_FindDupFile:
		sTabName = _T("FindDupFile");
		nID = IDD_MAGICKD_FINDDUPFILE;
		if (bEnable) {
			if (m_pFindDupFileDlg)
				SetFuncEnable(eFunc, false, false);
			pDlg = m_pFindDupFileDlg = new CFindDupFileDlg;
		} else {
			pDlg = m_pFindDupFileDlg;
			m_pFindDupFileDlg = NULL;
		}
		break;
	case eFunc_PicCollector:
		sTabName = _T("PicCollector");
		nID = IDD_MAGICKD_PICCOLLECTOR;
		if (bEnable) {
			if (m_pPicCollectorDlg)
				SetFuncEnable(eFunc, false, false);
			pDlg = m_pPicCollectorDlg = new CPicCollectorDlg;
		} else {
			pDlg = m_pPicCollectorDlg;
			m_pPicCollectorDlg = NULL;
		}
		break;
	}

	if (bEnable) {
		if (pDlg) {
			pDlg->Create(nID, this);
			pDlg->ShowWindow(SW_HIDE);
			m_cMainTab.InsertItem(TCIF_TEXT|TCIF_PARAM, eFunc, sTabName, 0, (LPARAM)pDlg);

			int nItem = g_pTheTray->FindTrayMenuItem(sTabName);
			if (nItem >= 0)
				g_pTheTray->CheckMenuItem(nItem, MF_BYPOSITION | MF_CHECKED);
		}
	} else {
		if (pDlg){
			iItemPos = m_cMainTab.FindItemPos(sTabName);
			if (iItemPos >=0)
				m_cMainTab.DeleteItem(iItemPos);
			pDlg->DestroyWindow();
			delete pDlg;

			int nItem = g_pTheTray->FindTrayMenuItem(sTabName);
			if (nItem >= 0)
				g_pTheTray->CheckMenuItem(nItem, MF_BYPOSITION | MF_UNCHECKED);
		}
	}

	m_cMainConfigDlg.UpdateFuncCheck();
	if (-1 == m_cMainTab.GetCurSel())
		m_cMainTab.SetCurSel(0);

	if (bRedraw)
		Invalidate();
}

CMagicKDDlg::FuncList CMagicKDDlg::GetFuncFromCWnd(CWnd *pWnd)
{
	VERIFY(::IsWindow(pWnd->GetSafeHwnd()));

	if (pWnd == m_pWallChangerDlg)
		return eFunc_WallChanger;
	else if (pWnd == m_pFindDupFileDlg)
		return eFunc_FindDupFile;
	else if (pWnd == m_pPicCollectorDlg)
		return eFunc_PicCollector;
	else
		return eFunc_NULL;
}

bool CMagicKDDlg::SetTransparency(BYTE uAlpha)
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
	ON_WM_SIZE()
	ON_WM_WINDOWPOSCHANGING()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MAIN, OnTcnSelchangeMaintab)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB_MAIN, OnTcnSelchangingMaintab)
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
	CDialog::OnCancel();
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

void CMagicKDDlg::OnTcnSelchangingMaintab(NMHDR *pNMHDR, LRESULT *pResult)
{
	ASSERT(::IsWindow(pNMHDR->hwndFrom));
	CDialog *pCurDlg = (CDialog *)m_cMainTab.GetCurItemLParam();
	if (pCurDlg)
		pCurDlg->ShowWindow(SW_HIDE);
	*pResult = 0;
}

void CMagicKDDlg::OnTcnSelchangeMaintab(NMHDR *pNMHDR, LRESULT *pResult)
{
	ASSERT(::IsWindow(pNMHDR->hwndFrom));
	DoSize();

	*pResult = 0;
}

LRESULT CMagicKDDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	//FILE *fp = fopen("C:\\abc", "a");
	//fprintf(fp, "%08X, %08X, %08X\n", message, wParam, lParam);
	//fflush(fp);
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
			g_pTheTray->TrackPopupMenu(this);
			Invalidate();
			break;
		}
	} else {
		switch (message) {
		case WM_COMMAND:
			{
			UINT nID = LOWORD(wParam);
			switch (nID) {
			// TabCtrl
			case IDS_MENU_CLOSE:
				{
				CDialog *pDlg = (CDialog *)m_cMainTab.GetCurItemLParam();
				if (pDlg)
					SetFuncEnable(GetFuncFromCWnd(pDlg), false);
				DoSize();
				}
				break;
			// Tray Icon
			case IDS_TRAY_RESTART:
				theApp.SetRestart();
				theApp.Quit();
				break;
			case IDS_TRAY_OPENWINDOW:
				ShowWindow(SW_SHOW);
				break;
			case IDS_TRAY_CLOSEWINDOW:
				theApp.Quit();
				break;
			default:
				if (m_pWallChangerDlg)
					::PostMessage(m_pWallChangerDlg->GetSafeHwnd(), message, wParam, lParam);
				if (m_pFindDupFileDlg)
					::PostMessage(m_pFindDupFileDlg->GetSafeHwnd(), message, wParam, lParam);
				break;
			}
			}
			break;
		case WM_QUERYENDSESSION:
			if (g_pTheConf) {
				delete g_pTheConf;
				g_pTheConf = NULL;
			}
			if (::g_pWallConf) {
				delete ::g_pWallConf;
				::g_pWallConf = NULL;
			}
			break;
		}
	}

	return __super::DefWindowProc(message, wParam, lParam);
}
