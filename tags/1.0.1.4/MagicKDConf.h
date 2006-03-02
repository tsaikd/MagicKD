#pragma once
#include "KDConf.h"

class CMagicKDConf : public CKDConf
{
public:
	CMagicKDConf();
	virtual ~CMagicKDConf();

	void Init(CIni *pIni);

	CKDConfBool m_General_bStartMin;
	CKDConfBool m_General_bShowCloseWindow;
	CKDConfBool m_General_bCheckUpdate;
	CKDConfBool m_FuncList_bWallChanger;
	CKDConfBool m_FuncList_bFindDupFile;
	CKDConfUINT m_General_uTransparency;
};

extern CMagicKDConf theConf;
