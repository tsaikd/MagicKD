#include "stdafx.h"
#include "Resource.h"
#include "Language.h"
#include "Others.h"
#include "WallOthers.h"
#include "MagicKD.h"
#include "KDWinMsgBox.h"
#include "FindDFConf.h"
#include "FindDFOthers.h"

#include "FindDupFileDlg.h"

enum {
	KDT_NULL,
	KDT_SORTTING
};

CFindDupFileDlg *g_pFindDupFileDlg = NULL;
CFindDFConf *g_pFindConf = NULL;

IMPLEMENT_DYNAMIC(CFindDupFileDlg, CDialog)
CFindDupFileDlg::CFindDupFileDlg(CWnd* pParent /*=NULL*/)
	:	CDialog(CFindDupFileDlg::IDD, pParent), m_bInit(false), m_bStop(false), m_iThreadState(0)
{
}

CFindDupFileDlg::~CFindDupFileDlg()
{
}

DWORD CFindDupFileDlg::ThreadProc()
{
	m_bStop = false;

	switch (m_iThreadState) {
	case KDSTATE_FINDDUP:
		{
		bool bFindSameFile = false;
		CFindDupFileProc *pImg;
		CString sPath;
		CFileFind finder;

		clock_t clockStart = clock();

		m_progress_FindDF.SetRange32(0, 10000);
		m_progress_FindDF.SetPos(0);
		SetNowPicPath(NULL);
		m_aDupFile.RemoveAll();
		m_tree_FindResult.SelectItem(NULL);
		m_tree_FindResult.SetRedraw(FALSE);
		m_tree_FindResult.DeleteAllItems();
		m_tree_FindResult.SetRedraw(TRUE);
		m_progress_FindDF.SetPos(500);

		int i, iCount = m_list_FindDupFileList.GetItemCount();
		for (i=0 ; i<iCount ; i++) {
			if (!IsCanThread() || m_bStop)
				goto Lable_ExitFindDupFileThread;

			sPath = m_list_FindDupFileList.GetItemText(i, 0);
			_FindAllFileAndAddToArray(&m_aDupFile, sPath);
		}
		m_progress_FindDF.SetPos(2000);

		if (!IsCanThread() || m_bStop)
			goto Lable_ExitFindDupFileThread;
		SetTimer(KDT_SORTTING, 1000, NULL);

		// Use another Quick Sort algorithm
		m_aDupFile.QuickSort(true, &m_bStop);
//		qsort(m_aDupFile.GetData(), m_aDupFile.GetCount(), sizeof(CFindDupFileProc *), FileCmpCB);

		KillTimer(KDT_SORTTING);
		m_progress_FindDF.SetPos(8000);

		POSITION pos;
		HTREEITEM hTreeItem;
		HTREEITEM hTreeChildItem;
		CListFindDupFileProc *pListSameFile;
		iCount = m_aDupFile.GetCount();
		for (i=0 ; i<iCount ; i++) {
			if (!IsCanThread())
				goto Lable_ExitFindDupFileThread;

			m_progress_FindDF.OffsetPos(1000 / iCount);
			pImg = m_aDupFile[i];
			pListSameFile = pImg->m_pListSameFile;
			if (pListSameFile) {
				bFindSameFile = true;
				hTreeItem = m_tree_FindResult.InsertItem(pImg->m_sFilePath);
				pos = pListSameFile->GetHeadPosition();
				while (pos) {
					if (!IsCanThread())
						goto Lable_ExitFindDupFileThread;

					pImg = pListSameFile->GetNext(pos);
					hTreeChildItem = m_tree_FindResult.InsertItem(pImg->m_sFilePath, hTreeItem);
					m_tree_FindResult.SetCheck(hTreeChildItem);
				}
			}
		}
		m_progress_FindDF.SetPos(9000);

Lable_ExitFindDupFileThread:
		iCount = m_aDupFile.GetCount();
		for (i=0 ; i<iCount ; i++)
			delete m_aDupFile[i];
		m_aDupFile.RemoveAll();
		m_progress_FindDF.SetPos(10000);

		if (IsCanThread()) {
			double dExecTime = (double)(clock() - clockStart) / CLOCKS_PER_SEC;
			CString sMsg;
			if (dExecTime < 60) {
				sMsg.Format(_T("%s%.3f (s)"), CResString(IDS_FIND_MSG_FINDTIME), dExecTime);
			} else {
				sMsg.Format(_T("%s%.0f:%02d (s)"), CResString(IDS_FIND_MSG_FINDTIME), dExecTime/60, (ULONGLONG)dExecTime%60);
			}
			if (!bFindSameFile)
				sMsg.AppendFormat(_T("\n%s"), CResString(IDS_FIND_MSG_NOSAMEFILE));
			g_pKDWinMsgBox->Add(GetSafeHwnd(), sMsg, NULL, MB_OK | MB_ICONINFORMATION);

			m_tree_FindResult.Invalidate();
			_EW_Finding(false);

			if (m_tree_FindResult.GetCount())
				_EW_FindOver(true);
			else
				_EW_FindOver(false);
		}
		}
		break;
	case KDSTATE_DELDUP:
		_DeleteDupFile();
		break;
	}

	m_iThreadState = KDSTATE_NULL;
	return 0;
}

