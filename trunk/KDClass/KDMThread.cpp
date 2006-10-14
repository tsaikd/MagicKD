#include "StdAfx.h"
#include "KDMThread.h"

CKDMThread::CKDMThread()
	:	m_bCanThread(true)
{
	_SetCanThread();
	m_muxThread.Unlock();
}

CKDMThread::~CKDMThread()
{
	TerminateThread(0);
}

//nPriority:
//	THREAD_PRIORITY_TIME_CRITICAL
//	THREAD_PRIORITY_HIGHEST
//	THREAD_PRIORITY_ABOVE_NORMAL
//	THREAD_PRIORITY_NORMAL
//	THREAD_PRIORITY_BELOW_NORMAL
//	THREAD_PRIORITY_LOWEST
//	THREAD_PRIORITY_IDLE
void CKDMThread::CreateThread(int nPriority/* = THREAD_PRIORITY_NORMAL*/) {
	if (m_muxThread.Lock()) {
		HANDLE hThread = INVALID_HANDLE_VALUE;
		DWORD dwThreadID = 0;

		_SetCanThread();
		hThread = ::CreateThread(NULL, 0, ThreadProc, (LPVOID) this, 0, &dwThreadID);
		::SetThreadPriority(hThread, nPriority);
		VERIFY(hThread && (hThread != INVALID_HANDLE_VALUE));

		m_ThreadCtrl.Add(hThread, dwThreadID);

		m_muxThread.Unlock();
	}
}

void CKDMThread::SetCanThread(bool bCanThread/* = true*/) {
	if (m_muxThread.Lock()) {
		_SetCanThread(bCanThread);
		m_muxThread.Unlock();
	}
}

//nPriority: (Can only use after CreateThread())
//	THREAD_PRIORITY_TIME_CRITICAL
//	THREAD_PRIORITY_HIGHEST
//	THREAD_PRIORITY_ABOVE_NORMAL
//	THREAD_PRIORITY_NORMAL
//	THREAD_PRIORITY_BELOW_NORMAL
//	THREAD_PRIORITY_LOWEST
//	THREAD_PRIORITY_IDLE
bool CKDMThread::SetThreadPriority(int nPriority/* = THREAD_PRIORITY_NORMAL*/) {
	bool bRes = true;
	if (m_muxThread.Lock()) {
		const CArray<HANDLE, HANDLE> *phlThread = m_ThreadCtrl.GetThreadHandleArray();
		INT_PTR i, iCount = phlThread->GetCount();
		for (i=0 ; i<iCount ; i++)
			bRes &= ::SetThreadPriority(phlThread->GetAt(i), nPriority) != FALSE;
		m_muxThread.Unlock();
	}
	return bRes;
}

bool CKDMThread::IsCanThread() {
	bool bRes = false;
	if (m_muxThread.Lock()) {
		bRes = m_bCanThread;
		m_muxThread.Unlock();
	}
	return bRes;
}

bool CKDMThread::IsThreadRunning()
{
	CheckDeadThread();
	if (m_muxThread.Lock()) {
		const CArray<HANDLE, HANDLE> *phlThread = m_ThreadCtrl.GetThreadHandleArray();
		INT_PTR i, iCount = m_ThreadCtrl.GetThreadCount();
		for (i=0 ; i<iCount ; i++) {
			if (WaitForSingleObject(phlThread->GetAt(i), 0) == WAIT_TIMEOUT) {
				m_muxThread.Unlock();
				return true;
			}
		}
		m_muxThread.Unlock();
	}
	return false;
}

void CKDMThread::CheckDeadThread()
{
	if (m_muxThread.Lock()) {
		const CArray<HANDLE, HANDLE> *phlThread = m_ThreadCtrl.GetThreadHandleArray();
		INT_PTR i, iCount = m_ThreadCtrl.GetThreadCount();
		for (i=0 ; i<iCount ; i++) {
			if (WaitForSingleObject(phlThread->GetAt(i), 0) != WAIT_TIMEOUT) {
				VERIFY(m_ThreadCtrl.Remove(i));
				iCount = m_ThreadCtrl.GetThreadCount();
				i--;
			}
		}
		m_muxThread.Unlock();
	}
}

//return vale:
//	WAIT_OBJECT_0
//	WAIT_TIMEOUT
DWORD CKDMThread::WaitForThread(DWORD dwMilliseconds) {
	DWORD dwRestms = dwMilliseconds;
	clock_t clockEnd = clock() + (dwMilliseconds * CLOCKS_PER_SEC / 1000);
	if (m_muxThread.Lock()) {
		const CArray<HANDLE, HANDLE> *phlThread = m_ThreadCtrl.GetThreadHandleArray();
		INT_PTR i, iCount = phlThread->GetCount();
		for (i=0 ; i<iCount ; i++) {
			if (WaitForSingleObject(phlThread->GetAt(i), dwRestms) == WAIT_TIMEOUT) {
				m_muxThread.Unlock();
				return WAIT_TIMEOUT;
			}
			dwRestms -= (clockEnd - clock()) * 1000 / CLOCKS_PER_SEC;
			if (dwRestms < 0)
				dwRestms = 0;
		}
		m_muxThread.Unlock();
	}
	return WAIT_OBJECT_0;
}

void CKDMThread::TerminateThread(DWORD dwExitCode/* = 0*/) {
	if (m_muxThread.Lock()) {
		const CArray<HANDLE, HANDLE> *phlThread = m_ThreadCtrl.GetThreadHandleArray();
		INT_PTR i, iCount = m_ThreadCtrl.GetThreadCount();
		for (i=0 ; i<iCount ; i++) {
			VERIFY(::TerminateThread(phlThread->GetAt(i), dwExitCode));
			CloseHandle(phlThread->GetAt(i));
		}

		CheckDeadThread();
		VERIFY(m_ThreadCtrl.GetThreadCount() == 0);
		m_muxThread.Unlock();
	}
}

void CKDMThread::_SetCanThread(bool bCanThread/* = true*/) {
	m_bCanThread = bCanThread;
}
