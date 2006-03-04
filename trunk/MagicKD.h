#pragma once

#ifndef __AFXWIN_H__
	#error �b�� PCH �]�t���ɮ׫e���]�t 'stdafx.h'
#endif

#include "resource.h"
#include "Ini.h"
#include "KDApp.h"
#include "KDSingleApp.h"
#include "MagicKDConf.h"
#include "MagicKDEndDlg.h"

class CMagicKDApp : public CWinApp, public CKDSingleApp, public CKDApp
{
public:
	CMagicKDApp();

	void Quit();

	CIni m_cIni;

public:
	virtual BOOL InitInstance();
};

extern CMagicKDApp theApp;
