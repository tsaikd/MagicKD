#pragma once
#include "KDListItem.h"
#include "KDListCtrl.h"

class CPicCDBVListItem : public CKDListItem
{
public:
	CPicCDBVListItem() : CKDListItem(2) {}
	virtual ~CPicCDBVListItem() {}

	LPCTSTR GetUrl() { return GetText(0); }
	LPCTSTR GetLocalpath() { return GetText(1); }

	bool SetUrl(LPCTSTR lpUrl) { return SetText(0, lpUrl); }
	bool SetLocalpath(LPCTSTR lpLocalpath) { return SetText(1, lpLocalpath); }
};

class CPicCDBVListCtrl : public CKDListCtrl
{
	DECLARE_MESSAGE_MAP()
public:
	CPicCDBVListCtrl();
	virtual ~CPicCDBVListCtrl();

	void Init();
	void Localize();
	void ReloadItem(int nID);
	void AddItem(CPicCDBVListItem *pItem);

public:
	afx_msg void OnLvnDeleteitem(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult);
};
