#include "StdAfx.h"
#include "MagicKDConf.h"

#define DEFAULT_STARTMIN			false
#define DEFAULT_SHOWCLOSEWINDOW		true
#define DEFAULT_CHECKUPDATE			true
#define DEFAULT_WALLCHANGER			false
#define DEFAULT_FINFDUPFILE			false
#define DEFAULT_TRANSPARENCY		255

CMagicKDConf::CMagicKDConf()
{
}

void CMagicKDConf::Init(CIni *pIni)
{
	CKDConf::Init(pIni);

	m_General_bStartMin.Init(pIni, _T("General"), _T("bStartMin"), DEFAULT_STARTMIN);
	m_General_bShowCloseWindow.Init(pIni, _T("General"), _T("bShowCloseWindow"), DEFAULT_SHOWCLOSEWINDOW);
	m_General_bCheckUpdate.Init(pIni, _T("General"), _T("bCheckUpdate"), DEFAULT_CHECKUPDATE);
	m_FuncList_bWallChanger.Init(pIni, _T("FuncList"), _T("bWallChanger"), DEFAULT_WALLCHANGER);
	m_FuncList_bFindDupFile.Init(pIni, _T("FuncList"), _T("bFindDupFile"), DEFAULT_FINFDUPFILE);
	m_General_uTransparency.Init(pIni, _T("General"), _T("uTransparency"), DEFAULT_TRANSPARENCY);
}
