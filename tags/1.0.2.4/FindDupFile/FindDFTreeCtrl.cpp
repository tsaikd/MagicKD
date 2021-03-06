#include "StdAfx.h"
#include "Resource.h"
#include "Language.h"

#include "FindDFTreeCtrl.h"

CFindDFTreeCtrl::CFindDFTreeCtrl()
	:	m_hContextItem(NULL)
{
}

CFindDFTreeCtrl::~CFindDFTreeCtrl()
{
}

BEGIN_MESSAGE_MAP(CFindDFTreeCtrl, CTreeCtrl)
	ON_WM_CONTEXTMENU()
	ON_NOTIFY_REFLECT(NM_RCLICK, OnNMRclick)
END_MESSAGE_MAP()

void CFindDFTreeCtrl::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	UINT uFlags = 0;
	CPoint pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);
	m_hContextItem = HitTest(pt, &uFlags);
	m_mContextMenu.CreatePopupMenu();
	if (m_hContextItem && (uFlags & TVHT_ONITEM)) {
		m_mContextMenu.AppendMenu(MF_STRING, IDS_FIND_MENU_OPENFILE, CResString(IDS_FIND_MENU_OPENFILE));
		m_mContextMenu.AppendMenu(MF_STRING, IDS_FIND_MENU_OPENDIR, CResString(IDS_FIND_MENU_OPENDIR));
	}

	if (m_mContextMenu.GetMenuItemCount()) {
		CPoint cpPopMenu;
		GetCursorPos(&cpPopMenu);
		m_mContextMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, cpPopMenu.x, cpPopMenu.y, GetParent());
	}
	m_mContextMenu.DestroyMenu();
	Invalidate();
}

void CFindDFTreeCtrl::OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult)
{
	ASSERT(::IsWindow(pNMHDR->hwndFrom));
	OnContextMenu(0, 0);

	*pResult = 0;
}
