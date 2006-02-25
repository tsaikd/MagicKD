#pragma once
#include "KDListItem.h"
#include "WallListCtrl.h"

class CWallClassListItem :
	public CKDListItem
{
public:
	CWallClassListItem() : CKDListItem(1), m_pChildDirList(NULL) {}
	virtual ~CWallClassListItem() {}

	CString GetItemName() { return GetText(0); }
	void SetItemName(CString sName) { SetText(0, sName); }
	void *GetChildDirList() { return m_pChildDirList; }
	void SetChildDirList(void *pChildDirList) { m_pChildDirList = pChildDirList; }

private:
	void *m_pChildDirList;
};



class CWallClassListCtrl :
	public CWallListCtrl
{
	DECLARE_MESSAGE_MAP()
public:
	CWallClassListCtrl();
	virtual ~CWallClassListCtrl();
	virtual void SaveIni();

	virtual void Init(CIni *pIni, CRect &rcChildDirList, bool *bShowDirLoadError);
	bool AddItem(LPCTSTR sClassName);
	void SetItemEnable(int nItem, bool bEnable);
	void UpdateSelectItemFileFindNum();

	bool EnableToolTips(LPCTSTR sToolTip = NULL, bool bEnable = true);

	void RemoveAllDirListFromPath(CString &sPath);

private:
	bool m_bInit;
	bool *m_pbShowDirLoadError;
	int m_iIDC_WALLDIRLISTBASE;
	CRect m_rcChildDirList;

public:
	afx_msg void OnDestroy();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnLvnDeleteitem(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
};
