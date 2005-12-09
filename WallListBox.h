#pragma once
#include "MagicKD.h"
#include "KDListBox.h"
#include "Language.h"

class CWallListBox : public CKDListBox
{
public:
	CWallListBox();
	virtual ~CWallListBox();

	void Init(void *pParent);

	void *m_pParent;

private:
	CMenu m_mContextMenu;
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
