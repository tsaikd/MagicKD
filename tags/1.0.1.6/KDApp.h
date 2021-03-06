/*

If You Want to Support UpdateAppOnLine, in StdAfx.h
	#define KDAPP_ENABLE_UPDATEAPPONLINE
If You Want to Support GetAppVersion, in StdAfx.h
	#define KDAPP_ENABLE_GETAPPVERSION

*/
#pragma once

class CKDApp
{
public:
	CKDApp();
	virtual ~CKDApp();

	_inline void SetRestart(bool bRestart = true) { m_bRestart = bRestart; }
	_inline LPCTSTR GetAppName() { return m_lpAppName; }
	_inline LPCTSTR GetAppPath() { return m_lpAppPath; }
	_inline LPCTSTR GetAppDir() { return m_lpAppDir; }
	_inline LPCTSTR GetAppConfDir() { return m_lpAppConfDir; }
	_inline LPCTSTR GetAppLangDir() { return m_lpAppLangDir; }

private:
	bool m_bRestart;
	LPCTSTR m_lpAppName;
	LPCTSTR m_lpAppPath;
	LPCTSTR m_lpAppDir;
	LPCTSTR m_lpAppConfDir;
	LPCTSTR m_lpAppLangDir;

#ifdef KDAPP_ENABLE_GETAPPVERSION
public:
	_inline LPCTSTR GetAppFileVer() { return m_lpAppFileVer; }
	_inline LPCTSTR GetAppProductVer() { return m_lpAppProductVer; }
private:
	LPCTSTR m_lpAppFileVer;
	LPCTSTR m_lpAppProductVer;
#endif //KDAPP_ENABLE_GETAPPVERSION

#ifdef KDAPP_ENABLE_UPDATEAPPONLINE
public:
	CString GetUpdateAppOnLineVer(LPCTSTR lpQueryUrl, LPCTSTR lpQueryKeyword,
		LONGLONG i64QueryOffset, short unsigned int iQueryVerSize);
	bool SetUpdateApp(CStringArray &saOldAppPath, CStringArray &saNewAppPath, bool bShowMsg = true);
private:
	bool m_bUpdateApp;
	bool m_bShowUpdateMsg;
	LPCTSTR m_lpTmpBatchPath;
	CStringArray m_saOldAppPath;
	CStringArray m_saNewAppPath;
#endif //KDAPP_ENABLE_UPDATEAPPONLINE
};
