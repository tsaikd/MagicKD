#pragma once
#include "KDListCtrl.h"
#include "KDIni.h"

class CWallListCtrl :
	public CKDListCtrl, public CKDIni
{
	DECLARE_MESSAGE_MAP()
public:
	CWallListCtrl();
	virtual ~CWallListCtrl();

	void Init(CIni *pIni);
	virtual void SaveIni();
	CIni *m_pIni;

private:
	bool m_bInit;
protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnDestroy();
};
