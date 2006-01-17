#pragma once
#include <afxtempl.h>
#include "Language.h"
#include "KDListCtrl.h"
#include "WallDirListItem.h"

// CWallListCtrl

class CWallListCtrl : public CKDListCtrl
{
	DECLARE_DYNAMIC(CWallListCtrl)
	DECLARE_MESSAGE_MAP()

public:
	CWallListCtrl();
	virtual ~CWallListCtrl();

	void Init();
	bool IsModified();
	void SetModified(bool bModified = true);

protected:
	CMenu m_mContextMenu;

private:
	bool m_bInit;
	bool m_bModified;
};


