#pragma once
#include "KDConf.h"
#include "KDIni.h"

class CWallConf
{
public:
	CWallConf();
	virtual ~CWallConf();

	void Init(CIni *pIni);
	void SaveConf();

	void AddSaveConf(CKDIni *pSaveConf);
	void RemoveSaveConf(CKDIni *pSaveConf);

	CKDConfString m_General_sComboxMsg;
	CKDConfUINT m_General_uWaitTime;
	CKDConfUINT m_General_uPicPathHistory;
	CKDConfBool m_General_bEnableTip;
	CKDConfBool m_General_bShowDirLoadError;

private:
	CIni *m_pIni;
	bool m_bInit;
	CList<CKDIni *, CKDIni *> m_listSaveConf;
};

extern CWallConf *g_pWallConf;
