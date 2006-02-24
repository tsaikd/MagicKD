#pragma once

class CFindDupFileProc
{
public:
	CFindDupFileProc(LPCTSTR sFilePath);
	~CFindDupFileProc();

	bool m_bFileValid;
	bool m_bAddToSameList;
	DWORD m_dwFileSize;
	CString m_sFilePath;
	FILETIME m_ftLastWriteTime;
	CList<CFindDupFileProc *, CFindDupFileProc *> *m_pListSameFile;
};
