#pragma once
#include "xImage.h"

class CxImageList :
	private CStringList
{
public:
	CxImageList();
	virtual ~CxImageList();

	POSITION AddHead(const CString &newElement);
	POSITION AddTail(const CString &newElement);

	POSITION FindPath(LPCTSTR searchValue, POSITION startAfter = 0);

	const CString & GetHeadPath();
	const CString & GetTailPath();
	const CString & GetAtPath(POSITION position);
	CxImage * GetHeadImage();
	CxImage * GetTailImage();
	POSITION GetHeadPathPosition();
	POSITION GetTailPathPosition();
	CString & GetNextPath(POSITION &rPosition);

	using CStringList::GetCount;

	bool IsHeadImageResample();
	bool IsTailImageResample();

	bool AutoPicSize(CxImage &img);

	CString RemoveHead();
	CString RemoveTail();

	void RemoveAll();

private:
	CPoint _AutoPicSize(CPoint &cpSizeSrc, CPoint const &cpSizeMax);

	CList<bool, bool> m_blImageResample;
	CList<CxImage*> m_imgl;
	CPoint m_cpScreenSize;
};
