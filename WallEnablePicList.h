#pragma once
#include <afxmt.h>
#include "WallDirListItem.h"

class CWallEnablePicList
{
public:
	CWallEnablePicList();
	virtual ~CWallEnablePicList();

	bool AddEnableItem(CStringArray *pItem);

	CString GetRandPic();
	LPCTSTR GetNextPic();

	bool RemoveEnableItem(CStringArray *pItem);
	CStringArray *RemoveFind(LPCTSTR sMatch);

	BOOL IsEmpty();
	ULONG GetCount();

private:
	CMutex m_mux;
	CList<CStringArray *, CStringArray *> m_lEnableItem;
	ULONG m_uCount;
	POSITION m_posNowList;
	int m_iNowArray;

	LPCTSTR _GetNextPic(UINT uJump = 1);
};

#ifdef _MAIN_CWallEnablePicList
#undef _MAIN_CWallEnablePicList
CWallEnablePicList *g_pWallEnablePicList = NULL;
#else
extern CWallEnablePicList *g_pWallEnablePicList;
#endif //_MAIN_CWallEnablePicList
