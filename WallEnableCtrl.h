#pragma once
#include "WallListCtrl.h"
#include "WallEnableItem.h"

class CWallEnableCtrl : public CWallListCtrl
{
public:
	CWallEnableCtrl();
	virtual ~CWallEnableCtrl();

	void Init();
	void AddItem(LPCTSTR sName, void *pChildList);

private:
	bool m_bInit;
	int m_iIDC_WALLDIRLISTBASE;


public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnLvnGetdispinfo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnDeleteitem(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
