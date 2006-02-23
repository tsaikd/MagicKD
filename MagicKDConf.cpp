#include "StdAfx.h"
#include "KDIni.h"
#include "MagicKDConf.h"

#define DEFAULT_STARTMIN			false
#define DEFAULT_SHOWCLOSEWINDOW		true
#define DEFAULT_WALLCHANGER			false
#define DEFAULT_TRANSPARENCY		255

CMagicKDConf::CMagicKDConf()
	:	m_bInit(false), m_pIni(NULL)
{
}

CMagicKDConf::~CMagicKDConf()
{
	SaveConf();
}

void CMagicKDConf::Init(CIni *pIni)
{
	m_pIni = pIni;

	m_General_bStartMin.Init(pIni, _T("General"), _T("bStartMin"), DEFAULT_STARTMIN);
	m_General_bShowCloseWindow.Init(pIni, _T("General"), _T("bShowCloseWindow"), DEFAULT_SHOWCLOSEWINDOW);
	m_FuncList_bWallChanger.Init(pIni, _T("FuncList"), _T("bWallChanger"), DEFAULT_WALLCHANGER);
	m_General_uTransparency.Init(pIni, _T("General"), _T("uTransparency"), DEFAULT_TRANSPARENCY);
	
	m_bInit = true;
}

void CMagicKDConf::SaveConf()
{
	if (!m_bInit)
		return;

	m_General_bStartMin.Destroy();
	m_General_bShowCloseWindow.Destroy();
	m_FuncList_bWallChanger.Destroy();
	m_General_uTransparency.Destroy();

	CKDIni *pSaveConf;
	POSITION pos = m_listSaveConf.GetHeadPosition();
	while (pos) {
		pSaveConf = m_listSaveConf.GetAt(pos);
		pSaveConf->SaveIni();

		m_listSaveConf.GetNext(pos);
	}
}

void CMagicKDConf::AddSaveConf(CKDIni *pSaveConf)
{
	m_listSaveConf.AddTail(pSaveConf);
}

void CMagicKDConf::RemoveSaveConf(CKDIni *pSaveConf)
{
	POSITION pos = m_listSaveConf.Find(pSaveConf);
	if (pos)
		m_listSaveConf.RemoveAt(pos);
}
