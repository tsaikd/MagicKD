#include "StdAfx.h"
#include "KDIni.h"

CKDIni::CKDIni()
	:	m_pIni(NULL), m_bModify(false), m_bNeedFreeMem(false)
{
}

CKDIni::~CKDIni()
{
	SaveIni();
}

void CKDIni::SaveIni()
{
	SetIniModify(false);
}

void CKDIni::Init(CIni *pIni)
{
	m_pIni = pIni;
}

void CKDIni::Init(LPCTSTR lpIniPathName)
{
	m_pIni = new CIni;
	m_pIni->SetPathName(lpIniPathName);
	m_bNeedFreeMem = true;
}

bool CKDIni::IsIniModify()
{
	return m_bModify;
}

void CKDIni::SetIniModify(bool bModify/* = true*/)
{
	m_bModify = bModify;
}
