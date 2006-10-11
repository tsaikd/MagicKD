#pragma once
#include "afxinet.h"
#include "KDThread.h"
#include "KDEvent.h"

class CKDGetHttpFile : public CKDThread
{
public:
	CKDGetHttpFile();
	CKDGetHttpFile(LPCTSTR lpURL, LPCTSTR lpLocalPath);
	virtual ~CKDGetHttpFile();
	virtual void Destroy();
	virtual DWORD ThreadProc();

	bool AddFileList(LPCTSTR lpURL, LPCTSTR lpLocalPath);

	inline bool IsDownloadAllOver() { return (m_slURL.IsEmpty() && m_slLocalPath.IsEmpty()); }
	CString GetNowDLURL() { m_muxNowDLURL.Lock(); CString sRes = m_sNowDLURL; m_muxNowDLURL.Unlock(); return sRes; }
	CString GetNowDLLocalPath() { m_muxNowDLURL.Lock(); CString sRes = m_sNowDLLocalPath; m_muxNowDLURL.Unlock(); return sRes; }
	double GetPercentOfNowDL();
	INT_PTR GetDownloadCount();
	UINT GetNowDLRetryTimes();
	CString RemoveHeadDLURL();
	CString RemoveHeadLocalPath();
	bool SaveNowDLToList(bool bHead = true);
	void SetDiscardNowDL(bool bDiscard = true);

	void Pause(bool bPause = true);

protected:
	virtual void OnStartDownloadThread() {}
	virtual void OnExitDownloadThread() {}
	virtual void OnDownloadFileOver() {}
	virtual void OnDownloadFileDiscard() {}
	virtual void OnDownloadFileRetryFailed() {}
	virtual void OnWriteFileError() {}

	CInternetSession *m_pSession;
	UINT			m_uQueryRetryTimes;
	CMutex			m_muxNowDLURL;
	CKDEvent		m_eventPause;	// Set: No Pause, Reset: Pause
	CString			m_sNowDLURL;
	CString			m_sNowDLLocalPath;
	double			m_fNowDLMaxPercent;
	CStringList		m_slURL;
	CStringList		m_slLocalPath;
	ULONGLONG		m_ulNowDLMaxSize;
	ULONGLONG		m_ulNowDLSize;

private:
	enum {
		DLSTATUS_NULL,
		DLSTATUS_INIT,
		DLSTATUS_CHKDLDIR,
		DLSTATUS_SETLOCAL,
		DLSTATUS_CONNECTHTTP,
		DLSTATUS_DOWNLOAD,
		DLSTATUS_CHKNET,
		DLSTATUS_CHKUSER,
		DLSTATUS_RMLIST,
		DLSTATUS_CLOSEVAR,
		DLSTATUS_QUIT,
		DLSTATUS_DLOVER,
		DLSTATUS_LOCALFILEERROR,
		DLSTATUS_OVERRETRY,
		DLSTATUS_DISCARD
	};
	bool DownloadFileFromHttp(LPCTSTR lpURL, LPCTSTR lpLocalPath, int iQuerySize = 8192);

	bool			m_bDiscardNowDL;
};
