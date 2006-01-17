#pragma once
#include "WallListCtrl.h"
#include "WallDirListItem.h"

class CWallDirListCtrl : public CWallListCtrl
{
public:
	CWallDirListCtrl();
	virtual ~CWallDirListCtrl();

	void Init(LPCTSTR sClassListName, int iClassNum);
	void SetClassListName(LPCTSTR sClassListName);
	LPCTSTR GetClassListName();

	void AddItem(int iClassNum, LPCTSTR sPath, int iFindNum = 0);

private:
	bool m_bInit;
	CString m_sClassListName;
	CArray<CWallDirListItem*> m_apDirListItem;


public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnLvnGetdispinfo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnDeleteitem(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
