#pragma once
#include "StdAfx.h"
#include "WallThread.h"
#include "WallDirListItem.h"

class CWallThreadFindPic :
	public CWallThread
{
public:
	CWallThreadFindPic();
	virtual ~CWallThreadFindPic();
	virtual DWORD ThreadProc();

	void AddItem(CWallDirListItem *pDirItem);
	void RemoveItem(CWallDirListItem *pDirItem);
	void TestOfflineDirItem();

	bool IsMatchSupport(LPCTSTR sPat);

private:
	CMutex m_muxThread;
	HANDLE m_hHaveDirItem;
	CWallDirListItem *m_pNowItem;
	CList<CWallDirListItem*, CWallDirListItem*> m_lDirItem;
	CList<CWallDirListItem*, CWallDirListItem*> m_lDirItemOffline;

	void _ThreadProcStage();
	int FindPicPath();
	CStringList *__pFindPicPathDynamic(LPCTSTR sPath, bool bRecursive = false);
};

extern CWallThreadFindPic *g_pWallThreadFindPic;
