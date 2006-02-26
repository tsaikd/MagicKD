#pragma once

class CKDApp
{
public:
	CKDApp();
	virtual ~CKDApp();

	_inline void SetRestart(bool bRestart = true) { m_bRestart = bRestart; }
	_inline LPCTSTR GetAppPath() { return m_lpAppPath; }
	_inline LPCTSTR GetAppDir() { return m_lpAppDir; }
	_inline LPCTSTR GetAppConfDir() { return m_lpAppConfDir; }
	_inline LPCTSTR GetAppLangDir() { return m_lpAppLangDir; }
	_inline LPCTSTR GetAppFileVer() { return m_lpAppFileVer; }
	_inline LPCTSTR GetAppProductVer() { return m_lpAppProductVer; }

private:
	bool m_bRestart;
	LPCTSTR m_lpAppPath;
	LPCTSTR m_lpAppDir;
	LPCTSTR m_lpAppConfDir;
	LPCTSTR m_lpAppLangDir;
	LPCTSTR m_lpAppFileVer;
	LPCTSTR m_lpAppProductVer;
};
