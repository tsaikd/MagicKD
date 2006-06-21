#include "StdAfx.h"
#include "Resource.h"
#include "WallConf.h"

#define DEFAULT_COMBOX_MSG			IDS_WALL_COMBOX_ASKUSER
#define DEFAULT_WAITTIME			30
#define DEFAULT_PICPATHHISTORY		10
#define DEFAULT_ENABLETIP			true
#define DEFAULT_SHOWDIRLOADERROR	true

CWallConf::CWallConf()
{
}

void CWallConf::Init(CIni *pIni)
{
	CKDConf::Init(pIni);

	m_General_uComboxMsg.Init(pIni, _T("General"), _T("uComboxMsg"), DEFAULT_COMBOX_MSG);
	m_General_uWaitTime.Init(pIni, _T("General"), _T("uWaitTime"), DEFAULT_WAITTIME);
	m_General_uPicPathHistory.Init(pIni, _T("General"), _T("uPicPathHistory"), DEFAULT_PICPATHHISTORY);
	m_General_bEnableTip.Init(pIni, _T("General"), _T("bEnableTip"), DEFAULT_ENABLETIP);
	m_General_bShowDirLoadError.Init(pIni, _T("General"), _T("bShowDirLoadError"), DEFAULT_SHOWDIRLOADERROR);
}
