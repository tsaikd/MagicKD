#pragma once

extern UINT WMU_KDUPDATER_REQ;
extern UINT WMU_KDUPDATER_RES;

extern UINT WMU_KDUPDATER_REQ_NEED_UPDATE;
extern UINT WMU_KDUPDATER_RES_NEED_UPDATE;
extern UINT WMU_KDUPDATER_RES_NONEED_UPDATE;

extern UINT WMU_KDUPDATER_REQ_START_UPDATE;
extern UINT WMU_KDUPDATER_REQ_CLOSE_APP;

class CKDUpdaterAPI
{
public:
	CKDUpdaterAPI();
	virtual ~CKDUpdaterAPI();

	void	SetKDUpdaterPath(LPCTSTR lpPath);
	void	SetAppMainWnd(HWND hWnd);
	void	SetUpdateWorkDir(LPCTSTR lpWorkDir);
	void	SetUpdateListURL(LPCTSTR lpListURL);
	void	SetUpdatePostCmd(LPCTSTR lpPostCmd);
	void	AddUpdateFile(LPCTSTR lpPath, LPCTSTR lpVersion);

	void	SetOnQuit(bool bOnQuit = true);
	bool	FindKDUpdater();
	bool	IsNeedUpdate(bool bRecheck = false);
	void	DoAppUpdate(DWORD dwDelayTime = 5000, HWND hWaitWnd = NULL);
	void	CloseKDUpdater();

protected:
	HWND	m_hUpdaterWnd;

private:
	static BOOL CALLBACK QueryAppKDUpdaterNeedUpdate(HWND hWnd, LPARAM lParam) {
		DWORD dwMsgResult;
		CKDUpdaterAPI *pThis = (CKDUpdaterAPI *)lParam;
		if (!pThis)
			return TRUE;

		LRESULT res = ::SendMessageTimeout(hWnd, WMU_KDUPDATER_REQ, (WPARAM)pThis->m_hAppMainWnd, 0,
			SMTO_BLOCK | SMTO_ABORTIFHUNG, 10000, &dwMsgResult);

		if (res == 0)
			return TRUE;

		if (dwMsgResult == WMU_KDUPDATER_RES) {
			pThis->m_hUpdaterWnd = hWnd; 
			return FALSE;
		}

		return TRUE; 
	}

	bool			m_bOnQuit;
	HWND			m_hAppMainWnd;
	CString			m_sKDUpdaterPath;
	CString			m_sUpdateWorkDir;
	CString			m_sUpdateListURL;
	CString			m_sUpdatePostCmd;
	CStringArray	m_saUpdateFilePath;
	CStringArray	m_saUpdateFileVersion;
};
