#pragma once
#include "WallListCtrl.h"
#include "WallClassListItem.h"

class CWallClassListCtrl : public CWallListCtrl
{
public:
	CWallClassListCtrl();
	virtual ~CWallClassListCtrl();

	void Init();
	bool AddItem(LPCTSTR sName);

private:
	bool m_bInit;
	CArray<CWallClassListItem*> m_apClassListItem;
	int m_iIDC_WALLDIRLISTBASE;


public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnLvnGetdispinfo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnDeleteitem(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