BOOL CFindDupFileDlg::OnInitDialog()
{
	g_pTheLog->Log(_T("Init FindDupFileDlg"), CKDLog::LOGLV_INFO);
	if (!g_pFindDupFileDlg)
		g_pFindDupFileDlg = this;

	CDialog::OnInitDialog();

	m_cIni.SetPathName(CString(theApp.GetAppConfDir()) + _T("FindDupFile.ini"));
	if (!g_pFindConf) {
		g_pFindConf = new CFindDFConf;
		g_pFindConf->Init(&m_cIni);
	}

	m_list_FindDupFileList.Init();
	m_tree_FindResult.ModifyStyle(0, TVS_CHECKBOXES);	// 直接在 VC 的資源選單選取會有 bug
	_EW_Finding(false);
	_EW_FindOver(false);

	m_bInit = true;
	Localize();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX 屬性頁應傳回 FALSE
}

void CFindDupFileDlg::OnDestroy()
{
	g_pTheLog->Log(_T("Exit FindDupFileDlg"), CKDLog::LOGLV_INFO);
	OnBnClickedFindBtnStopfind();
	SetCanThread(false);
	WaitForThread(3000);

	CDialog::OnDestroy();

	DEL(g_pFindConf);
	g_pFindDupFileDlg = NULL;
}

