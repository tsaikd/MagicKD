#include "StdAfx.h"
#include "Others.h"
#include "WallOthers.h"
#include "WallChangerDlg.h"

#include "WallThreadImageCache.h"

CWallThreadImageCache::CWallThreadImageCache()
	: m_cpScreenSize(1024, 768), m_hMsgWnd(NULL), m_hEventHavePicQueue(NULL)
{
	m_hEventHavePicQueue = CreateEvent(NULL, TRUE, FALSE, NULL);
	CreateThread(THREAD_PRIORITY_IDLE);
}

CWallThreadImageCache::~CWallThreadImageCache()
{
	RemoveAll();

	SetMsgWnd(NULL);
	SetCanThread(false);
	SetEvent(m_hEventHavePicQueue);
	if (WAIT_TIMEOUT == WaitForThread(10000)) {
#ifdef DEBUG
		MessageBox(NULL, _T("xImageList Thread is running!!"), _T("ERROR"), MB_OK | MB_ICONERROR);
#endif //DEBUG
		TerminateThread(0);
	}

	CloseHandle(m_hEventHavePicQueue);
}

DWORD CWallThreadImageCache::ThreadProc()
{
	CString sImageWaitForAdd, sBufPicPath;
	CxImage xImage;
	while (IsCanThread()) {
		WaitForSingleObject(m_hEventHavePicQueue, 1000);
//		WaitForSingleObject(m_hEventHavePicQueue, 30000);
		if (m_slImageWaitForAdd.IsEmpty()) {
			ResetEvent(m_hEventHavePicQueue);
			continue;
		}

		sImageWaitForAdd = m_slImageWaitForAdd.RemoveHead();
		if (sImageWaitForAdd.GetLength()) {
			if (!(xImage.Load(sImageWaitForAdd))) {
#ifdef DEBUG
				MessageBox(GetFocus(), sImageWaitForAdd, _T("Image Load Error"), MB_OK | MB_ICONERROR);
#endif //DEBUG
			}
			if (!IsCanThread())
				return 0;

			bool bAutoPicSize = AutoPicSize(xImage);
			if (bAutoPicSize) {
				sBufPicPath.Format(_T("%s.jpg"), GetTempFilePath());
				xImage.Save(sBufPicPath, CXIMAGE_FORMAT_JPG);
			} else {
				sBufPicPath = _T("");
			}

			if (!IsCanThread())
				return 0;

			m_blImageResample.AddTail(bAutoPicSize);
			m_slImageBufPath.AddTail(sBufPicPath);
			m_slImagePath.AddTail(sImageWaitForAdd);

			_UpdateCacheNum();
		}
	}

	return 0;
}

void CWallThreadImageCache::AddHead(const CString &newElement)
{
	m_slImageWaitForAdd.AddHead(newElement);
	SetEvent(m_hEventHavePicQueue);
}

void CWallThreadImageCache::AddTail(const CString &newElement)
{
	m_slImageWaitForAdd.AddTail(newElement);
	SetEvent(m_hEventHavePicQueue);
}

POSITION CWallThreadImageCache::FindPath(LPCTSTR searchValue, POSITION startAfter/* = 0*/) const
{
	return m_slImagePath.Find(searchValue, startAfter);
}

const CString & CWallThreadImageCache::GetHeadPath()
{
	return m_slImagePath.GetHead();
}

const CString & CWallThreadImageCache::GetTailPath()
{
	return m_slImagePath.GetTail();
}

const CString & CWallThreadImageCache::GetAtPath(POSITION position)
{
	return m_slImagePath.GetAt(position);
}

const CString & CWallThreadImageCache::GetHeadBufPath()
{
	return m_slImageBufPath.GetHead();
}

const CString & CWallThreadImageCache::GetTailBufPath()
{
	return m_slImageBufPath.GetTail();
}

POSITION CWallThreadImageCache::GetHeadPathPosition()
{
	return m_slImagePath.GetHeadPosition();
}

POSITION CWallThreadImageCache::GetTailPathPosition()
{
	return m_slImagePath.GetTailPosition();
}

CString & CWallThreadImageCache::GetNextPath(POSITION &rPosition)
{
	return m_slImagePath.GetNext(rPosition);
}

UINT CWallThreadImageCache::GetCount()
{
	return m_slImagePath.GetCount();
}

UINT CWallThreadImageCache::GetAllPicCount()
{
	return GetCount() + m_slImageWaitForAdd.GetCount();
}

bool CWallThreadImageCache::IsHeadImageResample()
{
	return m_blImageResample.GetHead();
}

bool CWallThreadImageCache::IsTailImageResample()
{
	return m_blImageResample.GetTail();
}

CString CWallThreadImageCache::RemoveHead()
{
	if (!GetCount())
		return CString();

	bool bResample = m_blImageResample.RemoveHead();
	CString sBufPath = m_slImageBufPath.RemoveHead();

	if (bResample && PathFileExists(sBufPath))
		DeleteFile(sBufPath);

	sBufPath = m_slImagePath.RemoveHead();

	_UpdateCacheNum();

	return sBufPath;
}

CString CWallThreadImageCache::RemoveTail()
{
	if (!GetCount())
		return CString();

	bool bResample = m_blImageResample.RemoveTail();
	CString sBufPath = m_slImageBufPath.RemoveTail();

	if (bResample && PathFileExists(sBufPath))
		DeleteFile(sBufPath);

	sBufPath = m_slImagePath.RemoveTail();

	_UpdateCacheNum();

	return sBufPath;
}

void CWallThreadImageCache::RemoveAll()
{
	while (GetCount())
		RemoveTail();
	m_slImageWaitForAdd.RemoveAll();
}

void CWallThreadImageCache::_UpdateCacheNum()
{
	PostMessage(WMU_UPDATE_CACHENUM, 0, 0);
}
