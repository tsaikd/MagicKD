#include "StdAfx.h"
#include "WallListCtrl.h"

CWallListCtrl::CWallListCtrl() : m_pIni(NULL), m_bInit(false)
{
}

CWallListCtrl::~CWallListCtrl()
{
}

void CWallListCtrl::Init(CIni *pIni)
{
	m_pIni = pIni;
	SetExtendedStyle(GetExtendedStyle() | LVS_EX_FULLROWSELECT);

	m_bInit = true;
}

void CWallListCtrl::SaveIni()
{
	if (!m_bInit)
		return;

	CKDIni::SaveIni();
}
