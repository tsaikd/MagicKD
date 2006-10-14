#pragma once
#include <afxmt.h>

/*
	inherit CKDThread and Rewrite virtual DWORD ThreadProc();
	Call CreateThread() to Run ThreadProc();

	Care: Setting a config to wait or terminate thread is better!!
	Example:
		SetCanThread(false);
		if (WAIT_TIMEOUT == WaitForThread(10000)) {
#ifdef DEBUG
			AfxMessageBox(_T("Thread is running!!"), MB_OK | MB_ICONERROR);
#endif //DEBUG
			TerminateThread(0);
		}
*/

class CKDMThreadSpec
{
public:
	inline CKDMThreadSpec() {}
	inline ~CKDMThreadSpec() {}

	inline void Add(HANDLE hThread, DWORD dwThreadID) {
		if (!m_mux.Lock())
			return;

		VERIFY(hThread);
		m_haThread.Add(hThread);
		m_dwaThreadID.Add(dwThreadID);
		VERIFY(m_haThread.GetCount() == m_dwaThreadID.GetCount());

		m_mux.Unlock();
	}

	inline bool Remove(INT_PTR iPos) {
		if (!m_mux.Lock())
			return false;

		VERIFY(m_haThread.GetCount() == m_dwaThreadID.GetCount());
		if (iPos >= m_haThread.GetCount()) {
			m_mux.Unlock();
			return false;
		}
		m_haThread.RemoveAt(iPos);

		m_mux.Unlock();
		return true;
	}

	inline INT_PTR GetThreadCount() {
		if (!m_mux.Lock())
			return 0;

		VERIFY(m_haThread.GetCount() == m_dwaThreadID.GetCount());
		INT_PTR iRes = m_haThread.GetCount();

		m_mux.Unlock();
		return iRes;
	}

	inline const CArray<HANDLE, HANDLE> *GetThreadHandleArray() {
		VERIFY(m_haThread.GetCount() == m_dwaThreadID.GetCount());
		return &m_haThread;
	}

private:
	CArray<HANDLE, HANDLE> m_haThread;
	CArray<DWORD, DWORD> m_dwaThreadID;
	CMutex m_mux;
};

class CKDMThread
{
public:
	CKDMThread();
	virtual ~CKDMThread();
	virtual DWORD ThreadProc() { return 0; }

	void CreateThread(int nPriority = THREAD_PRIORITY_NORMAL);

	void SetCanThread(bool bCanThread = true);
	bool SetThreadPriority(int nPriority = THREAD_PRIORITY_NORMAL);
	bool IsCanThread();
	bool IsThreadRunning();
	void CheckDeadThread();
	DWORD WaitForThread(DWORD dwMilliseconds);
	void TerminateThread(DWORD dwExutCode = 0);

protected:
	CKDMThreadSpec m_ThreadCtrl;

private:
	static DWORD WINAPI ThreadProc(LPVOID pParam) {
		DWORD dwRes = 0;
		CKDMThread *pThis = (CKDMThread *) pParam;
		dwRes = pThis->ThreadProc();
		return dwRes;
	}
	inline void _SetCanThread(bool bCanThread = true);

	CMutex		m_muxThread;
	bool		m_bCanThread;
};
