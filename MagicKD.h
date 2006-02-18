// MagicKD.h : PROJECT_NAME ���ε{�����D�n���Y��
//

#pragma once

#ifndef __AFXWIN_H__
	#error �b�� PCH �]�t���ɮ׫e���]�t 'stdafx.h'
#endif

#include "resource.h"		// �D�n�Ÿ�
#include "Ini.h"
#include "KDSingleApp.h"
#include "MagicKDEndDlg.h"


// CMagicKDApp:
// �аѾ\��@�����O�� MagicKD.cpp
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
