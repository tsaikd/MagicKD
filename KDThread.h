#pragma once
#include <afxmt.h>

/*
	inherit CKDThread and Rewrite DWORD ThreadProc();
	Call CreateThread() to Run ThreadProc();

	Care: Setting a config to wait or terminate thread is better!!
	Example:
		if (WAIT_TIMEOUT == WaitForSingleObject(m_hThread, 10000)) {
			MessageBox(this, _T("Thread is running!!"), _T("ERROR"), MB_OK | MB_ICONERROR);
			TerminateThread(m_hThread, 0);
		}
*/

class CKDThread
{
public:
	CKDThread() : m_hThread(NULL), m_dwThreadId(NULL) {}
	virtual ~CKDThread() {}
	virtual DWORD ThreadProc() { return 0; }

	void CreateThread()
	{ VERIFY( m_hThread = ::CreateThread(NULL, 0, ThreadProc, (LPVOID) this, 0, &m_dwThreadId) ); }
	bool IsThreadRunning() {
		if (m_semThread.Lock(0)) {
			m_semThread.Unlock();
			return false;
		} else {
			return true;
		}
	}
protected:
	HANDLE		m_hThread;
	DWORD		m_dwThreadId;
	CSemaphore	m_semThread;
private:
	static DWORD WINAPI ThreadProc(LPVOID pParam) {
		DWORD dwRes = 0;
		CKDThread *pThis = (CKDThread *) pParam;
		if (pThis->m_semThread.Lock(0)) {
			dwRes = pThis->ThreadProc();
			pThis->m_semThread.Unlock();
		}
		return dwRes;
	}
};
