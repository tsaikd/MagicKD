#include "StdAfx.h"
#include "FindDFConf.h"

#define DEFAULT_ENABLETOOLTIP true

CFindDFConf::CFindDFConf()
{
}

CFindDFConf::~CFindDFConf()
{
}

void CFindDFConf::Init(CIni *pIni)
{
	CKDConf::Init(pIni);

	m_bEnableToolTip.Init(pIni, _T("General"), _T("bEnableToolTip"), DEFAULT_ENABLETOOLTIP);
}
