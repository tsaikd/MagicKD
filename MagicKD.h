// MagicKD.h : PROJECT_NAME 應用程式的主要標頭檔
//

#pragma once

#ifndef __AFXWIN_H__
	#error 在對 PCH 包含此檔案前先包含 'stdafx.h'
#endif

#include "resource.h"		// 主要符號
#include "Ini.h"
#include "KDSingleApp.h"
#include "MagicKDEndDlg.h"


// CMagicKDApp:
// 請參閱實作此類別的 MagicKD.cpp
//

class CMagicKDApp : public CWinApp, public CKDSingleApp
{
public:
	CMagicKDApp();

	CString m_sAppDir;
	CString m_sAppPath;
	CIni m_cIni;
// 覆寫
public:
	virtual BOOL InitInstance();

// 程式碼實作

	DECLARE_MESSAGE_MAP()
};

extern CMagicKDApp theApp;
extern CMagicKDEndDlg theAppEndDlg;
