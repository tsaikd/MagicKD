#pragma once

class CFindDupFileProc
{
public:
	CFindDupFileProc();
	~CFindDupFileProc();

	bool m_bAddToSameList;
	ULONGLONG m_u64FileSize;
	CString m_sFilePath;
	FILETIME m_ftLastWriteTime;
	CList<CFindDupFileProc *, CFindDupFileProc *> *m_pListSameFile;
};
