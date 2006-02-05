#pragma once
#include "xImage.h"
#include "KDThread.h"

enum {
	WMU_UPDATE_CACHENUM = (WM_USER + 1),
	WMU_NULL
};

class CxImageList :
	public CKDThread
{
public:
	CxImageList();
	virtual ~CxImageList();
	virtual DWORD ThreadProc();

	void AddHead(const CString &newElement);
	void AddTail(const CString &newElement);

	POSITION FindPath(LPCTSTR searchValue, POSITION startAfter = 0) const;

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

	UINT GetCount();
	UINT GetAllPicCount();

	bool IsHeadImageResample();
	bool IsTailImageResample();

	bool AutoPicSize(CxImage &img);

	void SetCacheNumWnd(HWND hUpdateWnd);

	CString RemoveHead();
	CString RemoveTail();

	void RemoveAll();

private:
	bool _AutoPicSize(CPoint &cpSizeSrc, CPoint const &cpSizeMax);
	void _UpdateCacheNum();

	HANDLE m_hEventHavePicQueue;
	HWND m_hUpdateWnd;
	CPoint m_cpScreenSize;
	CList<bool, bool> m_blImageResample;
	CStringList m_slImagePath;
	CStringList m_slImageBufPath;
	CStringList m_slImageWaitForAdd;
};
