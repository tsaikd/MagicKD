#pragma once
#include <afxmt.h>
#include "WallDirListItem.h"

class CWallEnablePicList
{
public:
	CWallEnablePicList();
	virtual ~CWallEnablePicList();

	bool AddEnableItem(CWallDirListItem *pItem);

	CString GetRandPic();
	LPCTSTR GetNextPic();

	bool RemoveEnableItem(CWallDirListItem *pItem);
	bool RemoveFind(LPCTSTR sMatch);

	void UpdateWallChangerDlg();

	bool IsEmpty();
	ULONG GetCount();
	void ModifyCount(int iAddCount);

private:
	CMutex m_mux;
	CList<CWallDirListItem *, CWallDirListItem *> m_lEnableItem;
	ULONG m_uCount;
	POSITION m_posNowList;
	int m_iNowArray;

	LPCTSTR _GetNextPic(UINT uJump = 1);
};

extern CWallEnablePicList *g_pWallEnablePicList;
