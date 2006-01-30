#include "StdAfx.h"
#include "KDIni.h"

CKDIni::CKDIni() : m_bModify(false)
{
}

CKDIni::~CKDIni()
{
}

bool CKDIni::IsIniModify()
{
	return m_bModify;
}

void CKDIni::SetIniModify(bool bModify/* = true*/)
{
	m_bModify = bModify;
}

void CKDIni::SaveIni()
{
	SetIniModify(false);
}
