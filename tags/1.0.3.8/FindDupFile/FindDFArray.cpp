#include "StdAfx.h"
#include "FindDFOthers.h"

#include "FindDFArray.h"

#define READ_FILE_BUFFER_SIZE (1 << 20)
#define READ_FILE_BUFFER_STEP 10

CFindDFArray::CFindDFArray()
{
}

CFindDFArray::~CFindDFArray()
{
}

int CFindDFArray::QuickSortCmp(CFindDupFileProc *&x, CFindDupFileProc *&y)
{
	if (x == y)
		return 0;
	if (x->m_u64FileSize > y->m_u64FileSize)
		return 1;
	if (x->m_u64FileSize < y->m_u64FileSize)
		return -1;

	HANDLE h1 = CreateFile(x->m_sFilePath, FILE_READ_DATA | SYNCHRONIZE, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	HANDLE h2 = CreateFile(y->m_sFilePath, FILE_READ_DATA | SYNCHRONIZE, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

	if ((h1 == INVALID_HANDLE_VALUE) || (h2 == INVALID_HANDLE_VALUE)) {
		// output open error
		CloseHandle(h1);
		CloseHandle(h2);
		return 0;
	}

	BOOL bRes1;
	BOOL bRes2;
	DWORD dwByteRead;
	UINT uStep = READ_FILE_BUFFER_STEP;
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
		bRes1 = ReadFile(h1, sBuf1, READ_FILE_BUFFER_SIZE >> uStep, &dwByteRead, NULL);
		bRes2 = ReadFile(h2, sBuf2, READ_FILE_BUFFER_SIZE >> uStep, &dwByteRead, NULL);

		if (!bRes1 || !bRes2)
			RETURN(0);
		if (!dwByteRead)
			break;
		if (uStep)
			uStep--;

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

	// Recode the same file
	MergeListFindDupFileProc(x, y);

	RETURN(0);
#undef RETURN
}
