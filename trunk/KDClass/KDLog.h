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

	void Log(LPCTSTR lpMsg, eLogLV eLV = LOGLV_DEBUG);

private:
	CStdioFile m_LogFile;
	bool m_bInit;
	eLogLV m_eDefaultLV;
};
