#pragma once
#include "KDConf.h"

class CFindDFConf : public CKDConf
{
public:
	CFindDFConf();
	virtual ~CFindDFConf();

	void Init(CIni *pIni);

	CKDConfBool m_bEnableToolTip;
};

extern CFindDFConf *g_pFindConf;