void CFindDupFileDlg::DoSize()
{
	if (!m_bInit)
		return;

	int iMarginRight = 15;
	int iMarginBottom = 15;
	CRect rcItem;

	KDMoveDlgItem(GetDlgItem(IDC_FIND_BTN_ENABLETOOLTIP), this,
		KDMOVEDLGITEM_WAY_RIGHT | KDMOVEDLGITEM_WAY_F_INSIDE, iMarginRight);
	KDMoveDlgItem(GetDlgItem(IDC_FIND_LIST_FINDLIST), this,
		KDMOVEDLGITEM_WAY_RIGHT | KDMOVEDLGITEM_WAY_F_INSIDE, iMarginRight, true);
	KDMoveDlgItem(GetDlgItem(IDC_FIND_LIST_FINDLIST), GetDlgItem(IDC_FIND_BTN_ENABLETOOLTIP),
		KDMOVEDLGITEM_WAY_BOTTOM | KDMOVEDLGITEM_WAY_F_OUTSIDE, 5);
	m_list_FindDupFileList.GetClientRect(rcItem);
	m_list_FindDupFileList.SetColumnWidth(0, rcItem.Width());

	KDMoveDlgItem(GetDlgItem(IDC_FIND_BTN_STARTFIND), this,
		KDMOVEDLGITEM_WAY_RIGHT | KDMOVEDLGITEM_WAY_F_INSIDE, iMarginRight);
	KDMoveDlgItem(GetDlgItem(IDC_FIND_BTN_STOPFIND), GetDlgItem(IDC_FIND_BTN_STARTFIND),
		KDMOVEDLGITEM_WAY_LEFT | KDMOVEDLGITEM_WAY_F_OUTSIDE, 5);
	KDMoveDlgItem(GetDlgItem(IDC_FIND_PROGRESS_FINDDF), GetDlgItem(IDC_FIND_BTN_STOPFIND),
		KDMOVEDLGITEM_WAY_LEFT | KDMOVEDLGITEM_WAY_F_OUTSIDE, 5, true);

	KDMoveDlgItem(GetDlgItem(IDC_FIND_STATIC_NOWPICPATH), this,
		KDMOVEDLGITEM_WAY_RIGHT | KDMOVEDLGITEM_WAY_F_INSIDE, iMarginRight, true);

	KDMoveDlgItem(GetDlgItem(IDC_FIND_STATIC_SHOWPIC), this,
		KDMOVEDLGITEM_WAY_RIGHT | KDMOVEDLGITEM_WAY_F_INSIDE, iMarginRight);
	KDMoveDlgItem(GetDlgItem(IDC_FIND_STATIC_SHOWPIC), this,
		KDMOVEDLGITEM_WAY_BOTTOM | KDMOVEDLGITEM_WAY_F_INSIDE, iMarginBottom, true);
	GetDlgItem(IDC_FIND_STATIC_SHOWPIC)->GetWindowRect(rcItem);
	rcItem.left = rcItem.right - rcItem.Height();
	ScreenToClient(rcItem);
	GetDlgItem(IDC_FIND_STATIC_SHOWPIC)->MoveWindow(rcItem, FALSE);

	KDMoveDlgItem(GetDlgItem(IDC_FIND_TREE_FINDRESULT), this,
		KDMOVEDLGITEM_WAY_BOTTOM | KDMOVEDLGITEM_WAY_F_INSIDE, iMarginBottom, true);
	KDMoveDlgItem(GetDlgItem(IDC_FIND_TREE_FINDRESULT), GetDlgItem(IDC_FIND_STATIC_SHOWPIC),
		KDMOVEDLGITEM_WAY_LEFT | KDMOVEDLGITEM_WAY_F_OUTSIDE, 5, true);

	Invalidate();
}

void CFindDupFileDlg::Localize()
{
	if (!m_bInit)
		return;

	GetDlgItem(IDC_FIND_BTN_STARTFIND)->SetWindowText(CResString(IDS_FIND_BTN_STARTFIND));
	GetDlgItem(IDC_FIND_BTN_STOPFIND)->SetWindowText(CResString(IDS_FIND_BTN_STOPFIND));
	GetDlgItem(IDC_FIND_BTN_EXPANDALL)->SetWindowText(CResString(IDS_FIND_BTN_EXPANDALL));
	GetDlgItem(IDC_FIND_BTN_COLLAPSEALL)->SetWindowText(CResString(IDS_FIND_BTN_COLLAPSEALL));
	GetDlgItem(IDC_FIND_BTN_SELECTDUP)->SetWindowText(CResString(IDS_FIND_BTN_SELECTDUP));
	GetDlgItem(IDC_FIND_BTN_SELECTNONE)->SetWindowText(CResString(IDS_FIND_BTN_SELECTNONE));
	GetDlgItem(IDC_FIND_BTN_DELETEDUP)->SetWindowText(CResString(IDS_FIND_BTN_DELETEDUP));
	GetDlgItem(IDC_FIND_STATIC_DUPFILELIST)->SetWindowText(CResString(IDS_FIND_STATIC_DUPFILELIST));

	m_list_FindDupFileList.Localize();
	EnableToolTips(g_pFindConf->m_bEnableToolTip);
}

void CFindDupFileDlg::AddFindPath(LPCTSTR lpPath)
{
	if (PathFileExists(lpPath))
		m_list_FindDupFileList.InsertItem(m_list_FindDupFileList.GetItemCount(), lpPath);
}

void CFindDupFileDlg::EnableToolTips(bool bEnable/* = true*/)
{
	GetDlgItem(IDC_FIND_BTN_ENABLETOOLTIP)->SetWindowText(
		bEnable ? CResString(IDS_ALL_BTN_DISABLETIP) : CResString(IDS_ALL_BTN_ENABLETIP));

	m_list_FindDupFileList.EnableToolTips(CResString(IDS_FIND_TOOLTIP_FINDLIST), bEnable);
}

