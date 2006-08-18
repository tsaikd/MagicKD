#include "StdAfx.h"
#include "FindDupFileProc.h"

CFindDupFileProc::CFindDupFileProc()
	: m_u64FileSize(0), m_pListSameFile(NULL), m_bAddToSameList(false)
{
}

CFindDupFileProc::~CFindDupFileProc()
{
	if (m_pListSameFile)
		delete m_pListSameFile;
}
