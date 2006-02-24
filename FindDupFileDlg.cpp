#include "stdafx.h"
#include "Resource.h"
#include "Language.h"
#include "Others.h"
#include "WallOthers.h"
#include "MagicKD.h"
#include "FindDFConf.h"
#include "FindDFOthers.h"
#include "FindDupFileProc.h"

#include "FindDupFileDlg.h"

CFindDFConf *g_pFindConf = NULL;

IMPLEMENT_DYNAMIC(CFindDupFileDlg, CDialog)
CFindDupFileDlg::CFindDupFileDlg(CWnd* pParent /*=NULL*/)
	:	CDialog(CFindDupFileDlg::IDD, pParent), m_bInit(false)
{
}

CFindDupFileDlg::~CFindDupFileDlg()
{
}

DWORD CFindDupFileDlg::ThreadProc()
{
	CFindDupFileProc *pImg;
	CArray<CFindDupFileProc *, CFindDupFileProc *> aDupFile;
	CString sPath;
	CFileFind finder;

	m_tree_FindResult.DeleteAllItems();
	m_static_ShowPic.SetPicPath(NULL);
	m_static_ShowPic.Invalidate();

	int i, iCount = m_list_FindDupFileList.GetItemCount();
	for (i=0 ; i<iCount ; i++) {
		if (!IsCanThread())
			goto Lable_ExitFindDupFileThread;

		sPath = m_list_FindDupFileList.GetItemText(i, 0);
		_FindAllFileAndAddToArray(&aDupFile, sPath);
	}

	if (!IsCanThread())
		goto Lable_ExitFindDupFileThread;
	qsort(aDupFile.GetData(), aDupFile.GetCount(), sizeof(CFindDupFileProc *), FileCmpCB);

	POSITION pos;
	HTREEITEM hTreeItem;
	HTREEITEM hTreeChildItem;
	CListFindDupFileProc *pListSameFile;
	iCount = aDupFile.GetCount();
	for (i=0 ; i<iCount ; i++) {
		if (!IsCanThread())
			goto Lable_ExitFindDupFileThread;

		pImg = aDupFile[i];
		pListSameFile = pImg->m_pListSameFile;
		if (pListSameFile) {
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

Lable_ExitFindDupFileThread:
	iCount = aDupFile.GetCount();
	for (i=0 ; i<iCount ; i++)
		delete aDupFile[i];
	aDupFile.RemoveAll();

	if (IsCanThread()) {
		m_tree_FindResult.Invalidate();
		GetDlgItem(IDC_FIND_BTN_STARTFIND)->EnableWindow(TRUE);
	}
	return 0;
}

BOOL CFindDupFileDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_cIni.SetPathName(theApp.m_sAppDir + _T("FindDupFile.ini"));
	if (!::g_pFindConf) {
		::g_pFindConf = new CFindDFConf;
		::g_pFindConf->Init(&m_cIni);
	}

	GetDlgItem(IDC_FIND_BTN_STARTFIND)->SetWindowText(CResString(IDS_FIND_BTN_STARTFIND));
	GetDlgItem(IDC_FIND_BTN_SELECTDUP)->SetWindowText(CResString(IDS_FIND_BTN_SELECTDUP));
	GetDlgItem(IDC_FIND_BTN_SELECTNONE)->SetWindowText(CResString(IDS_FIND_BTN_SELECTNONE));
	GetDlgItem(IDC_FIND_BTN_DELETEDUP)->SetWindowText(CResString(IDS_FIND_BTN_DELETEDUP));

	m_list_FindDupFileList.Init();
	m_tree_FindResult.ModifyStyle(0, TVS_CHECKBOXES);	// �����b VC ���귽������|�� bug

	EnableToolTips(::g_pFindConf->m_bEnableToolTip);

	m_bInit = true;
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX �ݩʭ����Ǧ^ FALSE
}

void CFindDupFileDlg::OnDestroy()
{
	SetCanThread(false);
	WaitForThread(3000);

	CDialog::OnDestroy();

	if (::g_pFindConf) {
		delete ::g_pFindConf;
		::g_pFindConf = NULL;
	}
}

void CFindDupFileDlg::DoSize()
{
	if (!m_bInit)
		return;

//	int iMarginLeft = 15;
//	int iMarginTop = 15;
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

	KDMoveDlgItem(GetDlgItem(IDC_FIND_STATIC_SHOWPIC), GetDlgItem(IDC_FIND_BTN_SELECTDUP),
		KDMOVEDLGITEM_WAY_BOTTOM | KDMOVEDLGITEM_WAY_F_OUTSIDE, 5);
	KDMoveDlgItem(GetDlgItem(IDC_FIND_STATIC_SHOWPIC), this,
		KDMOVEDLGITEM_WAY_RIGHT | KDMOVEDLGITEM_WAY_F_INSIDE, iMarginRight);
	KDMoveDlgItem(GetDlgItem(IDC_FIND_STATIC_SHOWPIC), this,
		KDMOVEDLGITEM_WAY_BOTTOM | KDMOVEDLGITEM_WAY_F_INSIDE, iMarginBottom, true);
	GetDlgItem(IDC_FIND_STATIC_SHOWPIC)->GetWindowRect(rcItem);
	rcItem.left = rcItem.right - rcItem.Height();
	ScreenToClient(rcItem);
	GetDlgItem(IDC_FIND_STATIC_SHOWPIC)->MoveWindow(rcItem, FALSE);

	KDMoveDlgItem(GetDlgItem(IDC_FIND_TREE_FINDRESULT), GetDlgItem(IDC_FIND_BTN_SELECTDUP),
		KDMOVEDLGITEM_WAY_BOTTOM | KDMOVEDLGITEM_WAY_F_OUTSIDE, 5);
	KDMoveDlgItem(GetDlgItem(IDC_FIND_TREE_FINDRESULT), this,
		KDMOVEDLGITEM_WAY_BOTTOM | KDMOVEDLGITEM_WAY_F_INSIDE, iMarginBottom, true);
	KDMoveDlgItem(GetDlgItem(IDC_FIND_TREE_FINDRESULT), GetDlgItem(IDC_FIND_STATIC_SHOWPIC),
		KDMOVEDLGITEM_WAY_LEFT | KDMOVEDLGITEM_WAY_F_OUTSIDE, 5, true);

	Invalidate();
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

//void CFindDupFileDlg::SetFindResultTreeExpand(bool bExpand/* = true*/)
//{
//	HTREEITEM hTreeItem = m_tree_FindResult.GetRootItem();
//
//	while (hTreeItem) {
//		m_tree_FindResult.SetItemState(hTreeItem, bExpand ? TVIS_EXPANDED : 0, TVIS_EXPANDED);
//
//		hTreeItem = m_tree_FindResult.GetNextSiblingItem(hTreeItem);
//	}
//	m_tree_FindResult.Invalidate();
//	m_tree_FindResult.ScrollWindow(0, 0);
//}

void CFindDupFileDlg::_FindAllFileAndAddToArray(void *pArray, LPCTSTR sPath)
{
	if (!pArray || !sPath)
		return;

	CFindDupFileProc *pImg;
	CArray<CFindDupFileProc *, CFindDupFileProc *> *paFile = (CArray<CFindDupFileProc *, CFindDupFileProc *> *)pArray;

	if (PathIsDirectory(sPath)) {
		CFileFind finder;
		CString strWildcard(sPath);
		strWildcard += _T("\\*.*");

		BOOL bWorking = finder.FindFile(strWildcard);
		while (bWorking && IsCanThread()) {
			bWorking = finder.FindNextFile();

			if (finder.IsDots())
				continue;

			if (finder.IsDirectory())
				_FindAllFileAndAddToArray(pArray, finder.GetFilePath());

			pImg = new CFindDupFileProc(finder.GetFilePath());
			paFile->Add(pImg);
		}
	} else if (PathFileExists(sPath)) {
		pImg = new CFindDupFileProc(sPath);
		paFile->Add(pImg);
	}
}

BEGIN_MESSAGE_MAP(CFindDupFileDlg, CDialog)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_FIND_BTN_ENABLETOOLTIP, OnBnClickedFindBtnEnabletooltip)
	ON_BN_CLICKED(IDC_FIND_BTN_STARTFIND, OnBnClickedFindBtnStartfind)
	ON_NOTIFY(TVN_SELCHANGED, IDC_FIND_TREE_FINDRESULT, OnTvnSelchangedFindTreeFindresult)
	ON_BN_CLICKED(IDC_FIND_BTN_SELECTDUP, OnBnClickedFindBtnSelectdup)
	ON_BN_CLICKED(IDC_FIND_BTN_SELECTNONE, OnBnClickedFindBtnSelectnone)
	ON_BN_CLICKED(IDC_FIND_BTN_DELETEDUP, OnBnClickedFindBtnDeletedup)
END_MESSAGE_MAP()

void CFindDupFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FIND_LIST_FINDLIST, m_list_FindDupFileList);
	DDX_Control(pDX, IDC_FIND_TREE_FINDRESULT, m_tree_FindResult);
	DDX_Control(pDX, IDC_FIND_STATIC_SHOWPIC, m_static_ShowPic);
}

