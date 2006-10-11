#pragma once
#include <afxmt.h>

class CKDEvent : public CEvent
{
public:
	CKDEvent();
	virtual ~CKDEvent();

	inline BOOL Lock(DWORD dwTimeOut = INFINITE) { return m_bAcquired = CEvent::Lock(dwTimeOut); }
	inline BOOL Unlock() { if (m_bAcquired) m_bAcquired = !CEvent::Unlock(); return !m_bAcquired; }
	inline BOOL IsLocked() { return m_bAcquired; }

private:
	BOOL m_bAcquired;
};
