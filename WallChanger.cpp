// WallChanger.cpp : ��@��
//

#include "stdafx.h"
#include "MagicKD.h"
#include "WallChanger.h"


// CWallChanger ��ܤ��

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


// CWallChanger �T���B�z�`��
