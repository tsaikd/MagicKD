#include "StdAfx.h"
#include "WallChanger.h"
#include "WallDirListCtrl.h"

CWallDirListCtrl::CWallDirListCtrl() : m_bInit(false), m_hThread(NULL), m_dwThreadId(0), m_bCanThreadRun(false),
	m_ThreadParamItem(NULL), m_hThreadInit(NULL), m_pIni(NULL)
{
}

CWallDirListCtrl::~CWallDirListCtrl()
{
}

void CWallDirListCtrl::Init(LPCTSTR sClassListName, int iClassNum)
{
	CWallListCtrl::Init();
	SetClassListName(sClassListName);
	InsertColumn(0, CResString(IDS_WALL_DIRLISTCOLUMN0), LVCFMT_LEFT, 200, 0);
	InsertColumn(1, CResString(IDS_WALL_DIRLISTCOLUMN1), LVCFMT_LEFT, 50, 1);
	m_hThreadInit = CreateMutex(NULL, FALSE, NULL);
	m_pIni = &(((CWallChanger *)GetParent())->m_cIni);

	m_bInit = true;

	CString sIniSection(_T("DirList"));
	CStringArray saDirList;
	CStringArray saPathList;
	m_pIni->GetArray(sIniSection, sClassListName, &saDirList);
	int i, iCount=saDirList.GetCount();
	for (i=0 ; i<iCount ; i++) {
		m_pIni->GetArray(_T("PathList"), saDirList[i], &saPathList);
		AddItem(iClassNum, saDirList[i], &saPathList);
	}
}

void CWallDirListCtrl::SetClassListName(LPCTSTR sClassListName)
{
	m_sClassListName = sClassListName;
}

LPCTSTR CWallDirListCtrl::GetClassListName()
{
	return m_sClassListName;
}

void CWallDirListCtrl::AddItem(int iClassNum, LPCTSTR sPath, CStringArray *saPathList/* = NULL*/)
{
	if (!m_bInit)
		return;
	CWallDirListItem *pItem = new CWallDirListItem;
	pItem->SetClassNum(iClassNum);
	pItem->SetPath(sPath);
	if (saPathList)
		pItem->SetPicPathArray(saPathList);
	else {
		m_ThreadParamItem = pItem;
		CreateThread();
	}
	InsertItem(LVIF_PARAM | LVIF_TEXT | LVIF_IMAGE, GetItemCount(), LPSTR_TEXTCALLBACK, 0, 0, I_IMAGECALLBACK, (LPARAM) pItem);
}

LPCTSTR CWallDirListCtrl::GetPicPathRand()
{
	int iCount = GetItemCount();
	if (!m_bInit || iCount<=0)
		return NULL;
	CWallDirListItem *pItem = ((CWallDirListItem *)GetItemLParam(rand()%iCount));
	if (pItem)
		return pItem->GetPicPathRand();
	else
		return NULL;
}

void CWallDirListCtrl::UpdateItemPicNum(int nItem)
{
	if (!m_bInit)
		return;
	m_ThreadParamItem = (CWallDirListItem *)GetItemLParam(nItem);
	CreateThread();
}

int CWallDirListCtrl::FindPicPath(CWallDirListItem *pItem, bool bRecursive/* = false*/) {
	CStringArray saPicPath;
	CStringList *pslPicPath = __pFindPicPath(pItem->GetPath() + _T("\\*.*"), bRecursive);
	if (!pslPicPath->IsEmpty()) {
		saPicPath.SetSize(pslPicPath->GetCount());
		int i = 0;
		POSITION pos = pslPicPath->GetHeadPosition();
		while (pos) {
			saPicPath[i++] = pslPicPath->GetAt(pos);
			printf("%s\n", pslPicPath->GetAt(pos));
			pslPicPath->GetNext(pos);
		}
		pItem->SetPicPathArray(&saPicPath);
	}
	delete pslPicPath;
	return saPicPath.GetCount();
}

void CWallDirListCtrl::DeleteAllItemsIni()
{
	int i, iCount=GetItemCount();
	CString sClassName(_T("PathList"));
	CString sKey;

	for (i=0 ; i<iCount ; i++) {
		sKey = GetItemText(i, 0);
		if (sKey.GetLength())
			m_pIni->DeleteKey(sClassName, sKey);
	}
	m_pIni->DeleteEmptySection(sClassName);
}

