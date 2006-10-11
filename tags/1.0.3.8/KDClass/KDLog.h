#pragma once

class CKDLog
{
public:
	typedef enum {
		LOGLV_DEBUG,
		LOGLV_INFO,
		LOGLV_WARN,
		LOGLV_ERROR
	} eLogLV;

	CKDLog();
	virtual ~CKDLog();

	bool Init(LPCTSTR lpLogFilePath, eLogLV eDefaultLV = LOGLV_INFO);
	void Destroy();

	void Attach();
	void Detach();

	void SetMaxLogSize(LONGLONG lSize, bool bNewFile);
	void Log(LPCTSTR lpMsg, eLogLV eLV = LOGLV_DEBUG);
	void CheckSize();

private:
	CStdioFile m_LogFile;
	eLogLV m_eDefaultLV;
	bool m_bInit;
	bool m_bMaxNewFile;
	LONGLONG m_lMaxLogSize;
	CString m_sLogPath;
};
