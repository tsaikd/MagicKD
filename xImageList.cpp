#include "StdAfx.h"
#include "Others.h"

#include "xImageList.h"

CxImageList::CxImageList()
	: m_cpScreenSize(1024, 768), m_bCanThread(true), m_semWaitForAdd(0), m_pCacheNumWnd(NULL)
{
	CreateThread();
	SetThreadPriority(m_hThread, THREAD_PRIORITY_LOWEST);
}

CxImageList::~CxImageList()
{
	m_pCacheNumWnd = NULL;
	m_bCanThread = false;
	m_semWaitForAdd.Unlock();
	if (WAIT_TIMEOUT == WaitForSingleObject(m_hThread, 10000)) {
		MessageBox(NULL, _T("xImageList Thread is running!!"), _T("ERROR"), MB_OK | MB_ICONERROR);
		TerminateThread(m_hThread, 0);
	}

	RemoveAll();
}

void CxImageList::AddHead(const CString &newElement)
{
	m_slImageWaitForAdd.AddHead(newElement);
	m_semWaitForAdd.Unlock();
}

void CxImageList::AddTail(const CString &newElement)
{
	m_slImageWaitForAdd.AddTail(newElement);
	m_semWaitForAdd.Unlock();
}

POSITION CxImageList::FindPath(LPCTSTR searchValue, POSITION startAfter/* = 0*/)
{
	return CStringList::Find(searchValue, startAfter);
}

const CString & CxImageList::GetHeadPath()
{
	return CStringList::GetHead();
}

const CString & CxImageList::GetTailPath()
{
	return CStringList::GetTail();
}

const CString & CxImageList::GetAtPath(POSITION position)
{
	return CStringList::GetAt(position);
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
	return CStringList::GetHeadPosition();
}

POSITION CxImageList::GetTailPathPosition()
{
	return CStringList::GetTailPosition();
}

CString & CxImageList::GetNextPath(POSITION &rPosition)
{
	return CStringList::GetNext(rPosition);
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
	if ((cpSize.x > m_cpScreenSize.x) || (cpSize.y > m_cpScreenSize.y)) {
		_AutoPicSize(cpSize, m_cpScreenSize);

		if (!m_bCanThread)
			return false;

		img.Resample2(cpSize.x, cpSize.y, CxImage::IM_CATROM);

		if (!m_bCanThread)
			return false;

		return true;
	}
	return false;
}

void CxImageList::SetCacheNumWnd(CWnd *pCacheNumWnd)
{
	m_pCacheNumWnd = pCacheNumWnd;
}

CString CxImageList::RemoveHead()
{
	bool bResample = m_blImageResample.RemoveHead();
	CString sBufPath = m_slImageBufPath.RemoveHead();

	if (bResample && PathFileExists(sBufPath))
		DeleteFile(sBufPath);

	sBufPath = CStringList::RemoveHead();

	_SetCacheNumWnd();

	return sBufPath;
}

CString CxImageList::RemoveTail()
{
	bool bResample = m_blImageResample.RemoveTail();
	CString sBufPath = m_slImageBufPath.RemoveTail();

	if (bResample && PathFileExists(sBufPath))
		DeleteFile(sBufPath);

	sBufPath = CStringList::RemoveTail();

	_SetCacheNumWnd();

	return sBufPath;
}

void CxImageList::RemoveAll()
{
	while (GetCount())
		RemoveTail();

	return CStringList::RemoveAll();
}

CPoint CxImageList::_AutoPicSize(CPoint &cpSizeSrc, CPoint const &cpSizeMax)
{
	if ((cpSizeMax.x >= cpSizeSrc.x) && (cpSizeMax.y >= cpSizeSrc.y))
		return cpSizeSrc;

	double dRatioX, dRatioY;
	dRatioX = (double)cpSizeMax.x / (double)cpSizeSrc.x;
	dRatioY = (double)cpSizeMax.y / (double)cpSizeSrc.y;

	double dRatio = (dRatioX<dRatioY) ? dRatioX : dRatioY;
	cpSizeSrc.SetPoint((int)((double)cpSizeSrc.x * dRatio), (int)((double)cpSizeSrc.y * dRatio));
	return cpSizeSrc;
}

void CxImageList::_SetCacheNumWnd()
{
	if (m_bCanThread && m_pCacheNumWnd) {
		CString sNum;
		sNum.Format(_T("%d"), GetCount());
		m_pCacheNumWnd->SetWindowText(sNum);
	}
}

DWORD CxImageList::ThreadProc()
{
	if (!m_bCanThread)
		return 0;

	CString sImageWaitForAdd, sBufPicPath;
	CxImage xImage;
	while (m_bCanThread) {
		if (!m_slImageWaitForAdd.GetCount()) {
			m_semWaitForAdd.Lock();
			continue;
		}
		if (!m_bCanThread)
			return 0;

		sImageWaitForAdd = m_slImageWaitForAdd.RemoveHead();
		if (sImageWaitForAdd.GetLength()) {
			if (!m_bCanThread)
				return 0;

			if (xImage.Load(sImageWaitForAdd)) {
				if (!m_bCanThread)
					return 0;

				bool bAutoPicSize = AutoPicSize(xImage);
				if (bAutoPicSize) {
					sBufPicPath.Format(_T("%s.jpg"), GetTempFilePath());
					xImage.Save(sBufPicPath, CXIMAGE_FORMAT_JPG);
				} else {
					sBufPicPath = _T("");
				}

				if (!m_bCanThread)
					return 0;

				m_blImageResample.AddTail(bAutoPicSize);
				m_slImageBufPath.AddTail(sBufPicPath);
				CStringList::AddTail(sImageWaitForAdd);

				_SetCacheNumWnd();
			}
		}
	}

	return 0;
}
