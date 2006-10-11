#include "StdAfx.h"
#include "WallEnablePicList.h"
#include "WallChangerDlg.h"

#include "WallThreadFindPic.h"

CWallThreadFindPic *g_pWallThreadFindPic = NULL;

CWallThreadFindPic::CWallThreadFindPic()
	:	m_pNowItem(NULL)
{
	m_hHaveDirItem = CreateEvent(NULL, TRUE, FALSE, NULL);
}

CWallThreadFindPic::~CWallThreadFindPic()
{
	SetCanThread(false);
	SetEvent(m_hHaveDirItem);
	if (WAIT_TIMEOUT == WaitForThread(2000)) {
#ifdef DEBUG
		MessageBox(GetForegroundWindow(), _T("WallThreadFindPic is running!!"), _T("ERROR"), MB_OK | MB_ICONERROR);
#endif //DEBUG
		TerminateThread(0);
	}

	CloseHandle(m_hHaveDirItem);
}

DWORD CWallThreadFindPic::ThreadProc()
{
	// State 1: When First Find Pic, Send Message to Main Dialog
	while (IsCanThread()) {
		_ThreadProcStage();
		if (g_pWallEnablePicList->GetCount()) {
			PostMessage(WMU_FIRST_FIND_PIC, 0, 0);
			break;
		}
	}

	// Stage 2: After Stage 1
	while (IsCanThread())
		_ThreadProcStage();

	return 0;
}

void CWallThreadFindPic::AddItem(CWallDirListItem *pDirItem)
{
	if (!pDirItem)
		return;
	m_muxThread.Lock();

	m_lDirItem.AddTail(pDirItem);
	SetEvent(m_hHaveDirItem);

	m_muxThread.Unlock();
}

void CWallThreadFindPic::RemoveItem(CWallDirListItem *pDirItem)
{
	if (!pDirItem || !pDirItem->IsOnFindPic())
		return;
	m_muxThread.Lock();

	CWallDirListItem *pItem;
	POSITION pos;

	pos = m_lDirItemOffline.GetHeadPosition();
	while (pos) {
		pItem = m_lDirItemOffline.GetAt(pos);
		if (pItem == pDirItem) {
			m_lDirItemOffline.RemoveAt(pos);
			break;
		}

		m_lDirItemOffline.GetNext(pos);
	}

	pos = m_lDirItem.GetHeadPosition();
	while (pos) {
		pItem = m_lDirItem.GetAt(pos);
		if (pItem == pDirItem) {
			m_lDirItem.RemoveAt(pos);
			break;
		}

		m_lDirItem.GetNext(pos);
	}

	m_muxThread.Unlock();
}

void CWallThreadFindPic::TestOfflineDirItem()
{
	m_muxThread.Lock();
	
	if (m_lDirItemOffline.GetCount()) {
		m_lDirItem.AddTail(&m_lDirItemOffline);
		m_lDirItemOffline.RemoveAll();
		SetEvent(m_hHaveDirItem);
	}

	m_muxThread.Unlock();
}

bool CWallThreadFindPic::IsMatchSupport(LPCTSTR sPat) {
	if (PathMatchSpec(sPat, _T("*.jpg")))
		return true;
	else if (PathMatchSpec(sPat, _T("*.jpeg")))
		return true;
	else if (PathMatchSpec(sPat, _T("*.bmp")))
		return true;
	else if (PathMatchSpec(sPat, _T("*.gif")))
		return true;
	else if (PathMatchSpec(sPat, _T("*.png")))
		return true;
	return false;
}

CStringList *CWallThreadFindPic::__pFindPicPathDynamic(LPCTSTR sPath, bool bRecursive/* = false*/) {
	CStringList *pslPicPath = new CStringList;
	CFileFind finder;
	BOOL bWorking = finder.FindFile(sPath);
	while (bWorking) {
		if (!IsCanThread()) {
			return pslPicPath;
		}
		bWorking = finder.FindNextFile();
		CString ttmmpp = finder.GetFilePath();
		if (finder.IsDots())
			continue;
		if (finder.IsDirectory()) {
			if (bRecursive) {
				CStringList *pslTmp = __pFindPicPathDynamic(finder.GetFilePath() + _T("\\*.*"), bRecursive);
				pslPicPath->AddTail(pslTmp);
				delete pslTmp;
			}
		} else if (IsMatchSupport(finder.GetFileName()))
			pslPicPath->AddTail(finder.GetFilePath());
	}
	return pslPicPath;
}

void CWallThreadFindPic::_ThreadProcStage()
{
	WaitForSingleObject(m_hHaveDirItem, INFINITE);
	if (m_lDirItem.IsEmpty()) {
		ResetEvent(m_hHaveDirItem);
		return;
	}

	m_pNowItem = m_lDirItem.RemoveHead();
	if (!m_pNowItem)
		return;

	CString sDirPath = m_pNowItem->GetItemDirPath();
	if (!PathFileExists(sDirPath) && !m_pNowItem->IsDirOnFixDrive()) {
		m_lDirItemOffline.AddTail(m_pNowItem);
		return;
	}

	CStringList *pslPicPath = __pFindPicPathDynamic(sDirPath + _T("\\*.*"), true);
	CStringArray saPicPath;
	saPicPath.SetSize(pslPicPath->GetCount());
	int i = 0;
	POSITION pos = pslPicPath->GetHeadPosition();
	while (pos) {
		saPicPath[i++] = pslPicPath->GetAt(pos);
		pslPicPath->GetNext(pos);
		if (!IsCanThread()) {
			delete pslPicPath;
			return;
		}
	}
	delete pslPicPath;

	m_muxThread.Lock();
	if (m_pNowItem) {
		m_pNowItem->SetItemFileFindNum(saPicPath.GetCount());
		m_pNowItem->SetItemPicPathArray(saPicPath);
		m_pNowItem->SetOnFindPic(false);
		if (m_pNowItem->IsItemEnable())
			g_pWallEnablePicList->AddEnableItem(m_pNowItem);
		m_pNowItem->Invalidate();
	}
	m_muxThread.Unlock();
}
