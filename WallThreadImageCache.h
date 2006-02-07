#pragma once
#include "xImage.h"
#include "WallThread.h"

typedef struct _IMAGELIST {
	bool bResample;
	CString sPath;
	CString sBufPath;
} IMAGELIST,
*PIMAGELIST;

class CWallThreadImageCache :
	public CWallThread
{
public:
	CWallThreadImageCache();
	virtual ~CWallThreadImageCache();
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

	CString RemoveHead();
	CString RemoveTail();

	void RemoveAll();

private:
	void _UpdateCacheNum();

	HWND m_hMsgWnd;
	HANDLE m_hEventHavePicQueue;
	CPoint m_cpScreenSize;
	CList<bool, bool> m_blImageResample;
	CStringList m_slImagePath;
	CStringList m_slImageBufPath;
	CStringList m_slImageWaitForAdd;
};

#ifdef _MAIN_CxImageList
#undef _MAIN_CxImageList
CWallThreadImageCache *g_pWallThreadImageCache = NULL;
#else
extern CWallThreadImageCache *g_pWallThreadImageCache;
#endif //_MAIN_CxImageList
