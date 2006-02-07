#include "StdAfx.h"
#include "WallThread.h"

CWallThread::CWallThread()
	:	m_hMsgWnd(NULL)
{
}

CWallThread::~CWallThread()
{
}

void CWallThread::SetMsgWnd(HWND hMsgWnd)
{
	if (m_muxThread.Lock()) {
		m_hMsgWnd = hMsgWnd;
		m_muxThread.Unlock();
	}
}

BOOL CWallThread::PostMessage(UINT Msg, WPARAM wParam, LPARAM lParam)
{
	BOOL bRes;
	if (m_muxThread.Lock()) {
		bRes = ::PostMessage(m_hMsgWnd, Msg, wParam, lParam);
		m_muxThread.Unlock();
	}
	return bRes;
}
