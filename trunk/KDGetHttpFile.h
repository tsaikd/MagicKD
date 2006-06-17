#pragma once
#include "afxinet.h"
#include "KDThread.h"

class CKDGetHttpFile : public CKDThread
{
public:
	CKDGetHttpFile();
	virtual ~CKDGetHttpFile();
	virtual DWORD ThreadProc();

	bool AddFileList(LPCTSTR lpURL, LPCTSTR lpLocalPath);
	bool AddFileListQuick(LPCTSTR lpURL, LPCTSTR lpLocalPath);

	__inline bool IsDownloadAllOver() { return (m_slURL.IsEmpty() && m_slLocalPath.IsEmpty()); }
	CString GetNowDLURL() { m_muxNowDLURL.Lock(); CString sRes = m_sNowDLURL; m_muxNowDLURL.Unlock(); return sRes; }
	CString GetNowDLLocalPath() { m_muxNowDLURL.Lock(); CString sRes = m_sNowDLLocalPath; m_muxNowDLURL.Unlock(); return sRes; }
	double GetPercentOfNowDL();
	double GetPercentOfTotalDL();
	int GetDownloadCount();
	CString RemoveHeadDLURL();
	CString RemoveHeadLocalPath();
	bool SaveNowDLToList(bool bHead = true);

protected:
	virtual void OnDownloadFileOver() {}

	CMutex m_muxNowDLURL;
	CString m_sNowDLURL;
	CString m_sNowDLLocalPath;
	CStringList m_slURL;
	CStringList m_slLocalPath;
	ULONGLONG m_ulNowDLMaxSize;
	ULONGLONG m_ulNowDLSize;
	ULONGLONG m_ulTotalDLMaxSize;
	ULONGLONG m_ulTotalDLSize;

private:
	bool DownloadFileFromHttp(LPCTSTR lpURL, LPCTSTR lpLocalPath, int iQuerySize = 8192);
};
