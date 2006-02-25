#pragma once
#include "KDConf.h"

class CWallConf : public CKDConf
{
public:
	CWallConf();

	void Init(CIni *pIni);

	CKDConfString m_General_sComboxMsg;
	CKDConfUINT m_General_uWaitTime;
	CKDConfUINT m_General_uPicPathHistory;
	CKDConfBool m_General_bEnableTip;
	CKDConfBool m_General_bShowDirLoadError;
};

extern CWallConf *g_pWallConf;
