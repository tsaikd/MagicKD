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
	DECLARE_MESSAGE_MAP()
public:
	CMagicKDApp();

	void SetRestart(bool bRestart = true);

	CString m_sAppDir;
	CString m_sAppPath;
	CIni m_cIni;

private:
	bool m_bRestart;

public:
	virtual BOOL InitInstance();
};

extern CMagicKDApp theApp;
