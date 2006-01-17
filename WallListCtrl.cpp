// WallListCtrl.cpp : 實作檔
//

#include "StdAfx.h"
#include "WallListCtrl.h"


// CWallListCtrl

IMPLEMENT_DYNAMIC(CWallListCtrl, CListCtrl)
CWallListCtrl::CWallListCtrl() : m_bInit(false), m_bModified(false)
{
}

CWallListCtrl::~CWallListCtrl()
{
}

void CWallListCtrl::Init()
{
	SetExtendedStyle(GetExtendedStyle() | LVS_EX_FULLROWSELECT);

	m_bInit = true;
}

bool CWallListCtrl::IsModified()
{
	return m_bModified;
}

void CWallListCtrl::SetModified(bool bModified/* = true*/)
{
	m_bModified = bModified;
}

BEGIN_MESSAGE_MAP(CWallListCtrl, CListCtrl)
END_MESSAGE_MAP()

// CWallListCtrl 訊息處理常式
