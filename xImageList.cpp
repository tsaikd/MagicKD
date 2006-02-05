#include "StdAfx.h"
#include "Others.h"

#include "xImageList.h"

CxImageList::CxImageList()
	: m_cpScreenSize(1024, 768), m_hUpdateWnd(NULL), m_hEventHavePicQueue(NULL)
{
	CreateThread(THREAD_PRIORITY_LOWEST);
}

CxImageList::~CxImageList()
{
	RemoveAll();

	SetCacheNumWnd(NULL);
	SetCanThread(false);
	SetEvent(m_hEventHavePicQueue);
	if (WAIT_TIMEOUT == WaitForThread(10000)) {
		MessageBox(NULL, _T("xImageList Thread is running!!"), _T("ERROR"), MB_OK | MB_ICONERROR);
		TerminateThread(0);
	}

	CloseHandle(m_hEventHavePicQueue);
}

DWORD CxImageList::ThreadProc()
{
	m_hEventHavePicQueue = CreateEvent(NULL, TRUE, FALSE, NULL);
	CString sImageWaitForAdd, sBufPicPath;
	CxImage xImage;
	while (IsCanThread()) {
		WaitForSingleObject(m_hEventHavePicQueue, INFINITE);
		if (!IsCanThread())
			return 0;
		if (!m_slImageWaitForAdd.GetCount()) {
			ResetEvent(m_hEventHavePicQueue);
			continue;
		}

		sImageWaitForAdd = m_slImageWaitForAdd.RemoveHead();
		if ((sImageWaitForAdd.GetLength()) && (xImage.Load(sImageWaitForAdd))) {
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

void CxImageList::AddHead(const CString &newElement)
{
	m_slImageWaitForAdd.AddHead(newElement);
	SetEvent(m_hEventHavePicQueue);
}

void CxImageList::AddTail(const CString &newElement)
{
	m_slImageWaitForAdd.AddTail(newElement);
	SetEvent(m_hEventHavePicQueue);
}

POSITION CxImageList::FindPath(LPCTSTR searchValue, POSITION startAfter/* = 0*/) const
{
	return m_slImagePath.Find(searchValue, startAfter);
}

const CString & CxImageList::GetHeadPath()
{
	return m_slImagePath.GetHead();
}

const CString & CxImageList::GetTailPath()
{
	return m_slImagePath.GetTail();
}

const CString & CxImageList::GetAtPath(POSITION position)
{
	return m_slImagePath.GetAt(position);
}

const CString & CxImageList::GetHeadBufPath()
{
	return m_slImageBufPath.GetHead();
}

const CString & CxImageList::GetTailBufPath()
{
	return m_slImageBufPath.GetTail();
}

POSITION CxImageList::GetHeadPathPosition()
{
	return m_slImagePath.GetHeadPosition();
}

POSITION CxImageList::GetTailPathPosition()
{
	return m_slImagePath.GetTailPosition();
}

CString & CxImageList::GetNextPath(POSITION &rPosition)
{
	return m_slImagePath.GetNext(rPosition);
}

UINT CxImageList::GetCount()
{
	return m_slImagePath.GetCount();
}

UINT CxImageList::GetAllPicCount()
{
	return GetCount() + m_slImageWaitForAdd.GetCount();
}

bool CxImageList::IsHeadImageResample()
{
	return m_blImageResample.GetHead();
}

bool CxImageList::IsTailImageResample()
{
	return m_blImageResample.GetTail();
}

// if do resample, then return true
bool CxImageList::AutoPicSize(CxImage &img)
{
	CPoint cpSize(img.GetWidth(), img.GetHeight());

	if (_AutoPicSize(cpSize, m_cpScreenSize)) {
		img.QIShrink(cpSize.x, cpSize.y);
//		img.Resample2(cpSize.x, cpSize.y, CxImage::IM_CATROM);
		return true;
	} else {
		return false;
	}
}

void CxImageList::SetCacheNumWnd(HWND hUpdateWnd)
{
	m_hUpdateWnd = hUpdateWnd;
}

CString CxImageList::RemoveHead()
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

CString CxImageList::RemoveTail()
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

void CxImageList::RemoveAll()
{
	while (GetCount())
		RemoveTail();
	m_slImageWaitForAdd.RemoveAll();
}

// if need resample, then return true, and set new size at cpSizeSrc
// else return false
bool CxImageList::_AutoPicSize(CPoint &cpSizeSrc, CPoint const &cpSizeMax)
{
	if ((cpSizeMax.x >= cpSizeSrc.x) && (cpSizeMax.y >= cpSizeSrc.y))
		return false;

	double dRatioX, dRatioY;
	dRatioX = (double)cpSizeMax.x / (double)cpSizeSrc.x;
	dRatioY = (double)cpSizeMax.y / (double)cpSizeSrc.y;

	double dRatio = (dRatioX<dRatioY) ? dRatioX : dRatioY;
	cpSizeSrc.SetPoint((int)((double)cpSizeSrc.x * dRatio), (int)((double)cpSizeSrc.y * dRatio));
	return true;
}

void CxImageList::_UpdateCacheNum()
{
	if (IsCanThread() && m_hUpdateWnd)
		::PostMessage(m_hUpdateWnd, WMU_UPDATE_CACHENUM, 0, 0);
}