bool CWallDirListCtrl::__bMatchSupport(LPCTSTR sPat) {
	if (PathMatchSpec(sPat, _T("*.jpg")))
		return true;
	else if (PathMatchSpec(sPat, _T("*.jpeg")))
		return true;
	else if (PathMatchSpec(sPat, _T("*.bmp")))
		return true;
	else if (PathMatchSpec(sPat, _T("*.gif")))
		return true;
	return false;
}

CStringList *CWallDirListCtrl::__pFindPicPath(LPCTSTR sPath, bool bRecursive/* = false*/) {
	CStringList *pslPicPath = new CStringList;
	CFileFind finder;
	BOOL bWorking = finder.FindFile(sPath);
	while (bWorking) {
		bWorking = finder.FindNextFile();
		CString ttmmpp = finder.GetFilePath();
		if (finder.IsDots())
			continue;
		if (finder.IsDirectory()) {
			if (bRecursive) {
				CStringList *pslTmp = __pFindPicPath(finder.GetFilePath() + _T("\\*.*"), bRecursive);
				pslPicPath->AddTail(pslTmp);
				delete pslTmp;
			}
		} else if (__bMatchSupport(finder.GetFileName()))
			pslPicPath->AddTail(finder.GetFilePath());
	}
	return pslPicPath;
}

DWORD CWallDirListCtrl::ThreadProc()
{
	if (!m_ThreadParamItem)
		return 0;
	CWallDirListItem *pItem = NULL;
	DWORD dwWaitResult = WaitForSingleObject(m_hThreadInit, 1000);
	if (dwWaitResult == WAIT_OBJECT_0)
		pItem = m_ThreadParamItem;
	ReleaseMutex(m_hThreadInit);
	m_bCanThreadRun = true;
	if (pItem) {
		FindPicPath(pItem, true);

//		CString sBuf;
		CStringArray *pArray = pItem->GetPicPathArray();
		//int i, iCount = pArray->GetCount();
		//for ( i=0 ; i<iCount ; i++ ) {
		//	sBuf += pArray[i];
		//}
		m_pIni->WriteArray(_T("PathList"), pItem->GetPath(), pArray);
	}
	Invalidate();
	return 0;
}

BEGIN_MESSAGE_MAP(CWallDirListCtrl, CWallListCtrl)
	ON_WM_CONTEXTMENU()
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, OnLvnGetdispinfo)
	ON_NOTIFY_REFLECT(LVN_DELETEITEM, OnLvnDeleteitem)
END_MESSAGE_MAP()

void CWallDirListCtrl::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	POSITION pos = GetFirstSelectedItemPosition();
	m_mContextMenu.CreatePopupMenu();

	m_mContextMenu.AppendMenu(MF_STRING, IDS_WALLADDCLASSDIR, GetResString(IDS_WALLADDCLASSDIR));
	if (pos)
		m_mContextMenu.AppendMenu(MF_STRING, IDS_WALLDELCLASSDIR, GetResString(IDS_WALLDELCLASSDIR));

	if (m_mContextMenu.GetMenuItemCount()) {
		CPoint cpPopMenu;
		GetCursorPos(&cpPopMenu);
		m_mContextMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, cpPopMenu.x, cpPopMenu.y, this);
	}
	m_mContextMenu.DestroyMenu();
	Invalidate();
}

LRESULT CWallDirListCtrl::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_COMMAND:
		{
			UINT nID = LOWORD(wParam);
			switch (nID) {
			case IDS_WALLADDCLASSDIR:
				((CWallChanger *)GetParent())->AddClassDir();
				break;
			case IDS_WALLDELCLASSDIR:
				((CWallChanger *)GetParent())->DelClassDir();
				break;
			}
		}
		break;
	}

	return CWallListCtrl::DefWindowProc(message, wParam, lParam);
}

void CWallDirListCtrl::OnLvnGetdispinfo(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);

	if (pDispInfo->item.mask & LVIF_TEXT) {
		TCHAR sText[MAX_PATH] = {0};
		CWallDirListItem *pItem = (CWallDirListItem *) pDispInfo->item.lParam;
		switch (pDispInfo->item.iSubItem) {
			case 0:
				_stprintf(sText, "%s", pItem->GetPath());
				pDispInfo->item.pszText = sText;
				break;
			case 1:
				_stprintf(sText, "%d", pItem->GetFindNum());
				pDispInfo->item.pszText = sText;
				break;
		}
	}

	*pResult = 0;
}

void CWallDirListCtrl::OnLvnDeleteitem(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	CWallDirListItem *pItem = (CWallDirListItem *) GetItemData(pNMLV->iItem);
	if (pItem)
		delete pItem;

	*pResult = 0;
}
