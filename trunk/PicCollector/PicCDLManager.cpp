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
	SetCanThread(false);
	if (WAIT_TIMEOUT == WaitForThread(10000)) {
#ifdef DEBUG
		AfxMessageBox(_T("CPicCDLManager Thread is running!!"), MB_OK | MB_ICONERROR);
#endif //DEBUG
		SaveNowDLToList();
		TerminateThread(0);
	}

//	m_pFeed->ExecSQL(_T("DELETE FROM PicUnDownload"));
	m_pFeed->ExecSQL(_T("PRAGMA synchronous = OFF"));
	CString strSQL;
	while (!IsDownloadAllOver()) {
		strSQL.Format(_T("INSERT INTO PicUnDownload (Url, Localpath) VALUES ('%s', '%s')"),
			m_pFeed->EscapeQuote(RemoveHeadDLURL()),
			m_pFeed->EscapeQuote(RemoveHeadLocalPath()));
		m_pFeed->ExecSQL(strSQL);
	}
}

void CPicCDLManager::OnDownloadFileOver()
{
	CString strSQL;
	m_muxNowDLURL.Lock();
	strSQL.Format(_T("DELETE FROM PicUnDownload WHERE Url = '%s'"), g_pPicCollectorDlg->m_Feed.EscapeQuote(m_sNowDLURL));
	m_muxNowDLURL.Unlock();
	g_pPicCollectorDlg->m_Feed.ExecSQL(strSQL);
}

void CPicCDLManager::OnDownloadFileDiscard()
{
	OnDownloadFileOver();
}

void CPicCDLManager::OnDownloadFileRetryFailed()
{
	OnDownloadFileOver();
}
