// MagicKD.h : PROJECT_NAME ���ε{�����D�n���Y��
//

#pragma once

#ifndef __AFXWIN_H__
	#error �b�� PCH �]�t���ɮ׫e���]�t 'stdafx.h'
#endif

#include "resource.h"		// �D�n�Ÿ�


// CMagicKDApp:
// �аѾ\��@�����O�� MagicKD.cpp
//

class CMagicKDApp : public CWinApp
{
public:
	CMagicKDApp();

// �мg
	public:
	virtual BOOL InitInstance();

	CString m_sAppPath;
	CString m_sAppDir;

	DECLARE_MESSAGE_MAP()
};

extern CMagicKDApp theApp;
