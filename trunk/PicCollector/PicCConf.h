#pragma once
#include "KDConf.h"

class CPicCConf : public CKDConf
{
public:
	CPicCConf();
	virtual ~CPicCConf();

	void Init(CIni *pIni);

	CKDConfString m_sDlDir;
};

extern CPicCConf *g_pPicCConf;
