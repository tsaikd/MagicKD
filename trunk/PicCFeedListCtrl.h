#pragma once
#include "KDListCtrl.h"

class CPicCFeedListItem : public CKDListItem
{
public:
	CPicCFeedListItem() : CKDListItem(3) {}
	virtual ~CPicCFeedListItem() {}

	LPCTSTR GetName() { return GetText(0); }
	LPCTSTR GetFeedTitle() { return GetText(1); }
	LPCTSTR GetFeedURL() { return GetText(2); }
};


class CPicCFeedListCtrl : public CKDListCtrl
{
	DECLARE_MESSAGE_MAP()
public:
	CPicCFeedListCtrl();
	virtual ~CPicCFeedListCtrl();
	virtual void Init();

	void AddItem(CPicCFeedListItem *pItem);
	void ReloadItems();

private:
	bool m_bInit;

public:
	afx_msg void OnLvnDeleteitem(NMHDR *pNMHDR, LRESULT *pResult);
};
