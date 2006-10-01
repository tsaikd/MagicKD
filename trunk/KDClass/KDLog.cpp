#include "StdAfx.h"
#include "KDLog.h"

CKDLog::CKDLog()
	:	m_bInit(false), m_eDefaultLV(LOGLV_ERROR)
{
}

CKDLog::~CKDLog()
{
	Destroy();
}

bool CKDLog::Init(LPCTSTR lpLogFilePath, eLogLV eDefaultLV/* = LOGLV_INFO*/)
{
	if (m_bInit)
		Destroy();

	BOOL bRes = m_LogFile.Open(lpLogFilePath, CFile::modeCreate | CFile::modeNoTruncate | 
		CFile::modeReadWrite | CFile::shareDenyWrite);

	if (bRes != FALSE) {
		m_bInit = true;
		m_eDefaultLV = eDefaultLV;
		m_LogFile.SeekToEnd();

		return true;
	} else {
		Destroy();

		return false;
	}
}

void CKDLog::Destroy()
{
	m_LogFile.Close();
	m_bInit = false;
	m_eDefaultLV = LOGLV_ERROR;
}

void CKDLog::Log(LPCTSTR lpMsg, eLogLV eLV/* = LOGLV_DEBUG*/)
{
	if (!m_bInit)
		return;
	if (_tcslen(lpMsg) < 1)
		return;
	if (eLV < m_eDefaultLV)
		return;

	CTime time(time(NULL));
	CString sLogMsg = time.Format(_T("%Y/%m/%d %H:%M:%S "));

	if (eLV >= LOGLV_ERROR)
		sLogMsg.Append(_T("[ERROR] "));
	else if (eLV >= LOGLV_WARN)
		sLogMsg.Append(_T("[WARN] "));
	else if (eLV >= LOGLV_INFO)
		sLogMsg.Append(_T("[INFO] "));
	else
		sLogMsg.Append(_T("[DEBUG] "));

	sLogMsg.AppendFormat(_T("%s\n"), lpMsg);
	m_LogFile.WriteString(sLogMsg);
	m_LogFile.Flush();
}
