#pragma once
#include "KDListCtrl.h"
#include "KDIni.h"
#include "Ini.h"

class CWallListCtrl :
	public CKDListCtrl, public CKDIni
{
public:
	CWallListCtrl();
	virtual ~CWallListCtrl();

	void Init(CIni *pIni);
	virtual void SaveIni();
	CIni *m_pIni;

protected:
	CMenu m_mContextMenu;

private:
	bool m_bInit;
protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
