#pragma once
#include <afxtempl.h>
#include "Language.h"
#include "KDListCtrl.h"
#include "WallDirListItem.h"

// CWallListCtrl

class CWallListCtrl : public CKDListCtrl
{
	DECLARE_DYNAMIC(CWallListCtrl)

public:
	CWallListCtrl();
	virtual ~CWallListCtrl();

	void Init(void *pParent);

	void *m_pParent;

private:
	bool m_bInit;
	CArray<CWallDirListItem, CWallDirListItem&> m_aryDirListItem;

protected:
	CMenu m_mContextMenu;


protected:
	DECLARE_MESSAGE_MAP()
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnLvnGetdispinfo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnDeleteitem(NMHDR *pNMHDR, LRESULT *pResult);
};


