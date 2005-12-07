// WallChanger.cpp : 實作檔
//

#include "stdafx.h"
#include "MagicKD.h"
#include "WallChanger.h"


// CWallChanger 對話方塊

IMPLEMENT_DYNAMIC(CWallChanger, CDialog)
CWallChanger::CWallChanger(CWnd* pParent /*=NULL*/)
	: CDialog(CWallChanger::IDD, pParent)
{
}

CWallChanger::~CWallChanger()
{
}

void CWallChanger::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWallChanger, CDialog)
END_MESSAGE_MAP()


// CWallChanger 訊息處理常式
