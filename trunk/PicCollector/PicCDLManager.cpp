#include "StdAfx.h"
#include "Language.h"
#include "KDWinMsgBox.h"
#include "PicCollectorDlg.h"

#include "PicCDLManager.h"

CPicCDLManager::CPicCDLManager()
	:	m_pFeed(NULL), m_bDelay(false)
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
		AddFileList(saTable[i], saTable[i+1]);
	CreateThread();
}

void CPicCDLManager::Destroy()
{
	if (!m_pFeed)
		return;

	CKDGetHttpFile::Destroy();

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

bool CPicCDLManager::AddFileList(LPCTSTR lpURL, LPCTSTR lpLocalPath, bool bTail/* = true*/)
{
	if (!m_pFeed)
		return false;

	CString strSQL;
	strSQL.Format(_T("INSERT INTO PicUnDownload (Url, Localpath) VALUES ('%s', '%s')"),
		m_pFeed->EscapeQuote(lpURL),
		m_pFeed->EscapeQuote(lpLocalPath));
	m_pFeed->ExecSQL(strSQL);
	return CKDGetHttpFile::AddFileList(lpURL, lpLocalPath, bTail);
}

void CPicCDLManager::SetDBSync(bool bSync/* = true*/)
{
	if (!m_pFeed)
		return;

	m_pFeed->SetDBSync(bSync);
}

void CPicCDLManager::DelayDownload()
{
	if (!m_pFeed)
		return;

	m_bDelay = true;
	SetDiscardNowDL();
}

void CPicCDLManager::OnStartDownloadThread()
{
	if (IsCanThread()) {
		g_pPicCollectorDlg->DLIconSwitch(0);
		g_pPicCollectorDlg->GetDlgItem(IDC_PICC_BTN_DELNOWDL)->EnableWindow();
		g_pPicCollectorDlg->GetDlgItem(IDC_PICC_BTN_DELAYDL)->EnableWindow();
	}
}

void CPicCDLManager::OnExitDownloadThread()
{
	if (IsCanThread()) {
		g_pPicCollectorDlg->DLIconSwitch(2);
		g_pPicCollectorDlg->GetDlgItem(IDC_PICC_BTN_DELNOWDL)->EnableWindow(FALSE);
		g_pPicCollectorDlg->GetDlgItem(IDC_PICC_BTN_DELAYDL)->EnableWindow(FALSE);
	}
}

void CPicCDLManager::OnDownloadFileOver()
{
	CString strSQL;

	VERIFY(m_muxNowDLURL.Lock());
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

	VERIFY(m_muxNowDLURL.Lock());
	DeleteFile(m_sNowDLLocalPath);
	if (m_bDelay) {
		SaveNowDLToList(false);
		m_bDelay = false;
	} else {
		strSQL.Format(_T("DELETE FROM PicUnDownload WHERE Url = '%s'"), g_pPicCollectorDlg->m_Feed.EscapeQuote(m_sNowDLURL));
		g_pPicCollectorDlg->m_Feed.ExecSQL(strSQL);
	}
	m_muxNowDLURL.Unlock();

	if (IsCanThread()) {
		g_pPicCollectorDlg->GetDlgItem(IDC_PICC_BTN_DELNOWDL)->EnableWindow(TRUE);
		g_pPicCollectorDlg->GetDlgItem(IDC_PICC_BTN_DELAYDL)->EnableWindow(TRUE);
	}
}

void CPicCDLManager::OnDownloadFileRetryFailed()
{
	CString strSQL;

	VERIFY(m_muxNowDLURL.Lock());
	DeleteFile(m_sNowDLLocalPath);
	strSQL.Format(_T("INSERT INTO PicDownloadFailed (Url, Localpath, MaxDLPercent) VALUES ('%s', '%s', '%f')"),
		g_pPicCollectorDlg->m_Feed.EscapeQuote(m_sNowDLURL),
		g_pPicCollectorDlg->m_Feed.EscapeQuote(m_sNowDLLocalPath),
		m_fNowDLMaxPercent);
	g_pPicCollectorDlg->m_Feed.ExecSQL(strSQL);

	strSQL.Format(_T("DELETE FROM PicUnDownload WHERE Url = '%s'"), g_pPicCollectorDlg->m_Feed.EscapeQuote(m_sNowDLURL));
	g_pPicCollectorDlg->m_Feed.ExecSQL(strSQL);
	m_muxNowDLURL.Unlock();
}

void CPicCDLManager::OnWriteFileError()
{
	VERIFY(m_muxNowDLURL.Lock());
	DeleteFile(m_sNowDLLocalPath);
	m_muxNowDLURL.Unlock();
	if (IsCanThread()) {
		g_pPicCollectorDlg->DLIconSwitch(1);
		Pause();
		g_pKDWinMsgBox->Add(g_pPicCollectorDlg->GetSafeHwnd(), CResString(IDS_PICC_MSG_WRITE_FILE_ERROR),
			NULL, MB_OK|MB_ICONERROR);
	}
}
