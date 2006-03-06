#pragma once
#include "KDThread.h"

class CWallThread :
	public CKDThread
{
public:
	CWallThread();
	virtual ~CWallThread();

	void SetMsgWnd(HWND hMsgWnd);
	BOOL PostMessage(UINT Msg, WPARAM wParam, LPARAM lParam);

private:
	HWND m_hMsgWnd;
	CMutex m_muxThread;
};
