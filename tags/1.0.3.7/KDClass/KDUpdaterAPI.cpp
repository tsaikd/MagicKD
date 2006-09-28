#include "StdAfx.h"
#include "Ini.h"
#include "KDUpdaterAPI.h"

UINT WMU_KDUPDATER_REQ					= RegisterWindowMessage(_T("Req") _T("KDUpdater"));
UINT WMU_KDUPDATER_RES					= RegisterWindowMessage(_T("Res") _T("KDUpdater"));

UINT WMU_KDUPDATER_REQ_NEED_UPDATE		= RegisterWindowMessage(_T("Req") _T("KDUpdater Need Update"));
UINT WMU_KDUPDATER_RES_NEED_UPDATE		= RegisterWindowMessage(_T("Res") _T("KDUpdater Need Update"));
UINT WMU_KDUPDATER_RES_NEED_REVERT		= RegisterWindowMessage(_T("Res") _T("KDUpdater Need Revert"));

UINT WMU_KDUPDATER_REQ_START_UPDATE		= RegisterWindowMessage(_T("Req") _T("KDUpdater Start Update"));
UINT WMU_KDUPDATER_REQ_CLOSE_APP		= RegisterWindowMessage(_T("Req") _T("KDUpdater Close App"));

CKDUpdaterAPI::CKDUpdaterAPI()
	:	m_hUpdaterWnd(NULL), m_hParentWnd(NULL), m_hAppMainWnd(NULL), m_bOnQuit(false), m_bNeedRevert(false)
{
}

CKDUpdaterAPI::~CKDUpdaterAPI()
{
}

void CKDUpdaterAPI::SetParentWnd(HWND hParent)
{
	if (IsWindow(hParent))
		m_hParentWnd = hParent;
	else
		m_hParentWnd = NULL;
}

void CKDUpdaterAPI::SetKDUpdaterPath(LPCTSTR lpPath)
{
	if (PathFileExists(lpPath))
		m_sKDUpdaterPath = lpPath;
	else
		m_sKDUpdaterPath.Empty();
}

void CKDUpdaterAPI::SetAppMainWnd(HWND hWnd)
{
	if (IsWindow(hWnd))
		m_hAppMainWnd = hWnd;
	else
		m_hAppMainWnd = NULL;
}

void CKDUpdaterAPI::SetUpdateWorkDir(LPCTSTR lpWorkDir)
{
	if (PathIsDirectory(lpWorkDir))
		m_sUpdateWorkDir = lpWorkDir;
	else
		m_sUpdateWorkDir.Empty();
}

void CKDUpdaterAPI::SetUpdateListURL(LPCTSTR lpListURL)
{
	m_sUpdateListURL = lpListURL;
}

void CKDUpdaterAPI::SetUpdatePostCmd(LPCTSTR lpPostCmd)
{
	m_sUpdatePostCmd = lpPostCmd;
}

void CKDUpdaterAPI::AddUpdateFile(LPCTSTR lpPath, LPCTSTR lpVersion)
{
	m_saUpdateFilePath.Add(lpPath);
	m_saUpdateFileVersion.Add(lpVersion);
}

void CKDUpdaterAPI::SetOnQuit(bool bOnQuit/* = true*/)
{
	m_bOnQuit = bOnQuit;
}

bool CKDUpdaterAPI::FindKDUpdater()
{
	if (IsWindow(m_hUpdaterWnd))
		return true;
	if (!IsWindow(m_hAppMainWnd))
		return false;
	if (m_sUpdateWorkDir.IsEmpty())
		return false;
	if (m_sUpdateListURL.IsEmpty())
		return false;
	if (m_saUpdateFilePath.IsEmpty())
		return false;

	EnumWindows(QueryAppKDUpdaterNeedUpdate, (LPARAM)this);
	if (!IsWindow(m_hUpdaterWnd)) {
		if (PathFileExists(m_sKDUpdaterPath)) {
			CIni ini;
			int i, iCount;
			clock_t clockStart = clock();
			clock_t clockMaxWaitClock = clockStart + (15 * CLOCKS_PER_SEC);
			CString sBuf;
			CString sIniPath = m_sKDUpdaterPath + _T(".local.ini.tmp");

			DeleteFile(sIniPath);
			ini.SetPathName(sIniPath);
			ini.WriteUInt(_T("General"), _T("hRegWnd"), (UINT)m_hAppMainWnd);
			ini.WriteString(_T("General"), _T("sWorkDir"), m_sUpdateWorkDir);
			ini.WriteString(_T("General"), _T("sListURL"), m_sUpdateListURL);
			ini.WriteString(_T("General"), _T("sPostCmd"), m_sUpdatePostCmd);
			iCount = m_saUpdateFilePath.GetCount();
			for (i=0 ; i<iCount ; i++) {
				sBuf.Format(_T("File%d"), i+1);
				ini.WriteString(sBuf, _T("sFilePath"), m_saUpdateFilePath[i]);
				ini.WriteString(sBuf, _T("sVersion"), m_saUpdateFileVersion[i]);
			}

			sBuf.Format(_T("\"%s\""), sIniPath);
			ShellExecute(NULL, _T("open"), m_sKDUpdaterPath, sBuf, NULL, SW_SHOWMINNOACTIVE);
			while (!IsWindow(m_hUpdaterWnd) && (clock() < clockMaxWaitClock) && !m_bOnQuit) {
				Sleep(100);
				EnumWindows(QueryAppKDUpdaterNeedUpdate, (LPARAM)this);
			}
			DeleteFile(sIniPath);
		}
	}

	if (IsWindow(m_hUpdaterWnd)) {
		return true;
	} else {
		// Can't find KDUpdater, so no supporting for Updating Online
		return false;
	}
}

bool CKDUpdaterAPI::IsNeedUpdate(bool bRecheck/* = false*/)
{
	UINT uRes = 0;

	if (bRecheck)
		CloseKDUpdater();

	if (!FindKDUpdater())
		return false;

	uRes = ::SendMessage(m_hUpdaterWnd, WMU_KDUPDATER_REQ_NEED_UPDATE, 0, 0);
	m_bNeedRevert = (uRes == WMU_KDUPDATER_RES_NEED_REVERT) ? true : false;
	return (uRes == WMU_KDUPDATER_RES_NEED_UPDATE) ? true : false;
}

void CKDUpdaterAPI::DoAppUpdate(DWORD dwDelayTime/* = 5000*/, HWND hWaitWnd/* = NULL*/)
{
	if (!FindKDUpdater())
		return;

	::PostMessage(m_hUpdaterWnd, WMU_KDUPDATER_REQ_START_UPDATE, dwDelayTime, (LPARAM)hWaitWnd);
}

void CKDUpdaterAPI::CloseKDUpdater()
{
	if (!IsWindow(m_hUpdaterWnd))
		return;

	::SendMessage(m_hUpdaterWnd, WMU_KDUPDATER_REQ_CLOSE_APP, 0, 0);
	while (IsWindow(m_hUpdaterWnd) && !m_bOnQuit)
		Sleep(500);

	m_hUpdaterWnd = NULL;
}
