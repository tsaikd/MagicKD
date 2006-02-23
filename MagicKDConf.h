#pragma once
#include "KDConf.h"
#include "KDIni.h"

class CMagicKDConf
{
public:
	CMagicKDConf();
	virtual ~CMagicKDConf();

	void Init(CIni *pIni);
	void SaveConf();

	void AddSaveConf(CKDIni *pSaveConf);
	void RemoveSaveConf(CKDIni *pSaveConf);

	CKDConfBool m_General_bStartMin;
	CKDConfBool m_General_bShowCloseWindow;
	CKDConfBool m_FuncList_bWallChanger;
	CKDConfUINT m_General_uTransparency;

private:
	CIni *m_pIni;
	bool m_bInit;
	CList<CKDIni *, CKDIni *> m_listSaveConf;
};

extern CMagicKDConf theConf;
