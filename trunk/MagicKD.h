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
	DECLARE_MESSAGE_MAP()
public:
	CMagicKDApp();

	CIni m_cIni;

public:
	virtual BOOL InitInstance();
};

extern CMagicKDApp theApp;
