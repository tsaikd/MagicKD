#include "StdAfx.h"
#include "Language.h"
#include "MagicKDConf.h"

#define DEFAULT_STARTMIN			false
#define DEFAULT_SHOWCLOSEWINDOW		true
#define DEFAULT_CHECKUPDATE			true
#define DEFAULT_UPDATELASTEST		false
#define DEFAULT_WALLCHANGER			false
#define DEFAULT_FINFDUPFILE			false
#define DEFAULT_PICCOLLECTOR		false
#define DEFAULT_TRANSPARENCY		255
#define DEFAULT_LANGID				FindDefaultLang()

CMagicKDConf::CMagicKDConf()
{
}

void CMagicKDConf::Init(CIni *pIni)
{
	m_General_bStartMin.Init(pIni, _T("General"), _T("bStartMin"), DEFAULT_STARTMIN);
	m_General_bShowCloseWindow.Init(pIni, _T("General"), _T("bShowCloseWindow"), DEFAULT_SHOWCLOSEWINDOW);
	m_General_bCheckUpdate.Init(pIni, _T("General"), _T("bCheckUpdate"), DEFAULT_CHECKUPDATE);
	m_General_bUpdateLastest.Init(pIni, _T("General"), _T("bUpdateLastest"), DEFAULT_UPDATELASTEST);
	m_General_uTransparency.Init(pIni, _T("General"), _T("uTransparency"), DEFAULT_TRANSPARENCY);
	m_General_uLangID.Init(pIni, _T("General"), _T("uLangID"), DEFAULT_LANGID);
	m_FuncList_bWallChanger.Init(pIni, _T("FuncList"), _T("bWallChanger"), DEFAULT_WALLCHANGER);
	m_FuncList_bFindDupFile.Init(pIni, _T("FuncList"), _T("bFindDupFile"), DEFAULT_FINFDUPFILE);
	m_FuncList_bPicCollector.Init(pIni, _T("FuncList"), _T("bPicCollector"), DEFAULT_PICCOLLECTOR);
}
