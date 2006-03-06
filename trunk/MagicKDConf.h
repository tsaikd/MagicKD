#pragma once
#include "KDConf.h"

class CMagicKDConf : public CKDConf
{
public:
	CMagicKDConf();

	void Init(CIni *pIni);

	CKDConfBool m_General_bStartMin;
	CKDConfBool m_General_bShowCloseWindow;
	CKDConfBool m_General_bCheckUpdate;
	CKDConfBool m_General_bUpdateLastest;
	CKDConfBool m_FuncList_bWallChanger;
	CKDConfBool m_FuncList_bFindDupFile;
	CKDConfUINT m_General_uTransparency;
	CKDConfUINT m_General_uLangID;
};

extern CMagicKDConf *g_pTheConf;
