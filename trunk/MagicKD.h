#pragma once

#ifndef __AFXWIN_H__
	#error �b�� PCH �]�t���ɮ׫e���]�t 'stdafx.h'
#endif

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
	bool IsOnQuit();

	CIni m_cIni;
private:
	bool m_bOnQuit;

public:
	virtual BOOL InitInstance();
};

extern CMagicKDApp theApp;
