#pragma once
#include <afxmt.h>

/*
	inherit CKDThread and Rewrite DWORD ThreadProc();
	Call CreateThread() to Run ThreadProc();

	Care: Setting a config to wait or terminate thread is better!!
	Example:
		if (WAIT_TIMEOUT == WaitForThread(10000)) {
			MessageBox(this, _T("Thread is running!!"), _T("ERROR"), MB_OK | MB_ICONERROR);
			TerminateThread(0);
		}
*/

class CKDThread
{
public:
	CKDThread();
	virtual ~CKDThread();
	virtual DWORD ThreadProc();

	void CreateThread(int nPriority = THREAD_PRIORITY_NORMAL);
	void SetCanThread(bool bCanThread = true);
	bool SetThreadPriority(int nPriority = THREAD_PRIORITY_NORMAL);
	bool IsCanThread();
	bool IsThreadRunning();
	DWORD WaitForThread(DWORD dwMilliseconds);
	bool TerminateThread(DWORD dwExutCode = 0);

protected:
	HANDLE		m_hThread;
	DWORD		m_dwThreadId;
	CSemaphore	m_semThread;
	CSemaphore	m_semCanThread;
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