void CFindDupFileDlg::OnOK()
{
//	CDialog::OnOK();
}

void CFindDupFileDlg::OnCancel()
{
	theApp.GetMainWnd()->DestroyWindow();

//	CDialog::OnCancel();
}

void CFindDupFileDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	DoSize();
}

void CFindDupFileDlg::OnBnClickedFindBtnEnabletooltip()
{
	::g_pFindConf->m_bEnableToolTip = !::g_pFindConf->m_bEnableToolTip;

	EnableToolTips(::g_pFindConf->m_bEnableToolTip);
}

void CFindDupFileDlg::OnBnClickedFindBtnStartfind()
{
	GetDlgItem(IDC_FIND_BTN_STARTFIND)->EnableWindow(FALSE);
	CreateThread(THREAD_PRIORITY_LOWEST);
	SetFocus();
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
	CString sPath;
	HTREEITEM hTreeItem = m_tree_FindResult.GetRootItem();
	HTREEITEM hTreeChildItem;

	if (!hTreeItem) {
		MessageBox(CResString(IDS_FIND_MSG_DOFINDFIRST), NULL, MB_OK | MB_ICONERROR);
		return;
	}

	if (IDOK != MessageBox(CResString(IDS_FIND_MSG_DELETEDUP), NULL, MB_OKCANCEL | MB_ICONQUESTION))
		return;

	GetDlgItem(IDC_FIND_BTN_DELETEDUP)->EnableWindow(FALSE);

	while (hTreeItem) {
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
	GetDlgItem(IDC_FIND_BTN_DELETEDUP)->EnableWindow();
}

void CFindDupFileDlg::OnTvnSelchangedFindTreeFindresult(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	m_static_ShowPic.SetPicPath(m_tree_FindResult.GetItemText(pNMTreeView->itemNew.hItem));

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
						m_list_FindDupFileList.InsertItem(m_list_FindDupFileList.GetItemCount(), sPath);
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
						if (!sPath.IsEmpty()) {
							if (!PathIsDirectory(sPath)) {
								PathRemoveFileSpec(sPath.GetBuffer());
								sPath.ReleaseBuffer();
							}
							ShellExecute(NULL, _T("open"), sPath, NULL, NULL, SW_SHOW);
						}
					}
				}
				break;
			}
		}
		break;
	}

	return __super::DefWindowProc(message, wParam, lParam);
}
