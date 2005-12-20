#pragma once
#include "KDListItem.h"
#include "WallDirListCtrl.h"

class CWallClassListItem : public CKDListItem
{
public:
	CWallClassListItem() : m_pChildDirList(NULL) {}
	virtual ~CWallClassListItem() {}

	CString GetItemName() { return m_sName; }
	void SetItemName(CString sName) { m_sName = sName; }
	CWallDirListCtrl *GetChildDirList() { return m_pChildDirList; }
	void SetChildDirList(CWallDirListCtrl *pChildDirList) { m_pChildDirList = pChildDirList; }

private:
	CString m_sName;
	CWallDirListCtrl *m_pChildDirList;
};