void CFindDupFileDlg::SetFindResultTreeCheck(bool bCheck/* = true*/)
{
	HTREEITEM hTreeItem = m_tree_FindResult.GetRootItem();
	HTREEITEM hTreeChildItem;

	while (hTreeItem) {
		hTreeChildItem = m_tree_FindResult.GetChildItem(hTreeItem);

		while (hTreeChildItem) {
			m_tree_FindResult.SetCheck(hTreeChildItem, bCheck ? TRUE : FALSE);

			hTreeChildItem = m_tree_FindResult.GetNextSiblingItem(hTreeChildItem);
		}

		hTreeItem = m_tree_FindResult.GetNextSiblingItem(hTreeItem);
	}
}

void CFindDupFileDlg::SetFindResultTreeExpand(bool bExpand/* = true*/)
{
	HTREEITEM hTreeItem = m_tree_FindResult.GetRootItem();

	while (hTreeItem) {
		m_tree_FindResult.Expand(hTreeItem, bExpand ? TVE_EXPAND : TVE_COLLAPSE);

		hTreeItem = m_tree_FindResult.GetNextSiblingItem(hTreeItem);
	}
}

void CFindDupFileDlg::SetNowPicPath(LPCTSTR lpPath/* = NULL*/)
{
	GetDlgItem(IDC_FIND_STATIC_NOWPICPATH)->SetWindowText(lpPath);
	m_static_ShowPic.SetPicPath(lpPath);
	m_static_ShowPic.Invalidate();
}

void CFindDupFileDlg::_EW_Finding(bool bEnable)
{
	GetDlgItem(IDC_FIND_BTN_STARTFIND)->EnableWindow(!bEnable);
	GetDlgItem(IDC_FIND_BTN_STOPFIND)->EnableWindow(bEnable);
	GetDlgItem(IDC_FIND_LIST_FINDLIST)->EnableWindow(!bEnable);
}

void CFindDupFileDlg::_EW_FindOver(bool bEnable)
{
	GetDlgItem(IDC_FIND_BTN_EXPANDALL)->EnableWindow(bEnable);
	GetDlgItem(IDC_FIND_BTN_COLLAPSEALL)->EnableWindow(bEnable);
	GetDlgItem(IDC_FIND_BTN_SELECTDUP)->EnableWindow(bEnable);
	GetDlgItem(IDC_FIND_BTN_SELECTNONE)->EnableWindow(bEnable);
	GetDlgItem(IDC_FIND_BTN_DELETEDUP)->EnableWindow(bEnable);
	GetDlgItem(IDC_FIND_TREE_FINDRESULT)->EnableWindow(bEnable);
}

void CFindDupFileDlg::_FindAllFileAndAddToArray(void *pArray, LPCTSTR sPath)
{
	if (!pArray || !sPath)
		return;

	CFindDupFileProc *pImg;
	CArray<CFindDupFileProc *, CFindDupFileProc *> *paFile = (CArray<CFindDupFileProc *, CFindDupFileProc *> *)pArray;
	CFileFind finder;

	if (PathIsDirectory(sPath)) {
		CString strWildcard(sPath);
		strWildcard += _T("\\*.*");

		BOOL bWorking = finder.FindFile(strWildcard);
		while (bWorking && IsCanThread() && !m_bStop) {
			bWorking = finder.FindNextFile();

			if (finder.IsDots())
				continue;

			if (finder.IsDirectory()) {
				_FindAllFileAndAddToArray(pArray, finder.GetFilePath());
			} else {
				pImg = new CFindDupFileProc();
				pImg->m_sFilePath = finder.GetFilePath();
				pImg->m_u64FileSize = finder.GetLength();
				finder.GetLastWriteTime(&pImg->m_ftLastWriteTime);
				paFile->Add(pImg);
			}
		}
	} else if (finder.FindFile(sPath) && IsCanThread()) {
		finder.FindNextFile();
		pImg = new CFindDupFileProc();
		pImg->m_sFilePath = finder.GetFilePath();
		pImg->m_u64FileSize = finder.GetLength();
		finder.GetLastWriteTime(&pImg->m_ftLastWriteTime);
		paFile->Add(pImg);
	}
}

