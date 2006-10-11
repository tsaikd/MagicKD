#include "StdAfx.h"
#include "PicCConf.h"

#define DEFAULT_DLDIR _T("")

CPicCConf *g_pPicCConf = NULL;

CPicCConf::CPicCConf()
{
}

CPicCConf::~CPicCConf()
{
}

void CPicCConf::Init(CIni *pIni)
{
	CKDConf::Init(pIni);

	m_sDlDir.Init(pIni, _T("General"), _T("sDlDir"), DEFAULT_DLDIR);
}
