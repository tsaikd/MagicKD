#include "StdAfx.h"
#include "FindDupFileProc.h"

CFindDupFileProc::CFindDupFileProc(LPCTSTR sFilePath)
	:	m_dwFileSize(0), m_pListSameFile(NULL), m_bAddToSameList(false)
{
	m_sFilePath = sFilePath;

	HANDLE hFile = CreateFile(m_sFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE) {
		m_bFileValid = false;
		m_dwFileSize = 0;
	} else {
		m_bFileValid = true;
		m_dwFileSize = GetFileSize(hFile, NULL);
		GetFileTime(hFile, NULL, NULL, &m_ftLastWriteTime);
		CloseHandle(hFile);
	}
}

CFindDupFileProc::~CFindDupFileProc()
{
	if (m_pListSameFile)
		delete m_pListSameFile;
}