void CFindDupFileDlg::_DeleteDupFile()
{
	CString sPath;
	HTREEITEM hTreeItem = m_tree_FindResult.GetRootItem();
	HTREEITEM hTreeChildItem;

	if (!hTreeItem) {
		MessageBox(CResString(IDS_FIND_MSG_DOFINDFIRST), NULL, MB_OK | MB_ICONERROR);
		GetDlgItem(IDC_FIND_BTN_DELETEDUP)->EnableWindow();
		return;
	}

	if (IDOK != MessageBox(CResString(IDS_FIND_MSG_DELETEDUP), NULL, MB_OKCANCEL | MB_ICONQUESTION)) {
		GetDlgItem(IDC_FIND_BTN_DELETEDUP)->EnableWindow();
		return;
	}

	GetDlgItem(IDC_FIND_BTN_DELETEDUP)->EnableWindow(FALSE);
	m_tree_FindResult.EnableWindow(FALSE);
	m_tree_FindResult.SetRedraw(FALSE);
	m_static_ShowPic.SetRedraw(FALSE);

	while (hTreeItem && IsCanThread()) {
		if (m_tree_FindResult.GetCheck(hTreeItem)) {
			sPath = m_tree_FindResult.GetItemText(hTreeItem);
			if (PathFileExists(sPath))
				DeleteFile(sPath);
		}

		hTreeChildItem = m_tree_FindResult.GetChildItem(hTreeItem);

		while (hTreeChildItem) {
			if (m_tree_FindResult.GetCheck(hTreeChildItem)) {
				sPath = m_tree_FindResult.GetItemText(hTreeChildItem);
				if (PathFileExists(sPath))
					DeleteFile(sPath);
			}

			hTreeChildItem = m_tree_FindResult.GetNextSiblingItem(hTreeChildItem);
		}

		hTreeItem = m_tree_FindResult.GetNextSiblingItem(hTreeItem);
	}

	m_tree_FindResult.DeleteAllItems();
	m_static_ShowPic.SetPicPath(NULL);
	m_tree_FindResult.SetRedraw(TRUE);
	m_static_ShowPic.SetRedraw(TRUE);
	m_tree_FindResult.Invalidate();
	m_static_ShowPic.Invalidate();
	m_tree_FindResult.EnableWindow();
	GetDlgItem(IDC_FIND_BTN_DELETEDUP)->EnableWindow();
}

BEGIN_MESSAGE_MAP(CFindDupFileDlg, CDialog)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_NOTIFY(TVN_SELCHANGED, IDC_FIND_TREE_FINDRESULT, OnTvnSelchangedFindTreeFindresult)
	ON_BN_CLICKED(IDC_FIND_BTN_ENABLETOOLTIP, OnBnClickedFindBtnEnabletooltip)
	ON_BN_CLICKED(IDC_FIND_BTN_STARTFIND, OnBnClickedFindBtnStartfind)
	ON_BN_CLICKED(IDC_FIND_BTN_SELECTDUP, OnBnClickedFindBtnSelectdup)
	ON_BN_CLICKED(IDC_FIND_BTN_SELECTNONE, OnBnClickedFindBtnSelectnone)
	ON_BN_CLICKED(IDC_FIND_BTN_DELETEDUP, OnBnClickedFindBtnDeletedup)
	ON_BN_CLICKED(IDC_FIND_BTN_STOPFIND, OnBnClickedFindBtnStopfind)
	ON_BN_CLICKED(IDC_FIND_BTN_EXPANDALL, OnBnClickedFindBtnExpandall)
	ON_BN_CLICKED(IDC_FIND_BTN_COLLAPSEALL, OnBnClickedFindBtnCollapseall)
END_MESSAGE_MAP()

void CFindDupFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FIND_LIST_FINDLIST, m_list_FindDupFileList);
	DDX_Control(pDX, IDC_FIND_TREE_FINDRESULT, m_tree_FindResult);
	DDX_Control(pDX, IDC_FIND_STATIC_SHOWPIC, m_static_ShowPic);
	DDX_Control(pDX, IDC_FIND_PROGRESS_FINDDF, m_progress_FindDF);
}

void CFindDupFileDlg::OnOK()
{
//	CDialog::OnOK();
}

