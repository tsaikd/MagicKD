#pragma once
#include "xImage.h"
#include "KDThread.h"

class CxImageList :
	private CStringList, public CKDThread
{
public:
	CxImageList();
	virtual ~CxImageList();
	virtual DWORD ThreadProc();

	void AddHead(const CString &newElement);
	void AddTail(const CString &newElement);

	POSITION FindPath(LPCTSTR searchValue, POSITION startAfter = 0);

	const CString & GetHeadPath();
	const CString & GetTailPath();
	const CString & GetAtPath(POSITION position);
	const CString & GetHeadBufPath();
	const CString & GetTailBufPath();
	CxImage * GetHeadImage();
	CxImage * GetTailImage();
	POSITION GetHeadPathPosition();
	POSITION GetTailPathPosition();
	CString & GetNextPath(POSITION &rPosition);

	using CStringList::GetCount;

	bool IsHeadImageResample();
	bool IsTailImageResample();

	bool AutoPicSize(CxImage &img);

	void SetCacheNumWnd(CWnd *pCacheNumWnd);

	CString RemoveHead();
	CString RemoveTail();

	void RemoveAll();

private:
	CPoint _AutoPicSize(CPoint &cpSizeSrc, CPoint const &cpSizeMax);
	void _SetCacheNumWnd();

	CWnd *m_pCacheNumWnd;
	bool m_bCanThread;
	CSemaphore m_semWaitForAdd;
	CPoint m_cpScreenSize;
	CList<bool, bool> m_blImageResample;
	CStringList m_slImageBufPath;
	CStringList m_slImageWaitForAdd;
};
