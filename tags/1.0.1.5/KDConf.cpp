#include "StdAfx.h"
#include "KDConf.h"

CKDConf::CKDConf()
	:	m_bInit(false), m_pIni(NULL)
{
}

CKDConf::~CKDConf()
{
	SaveConf();
}

void CKDConf::Init(CIni *pIni)
{
	m_pIni = pIni;

	m_bInit = true;
}

void CKDConf::SaveConf()
{
	if (!m_bInit)
		return;

	CKDIni *pSaveConf;
	POSITION pos = m_listSaveConf.GetHeadPosition();
	while (pos) {
		pSaveConf = m_listSaveConf.GetAt(pos);
		pSaveConf->SaveIni();

		m_listSaveConf.GetNext(pos);
	}
}

void CKDConf::AddSaveConf(CKDIni *pSaveConf)
{
	m_listSaveConf.AddTail(pSaveConf);
}

void CKDConf::RemoveSaveConf(CKDIni *pSaveConf)
{
	CKDIni *pKDIni = NULL;
	POSITION pos = m_listSaveConf.Find(pSaveConf);
	if (pos) {
		pKDIni = m_listSaveConf.GetAt(pos);
		m_listSaveConf.RemoveAt(pos);
		if (pKDIni)
			pKDIni->SaveIni();
	}
}