void CFindDupFileDlg::OnCancel()
{
	((CDialog *)GetParent())->EndDialog(IDCANCEL);

//	CDialog::OnCancel();
}

void CFindDupFileDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	DoSize();
}

void CFindDupFileDlg::OnTimer(UINT nIDEvent)
{
	switch (nIDEvent) {
	case KDT_SORTTING:
		{
		// Progress range 2000 to 8000
		int iPos = (int)(m_aDupFile.GetQSortProgress() * 6000 + 2000);
		if (iPos < 2000)
			iPos = 2000;
		if (iPos > 8000)
			iPos = 8000;
		m_progress_FindDF.SetPos(iPos);
		}
		break;
	default:
		KillTimer(nIDEvent);
		break;
	}

	__super::OnTimer(nIDEvent);
}

void CFindDupFileDlg::OnBnClickedFindBtnEnabletooltip()
{
	g_pFindConf->m_bEnableToolTip = !g_pFindConf->m_bEnableToolTip;

	EnableToolTips(g_pFindConf->m_bEnableToolTip);
}

void CFindDupFileDlg::OnBnClickedFindBtnStartfind()
{
	_EW_Finding(true);
	m_iThreadState = KDSTATE_FINDDUP;
	CreateThread(THREAD_PRIORITY_LOWEST);
	SetFocus();
}

void CFindDupFileDlg::OnBnClickedFindBtnStopfind()
{
	m_bStop = true;
}

void CFindDupFileDlg::OnBnClickedFindBtnExpandall()
{
	SetFindResultTreeExpand();
	HTREEITEM hItem = m_tree_FindResult.GetRootItem();
	if (hItem)
		m_tree_FindResult.SelectItem(hItem);
}

void CFindDupFileDlg::OnBnClickedFindBtnCollapseall()
{
	SetFindResultTreeExpand(false);
	HTREEITEM hItem = m_tree_FindResult.GetRootItem();
	if (hItem)
		m_tree_FindResult.SelectItem(hItem);
}

void CFindDupFileDlg::OnBnClickedFindBtnSelectdup()
{
	SetFindResultTreeCheck();
}

void CFindDupFileDlg::OnBnClickedFindBtnSelectnone()
{
	SetFindResultTreeCheck(false);
}

void CFindDupFileDlg::OnBnClickedFindBtnDeletedup()
{
	GetDlgItem(IDC_FIND_BTN_DELETEDUP)->EnableWindow(FALSE);
	m_iThreadState = KDSTATE_DELDUP;
	CreateThread(THREAD_PRIORITY_LOWEST);
}

void CFindDupFileDlg::OnTvnSelchangedFindTreeFindresult(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	if (pNMTreeView->itemNew.hItem)
		SetNowPicPath(m_tree_FindResult.GetItemText(pNMTreeView->itemNew.hItem));

	*pResult = 0;
}

LRESULT CFindDupFileDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message) {
	case WM_COMMAND:
		{
			UINT nID = LOWORD(wParam);
			switch (nID) {
			// FindList
			case IDS_FIND_MENU_ADDFINDPATH:
				{
					CString sPath;
					if (ChooseFolder(sPath, GetSafeHwnd()))
						AddFindPath(sPath);
				}
				break;
			case IDS_FIND_MENU_REMOVEFINDPATH:
				m_list_FindDupFileList.DeleteSelectItem();
				break;
			// ResultTree
			case IDS_FIND_MENU_OPENFILE:
				{
					HTREEITEM hItem = m_tree_FindResult.m_hContextItem;
					if (hItem) {
						CString sPath = m_tree_FindResult.GetItemText(hItem);
						if (!sPath.IsEmpty() && PathFileExists(sPath))
							ShellExecute(NULL, _T("open"), sPath, NULL, NULL, SW_SHOW);
					}
				}
				break;
			case IDS_FIND_MENU_OPENDIR:
				{
					HTREEITEM hItem = m_tree_FindResult.m_hContextItem;
					if (hItem) {
						CString sPath = m_tree_FindResult.GetItemText(hItem);
						if (!sPath.IsEmpty())
							ExplorerFile(sPath);
					}
				}
				break;
			}
		}
		break;
	}

	return __super::DefWindowProc(message, wParam, lParam);
}
