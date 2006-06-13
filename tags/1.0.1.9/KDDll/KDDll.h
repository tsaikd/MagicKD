#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols


class CKDDllApp : public CWinApp
{
	DECLARE_MESSAGE_MAP()
public:
	CKDDllApp();

public:
	virtual BOOL InitInstance();
};
