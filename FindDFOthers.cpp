#include "StdAfx.h"

#include "FindDFOthers.h"

#define READ_FILE_BUFFER_SIZE 8388608

int FileTimeCmp(FILETIME *pft1, FILETIME *pft2)
{
	if (pft1->dwHighDateTime > pft2->dwHighDateTime)
		return 1;
	if (pft1->dwHighDateTime < pft2->dwHighDateTime)
		return -1;

	if (pft1->dwLowDateTime > pft2->dwLowDateTime)
		return 1;
	if (pft1->dwLowDateTime < pft2->dwLowDateTime)
		return -1;

	return 0;
}

int FileCmpCB(const void *pv1, const void *pv2)
{
	CFindDupFileProc *p1 = (*(CFindDupFileProc **)pv1);
	CFindDupFileProc *p2 = (*(CFindDupFileProc **)pv2);

	if (!p1->m_bFileValid || !p2->m_bFileValid)
		return 0;
	if (p1->m_dwFileSize > p2->m_dwFileSize)
		return 1;
	if (p1->m_dwFileSize < p2->m_dwFileSize)
		return -1;

	HANDLE h1 = CreateFile(p1->m_sFilePath, FILE_READ_DATA | SYNCHRONIZE, FILE_SHARE_READ, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);
	HANDLE h2 = CreateFile(p2->m_sFilePath, FILE_READ_DATA | SYNCHRONIZE, FILE_SHARE_READ, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);

	if ((h1 == INVALID_HANDLE_VALUE) || (h2 == INVALID_HANDLE_VALUE)) {
		// output open error
		CloseHandle(h1);
		CloseHandle(h2);
		return 0;
	}

	BOOL bRes1;
	BOOL bRes2;
	DWORD dwByteRead;
	BYTE *sBuf1 = new BYTE [READ_FILE_BUFFER_SIZE];
	BYTE *sBuf2 = new BYTE [READ_FILE_BUFFER_SIZE];
	BYTE *pBuf1;
	BYTE *pBuf2;

#define RETURN(x)	{						\
						delete [] sBuf1;	\
						delete [] sBuf2;	\
						CloseHandle(h1);	\
						CloseHandle(h2);	\
						return ((x));		\
					}

	while (1) {
		bRes1 = ReadFile(h1, sBuf1, READ_FILE_BUFFER_SIZE, &dwByteRead, NULL);
		bRes2 = ReadFile(h2, sBuf2, READ_FILE_BUFFER_SIZE, &dwByteRead, NULL);

		if (bRes1 && bRes2 && (dwByteRead == 0))
			break;
		if (!bRes1 || !bRes2)
			break;

		pBuf1 = sBuf1;
		pBuf2 = sBuf2;

		while (dwByteRead--) {
			if (*pBuf1 > *pBuf2)
				RETURN(1);
			if (*pBuf1 < *pBuf2)
				RETURN(-1);

			pBuf1++;
			pBuf2++;
		}
	}

	// log the same file
	MergeListFindDupFileProc(p1, p2);

	RETURN(0);
#undef RETURN
}

void MergeListFindDupFileProc(CFindDupFileProc *p1, CFindDupFileProc *p2)
{
	if (p1 == p2)
		return;

	if (FileTimeCmp(&p1->m_ftLastWriteTime, &p2->m_ftLastWriteTime)) {
		// p1 is newer than p2
		CFindDupFileProc *pt = p1;
		p1 = p2;
		p2 = pt;
	}

	// p1 is always older than p2
	CListFindDupFileProc *pList1 = p1->m_pListSameFile;
	CListFindDupFileProc *pList2 = p2->m_pListSameFile;

	if (pList1 && pList2) {
		pList1->AddTail(p2);
		pList1->AddTail(pList2);
		delete p2->m_pListSameFile;
		p2->m_pListSameFile = NULL;
	} else if (pList1) {
		if (!p2->m_bAddToSameList) {
			pList1->AddTail(p2);
		}
	} else if (pList2) {
		if (!p1->m_bAddToSameList) {
			p1->m_pListSameFile = pList2;
			p2->m_pListSameFile = NULL;

			p1->m_pListSameFile->AddTail(p2);
		}
	} else {
		if (!p2->m_bAddToSameList) {
			p1->m_pListSameFile = new CListFindDupFileProc;
			p1->m_pListSameFile->AddTail(p2);
		}
	}

	p2->m_bAddToSameList = true;
}
