#include "StdAfx.h"
#include "KDThread.h"

CKDThread::CKDThread()
	: m_hThread(NULL), m_dwThreadId(NULL)
{
	SetCanThread();
}

CKDThread::~CKDThread()
{
	if (m_hThread)
		CloseHandle(m_hThread);
}

DWORD CKDThread::ThreadProc() {
	return 0;
}

//nPriority:
//	THREAD_PRIORITY_TIME_CRITICAL
//	THREAD_PRIORITY_HIGHEST
//	THREAD_PRIORITY_ABOVE_NORMAL
//	THREAD_PRIORITY_NORMAL
//	THREAD_PRIORITY_BELOW_NORMAL
//	THREAD_PRIORITY_LOWEST
//	THREAD_PRIORITY_IDLE
void CKDThread::CreateThread(int nPriority/* = THREAD_PRIORITY_NORMAL*/) {
	SetCanThread();
	VERIFY( m_hThread = ::CreateThread(NULL, 0, ThreadProc, (LPVOID) this, 0, &m_dwThreadId) );
	SetThreadPriority(nPriority);
}

void CKDThread::SetCanThread(bool bCanThread/* = true*/) {
	if (bCanThread)
		m_semCanThread.Lock(0);
	else
		m_semCanThread.Unlock();
}

//nPriority: (Can only use after CreateThread())
//	THREAD_PRIORITY_TIME_CRITICAL
//	THREAD_PRIORITY_HIGHEST
//	THREAD_PRIORITY_ABOVE_NORMAL
//	THREAD_PRIORITY_NORMAL
//	THREAD_PRIORITY_BELOW_NORMAL
//	THREAD_PRIORITY_LOWEST
//	THREAD_PRIORITY_IDLE
bool CKDThread::SetThreadPriority(int nPriority/* = THREAD_PRIORITY_NORMAL*/) {
	if (IsThreadRunning())
		return ::SetThreadPriority(m_hThread, nPriority);
	else
		return false;
}

bool CKDThread::IsCanThread() {
	if (m_semCanThread.Lock(0)) {
		m_semCanThread.Unlock();
		return false;
	} else {
		return true;
	}
}

bool CKDThread::IsThreadRunning() {
	if (m_semThread.Lock(0)) {
		m_semThread.Unlock();
		return false;
	} else {
		return true;
	}
}

//return value:
//	WAIT_OBJECT_0
//	WAIT_TIMEOUT
DWORD CKDThread::WaitForThread(DWORD dwMilliseconds) {
	return ::WaitForSingleObject(m_hThread, dwMilliseconds);
}

bool CKDThread::TerminateThread(DWORD dwExitCode/* = 0*/) {
	return ::TerminateThread(m_hThread, dwExitCode);
}
