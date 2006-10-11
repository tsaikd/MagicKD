#include "StdAfx.h"
#include "Others.h"
#include "KDLog.h"

CKDLog::CKDLog()
	:	m_bInit(false), m_eDefaultLV(LOGLV_ERROR), m_bMaxNewFile(true), m_lMaxLogSize(0)
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
		CFile::modeReadWrite | CFile::shareDenyNone);

	if (bRes != FALSE) {
		m_bInit = true;
		m_eDefaultLV = eDefaultLV;
		m_sLogPath = lpLogFilePath;

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
	m_sLogPath.Empty();
}

void CKDLog::Attach()
{
	if (m_bInit)
		return;

	Init(m_sLogPath, m_eDefaultLV);
}

void CKDLog::Detach()
{
	if (!m_bInit)
		return;

	m_LogFile.Close();
	m_bInit = false;
}

void CKDLog::SetMaxLogSize(LONGLONG lSize, bool bNewFile)
{
	if (lSize > 0)
		m_lMaxLogSize = lSize;
	else
		m_lMaxLogSize = 0;

	m_bMaxNewFile = bNewFile;
}

void CKDLog::Log(LPCTSTR lpMsg, eLogLV eLV/* = LOGLV_DEBUG*/)
{
	if (!m_bInit)
		return;
	if (_tcslen(lpMsg) <= 0)
		return;
	if (eLV < m_eDefaultLV)
		return;

	CheckSize();
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
	m_LogFile.SeekToEnd();
	m_LogFile.WriteString(sLogMsg);
	m_LogFile.Flush();
}

void CKDLog::CheckSize()
{
	if (!m_bInit)
		return;
	if (m_lMaxLogSize <= 0)
		return;
	if (m_LogFile.GetLength() < (ULONGLONG)m_lMaxLogSize)
		return;

	if (m_bMaxNewFile) {
		Detach();
		RotateFile(m_sLogPath);
		Attach();
	} else {
#define BUFNUM (32*1024)
		CString sTmpPath;
		CStdioFile tmpFile;
		BYTE *buf = new BYTE[BUFNUM];
		UINT uRead;

		m_LogFile.Seek((LONGLONG)(m_lMaxLogSize*(-0.9)), CFile::end);
		m_LogFile.ReadString(sTmpPath);

		sTmpPath.Format(_T("%s.tmp"), m_sLogPath);
		tmpFile.Open(sTmpPath, CFile::modeCreate|CFile::modeWrite|CFile::shareExclusive);
		while (uRead = m_LogFile.Read(buf, BUFNUM))
			tmpFile.Write(buf, uRead);

		tmpFile.Close();
		delete [] buf;

		Detach();
		DeleteFile(m_sLogPath);
		MoveFile(sTmpPath, m_sLogPath);
		Attach();
#undef BUFNUM
	}
}
