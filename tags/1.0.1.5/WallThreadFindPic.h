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
	void TestOfflineDirItem();

private:
	CMutex m_muxThread;
	HANDLE m_hHaveDirItem;
	CList<CWallDirListItem*, CWallDirListItem*> m_lDirItem;
	CList<CWallDirListItem*, CWallDirListItem*> m_lDirItemOffline;

	void _ThreadProcStage();
	int FindPicPath();
	CStringList *__pFindPicPathDynamic(LPCTSTR sPath, bool bRecursive = false);
	bool __bMatchSupport(LPCTSTR sPat);
};

extern CWallThreadFindPic *g_pWallThreadFindPic;
