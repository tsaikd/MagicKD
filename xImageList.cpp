#include "StdAfx.h"
#include "xImageList.h"

CxImageList::CxImageList()
	: m_cpScreenSize(1024, 768)
{
}

CxImageList::~CxImageList()
{
	RemoveAll();
}

POSITION CxImageList::AddHead(const CString &newElement)
{
	CxImage *pImage = new CxImage;

	if (pImage->Load(newElement)) {
		m_blImageResample.AddHead(AutoPicSize(*pImage));
		m_imgl.AddHead(pImage);
		return CStringList::AddHead(newElement);
	} else {
		delete pImage;
		return 0;
	}
}

POSITION CxImageList::AddTail(const CString &newElement)
{
	CxImage *pImage = new CxImage;

	if (pImage->Load(newElement)) {
		m_blImageResample.AddTail(AutoPicSize(*pImage));
		m_imgl.AddTail(pImage);
		return CStringList::AddTail(newElement);
	} else {
		delete pImage;
		return 0;
	}
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

CxImage * CxImageList::GetHeadImage()
{
	return m_imgl.GetHead();
}

CxImage * CxImageList::GetTailImage()
{
	return m_imgl.GetTail();
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
		img.Resample(cpSize.x, cpSize.y);
		return true;
	} else {
		return false;
	}
}

CString CxImageList::RemoveHead()
{
	m_blImageResample.RemoveHead();
	delete m_imgl.RemoveHead();
	return CStringList::RemoveHead();
}

CString CxImageList::RemoveTail()
{
	m_blImageResample.RemoveTail();
	delete m_imgl.RemoveTail();
	return CStringList::RemoveTail();
}

void CxImageList::RemoveAll()
{
	while (m_imgl.GetCount())
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
