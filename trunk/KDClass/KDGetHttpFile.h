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
	UINT GetNowDLRetryTimes();
	CString RemoveHeadDLURL();
	CString RemoveHeadLocalPath();
	bool SaveNowDLToList(bool bHead = true);
	void SetDiscardNowDL(bool bDiscard = true);

	void Pause(bool bPause = true);

protected:
	virtual void OnDownloadFileOver() {}
	virtual void OnDownloadFileDiscard() {}
	virtual void OnDownloadFileRetryFailed() {}

	UINT			m_uQueryRetryTimes;
	CMutex			m_muxNowDLURL;
	CSemaphore		m_semPause;
	CString			m_sNowDLURL;
	CString			m_sNowDLLocalPath;
	double			m_fNowDLMaxPercent;
	CStringList		m_slURL;
	CStringList		m_slLocalPath;
	ULONGLONG		m_ulNowDLMaxSize;
	ULONGLONG		m_ulNowDLSize;
	ULONGLONG		m_ulTotalDLMaxSize;
	ULONGLONG		m_ulTotalDLSize;

private:
	bool DownloadFileFromHttp(LPCTSTR lpURL, LPCTSTR lpLocalPath, int iQuerySize = 8192);

	bool			m_bDiscardNowDL;
};
