#include "StdAfx.h"
#include "WallEnablePicList.h"
#include "WallChangerDlg.h"

#include "WallThreadFindPic.h"

CWallThreadFindPic::CWallThreadFindPic()
{
	m_hHaveDirItem = CreateEvent(NULL, TRUE, FALSE, NULL);
	CreateThread(THREAD_PRIORITY_IDLE);
}

CWallThreadFindPic::~CWallThreadFindPic()
{
	SetCanThread(false);
	SetEvent(m_hHaveDirItem);
	if (WAIT_TIMEOUT == WaitForThread(5000)) {
#ifdef DEBUG
		MessageBox(GetFocus(), _T("FindPic Thread is running!!"), _T("ERROR"), MB_OK | MB_ICONERROR);
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
		if (::g_pWallEnablePicList->GetCount()) {
			PostMessage(WMU_FIRST_FIND_PIC, 0, 0);
			break;
		}
	}

	// Stage 2: After Stage 1
	while (IsCanThread()) {
		_ThreadProcStage();
	}
	return 0;
}

void CWallThreadFindPic::AddItem(CWallDirListItem *pDirItem)
{
	if (m_muxThread.Lock()) {
		if (!pDirItem) {
			m_muxThread.Unlock();
			return;
		}

		m_lDirItem.AddTail(pDirItem);
		SetEvent(m_hHaveDirItem);

		m_muxThread.Unlock();
	}
}

void CWallThreadFindPic::TestOfflineDirItem()
{
	if (m_muxThread.Lock()) {
		if (m_lDirItemOffline.GetCount()) {
			m_lDirItem.AddTail(&m_lDirItemOffline);
			m_lDirItemOffline.RemoveAll();
			SetEvent(m_hHaveDirItem);
		}

		m_muxThread.Unlock();
	}
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
		} else if (__bMatchSupport(finder.GetFileName()))
			pslPicPath->AddTail(finder.GetFilePath());
	}
	return pslPicPath;
}

bool CWallThreadFindPic::__bMatchSupport(LPCTSTR sPat) {
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

void CWallThreadFindPic::_ThreadProcStage()
{
	WaitForSingleObject(m_hHaveDirItem, INFINITE);
	if (m_lDirItem.IsEmpty()) {
		ResetEvent(m_hHaveDirItem);
		return;
	}

	CWallDirListItem *pDirItem = m_lDirItem.RemoveHead();
	CString sDirPath = pDirItem->GetItemDirPath();
	if (!PathFileExists(sDirPath) && !pDirItem->IsDirOnFixDrive()) {
		m_lDirItemOffline.AddTail(pDirItem);
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

	pDirItem->SetItemFileFindNum(saPicPath.GetCount());
	pDirItem->SetItemPicPathArray(saPicPath);
	pDirItem->SetIniModify();
	pDirItem->SetOnFindPic(false);
	if (pDirItem->IsItemEnable())
		::g_pWallEnablePicList->AddEnableItem(pDirItem);
	pDirItem->Invalidate();
}
