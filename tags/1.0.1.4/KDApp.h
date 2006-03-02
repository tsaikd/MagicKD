/*

if you want to use UpdateAppOnLine, please
	#define KDAPP_ENABLE_UPDATEAPPONLINE
at stdafx.h

*/
#pragma once

class CKDApp
{
public:
	CKDApp();
	virtual ~CKDApp();

#ifdef KDAPP_ENABLE_UPDATEAPPONLINE
	CString GetUpdateAppOnLineVer(LPCTSTR lpQueryUrl, LPCTSTR lpQueryKeyword,
		LONGLONG i64QueryOffset, short unsigned int iQueryVerSize);
	bool SetUpdateApp(LPCTSTR lpOldAppPath, LPCTSTR lpNewAppPath);
	bool SetUpdateApp(CStringArray &saOldAppPath, CStringArray &saNewAppPath);
#endif

	_inline void SetRestart(bool bRestart = true) { m_bRestart = bRestart; }
	_inline LPCTSTR GetAppName() { return m_lpAppName; }
	_inline LPCTSTR GetAppPath() { return m_lpAppPath; }
	_inline LPCTSTR GetAppDir() { return m_lpAppDir; }
	_inline LPCTSTR GetAppConfDir() { return m_lpAppConfDir; }
	_inline LPCTSTR GetAppLangDir() { return m_lpAppLangDir; }
	_inline LPCTSTR GetAppFileVer() { return m_lpAppFileVer; }
	_inline LPCTSTR GetAppProductVer() { return m_lpAppProductVer; }

private:
	bool m_bRestart;
	LPCTSTR m_lpAppName;
	LPCTSTR m_lpAppPath;
	LPCTSTR m_lpAppDir;
	LPCTSTR m_lpAppConfDir;
	LPCTSTR m_lpAppLangDir;
	LPCTSTR m_lpAppFileVer;
	LPCTSTR m_lpAppProductVer;

#ifdef KDAPP_ENABLE_UPDATEAPPONLINE
	bool m_bUpdateApp;
	LPCTSTR m_lpTmpBatchPath;
	CStringArray m_saOldAppPath;
	CStringArray m_saNewAppPath;
#endif
};
