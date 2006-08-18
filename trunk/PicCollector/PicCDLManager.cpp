#include "StdAfx.h"
#include "PicCollectorDlg.h"

#include "PicCDLManager.h"

CPicCDLManager::CPicCDLManager()
	:	m_pFeed(NULL)
{
}

CPicCDLManager::~CPicCDLManager()
{
}

void CPicCDLManager::Init(CPicCFeed *pFeed)
{
	CStringArray saTable;
	int i;
	m_pFeed = pFeed;

	pFeed->GetTableSQL(_T("SELECT Url, Localpath FROM PicUnDownload"), saTable);
	for (i=2 ; i<saTable.GetCount() ; i+=2)
		AddFileListQuick(saTable[i], saTable[i+1]);
	CreateThread();
}

void CPicCDLManager::Destroy()
{
	if (!m_pFeed)
		return;

	SetCanThread(false);
	if (WAIT_TIMEOUT == WaitForThread(10000)) {
#ifdef DEBUG
		AfxMessageBox(_T("CPicCDLManager Thread is running!!"), MB_OK | MB_ICONERROR);
#endif //DEBUG
		SaveNowDLToList();
		TerminateThread(0);
	}

	SetDBSync(false);
	CString strSQL;
	while (!IsDownloadAllOver()) {
		strSQL.Format(_T("INSERT INTO PicUnDownload (Url, Localpath) VALUES ('%s', '%s')"),
			m_pFeed->EscapeQuote(RemoveHeadDLURL()),
			m_pFeed->EscapeQuote(RemoveHeadLocalPath()));
		m_pFeed->ExecSQL(strSQL);
	}
	SetDBSync(true);
}

bool CPicCDLManager::AddFileListQuick(LPCTSTR lpURL, LPCTSTR lpLocalPath)
{
	if (!m_pFeed)
		return false;

	CString strSQL;
	strSQL.Format(_T("INSERT INTO PicUnDownload (Url, Localpath) VALUES ('%s', '%s')"),
		m_pFeed->EscapeQuote(lpURL),
		m_pFeed->EscapeQuote(lpLocalPath));
	m_pFeed->ExecSQL(strSQL);
	return CKDGetHttpFile::AddFileListQuick(lpURL, lpLocalPath);
}

void CPicCDLManager::SetDBSync(bool bSync/* = true*/)
{
	if (!m_pFeed)
		return;

	m_pFeed->SetDBSync(bSync);
}

void CPicCDLManager::OnDownloadFileOver()
{
	CString strSQL;
	m_muxNowDLURL.Lock();
	strSQL.Format(_T("INSERT INTO PicDownloadOver (Url, Localpath) VALUES ('%s', '%s')"),
		g_pPicCollectorDlg->m_Feed.EscapeQuote(m_sNowDLURL),
		g_pPicCollectorDlg->m_Feed.EscapeQuote(m_sNowDLLocalPath));
	g_pPicCollectorDlg->m_Feed.ExecSQL(strSQL);

	strSQL.Format(_T("DELETE FROM PicUnDownload WHERE Url = '%s'"), g_pPicCollectorDlg->m_Feed.EscapeQuote(m_sNowDLURL));
	g_pPicCollectorDlg->m_Feed.ExecSQL(strSQL);
	m_muxNowDLURL.Unlock();
}

void CPicCDLManager::OnDownloadFileDiscard()
{
	CString strSQL;
	m_muxNowDLURL.Lock();
	strSQL.Format(_T("DELETE FROM PicUnDownload WHERE Url = '%s'"), g_pPicCollectorDlg->m_Feed.EscapeQuote(m_sNowDLURL));
	g_pPicCollectorDlg->m_Feed.ExecSQL(strSQL);
	m_muxNowDLURL.Unlock();
}

void CPicCDLManager::OnDownloadFileRetryFailed()
{
	CString strSQL;
	m_muxNowDLURL.Lock();
	strSQL.Format(_T("INSERT INTO PicDownloadFailed (Url, Localpath, MaxDLPercent) VALUES ('%s', '%s', '%f')"),
		g_pPicCollectorDlg->m_Feed.EscapeQuote(m_sNowDLURL),
		g_pPicCollectorDlg->m_Feed.EscapeQuote(m_sNowDLLocalPath),
		m_fNowDLMaxPercent);
	g_pPicCollectorDlg->m_Feed.ExecSQL(strSQL);

	strSQL.Format(_T("DELETE FROM PicUnDownload WHERE Url = '%s'"), g_pPicCollectorDlg->m_Feed.EscapeQuote(m_sNowDLURL));
	g_pPicCollectorDlg->m_Feed.ExecSQL(strSQL);
	m_muxNowDLURL.Unlock();
}
