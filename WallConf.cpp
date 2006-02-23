#include "StdAfx.h"
#include "Resource.h"
#include "Language.h"
#include "WallConf.h"

#define DEFAULT_WAITTIME			30
#define DEFAULT_PICPATHHISTORY		10
#define DEFAULT_ENABLETIP			true
#define DEFAULT_SHOWDIRLOADERROR	true
#define DEFAULT_COMBOX_MSG			CResString(IDS_WALL_COMBOX_ASKUSER)

CWallConf::CWallConf()
	:	m_bInit(false), m_pIni(NULL)
{
}

CWallConf::~CWallConf()
{
	SaveConf();
}

void CWallConf::Init(CIni *pIni)
{
	m_pIni = pIni;

	m_General_sComboxMsg.Init(pIni, _T("General"), _T("sComboxMsg"), DEFAULT_COMBOX_MSG);
	m_General_uWaitTime.Init(pIni, _T("General"), _T("uWaitTime"), DEFAULT_WAITTIME);
	m_General_uPicPathHistory.Init(pIni, _T("General"), _T("uPicPathHistory"), DEFAULT_PICPATHHISTORY);
	m_General_bEnableTip.Init(pIni, _T("General"), _T("bEnableTip"), DEFAULT_ENABLETIP);
	m_General_bShowDirLoadError.Init(pIni, _T("General"), _T("bShowDirLoadError"), DEFAULT_SHOWDIRLOADERROR);
	
	m_bInit = true;
}

void CWallConf::SaveConf()
{
	if (!m_bInit)
		return;

	m_General_sComboxMsg.Destroy();
	m_General_uWaitTime.Destroy();
	m_General_uPicPathHistory.Destroy();
	m_General_bEnableTip.Destroy();
	m_General_bShowDirLoadError.Destroy();

	CKDIni *pSaveConf;
	POSITION pos = m_listSaveConf.GetHeadPosition();
	while (pos) {
		pSaveConf = m_listSaveConf.GetAt(pos);
		pSaveConf->SaveIni();

		m_listSaveConf.GetNext(pos);
	}
}

void CWallConf::AddSaveConf(CKDIni *pSaveConf)
{
	m_listSaveConf.AddTail(pSaveConf);
}

void CWallConf::RemoveSaveConf(CKDIni *pSaveConf)
{
	POSITION pos = m_listSaveConf.Find(pSaveConf);
	if (pos)
		m_listSaveConf.RemoveAt(pos);
}
