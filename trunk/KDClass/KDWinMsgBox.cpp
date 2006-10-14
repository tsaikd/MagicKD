#include "StdAfx.h"
#include "KDSysInfo.h"
#include "KDWinMsgBox.h"

CKDWinMsgBox *g_pKDWinMsgBox = NULL;

CKDWinMsgBox::CKDWinMsgBox()
{
}

CKDWinMsgBox::~CKDWinMsgBox()
{
}

void CKDWinMsgBox::Add(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType)
{
	VERIFY(m_mux.Lock());
	CKDWinMsgBoxProc msg(hWnd, lpText, lpCaption, uType);
	m_lMsg.AddTail(msg);
	m_mux.Unlock();

	if (!IsThreadRunning())
		CreateThread(THREAD_PRIORITY_LOWEST);
}

DWORD CKDWinMsgBox::ThreadProc()
{
	VERIFY(m_mux.Lock());
	INT_PTR iCount = m_lMsg.GetCount();
	m_mux.Unlock();

	while (iCount && IsCanThread()) {
		if (g_pKDSysInfo->IsFullScreen()) {
			Sleep(5000);
		} else {
			CKDWinMsgBoxProc msg = m_lMsg.RemoveHead();
			MessageBox(msg.m_hWnd, msg.m_sText, msg.m_sCaption, msg.m_uType);
		}

		VERIFY(m_mux.Lock());
		iCount = m_lMsg.GetCount();
		m_mux.Unlock();
	}
	return 0;
}
