// MagicKD.h : PROJECT_NAME 應用程式的主要標頭檔
//

#pragma once

#ifndef __AFXWIN_H__
	#error 在對 PCH 包含此檔案前先包含 'stdafx.h'
#endif

#include "resource.h"		// 主要符號


// CMagicKDApp:
// 請參閱實作此類別的 MagicKD.cpp
//

class CMagicKDApp : public CWinApp
{
public:
	CMagicKDApp();

// 覆寫
	public:
	virtual BOOL InitInstance();

	CString m_sAppPath;
	CString m_sAppDir;

	DECLARE_MESSAGE_MAP()
};

extern CMagicKDApp theApp;
