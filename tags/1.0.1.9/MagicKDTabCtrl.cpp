#include "StdAfx.h"
#include "Resource.h"
#include "Language.h"

#include "MagicKDTabCtrl.h"

CMagicKDTabCtrl::CMagicKDTabCtrl()
{
}

CMagicKDTabCtrl::~CMagicKDTabCtrl()
{
}

int CMagicKDTabCtrl::SetCurSel(int nItem)
{
	int iRes;

	NMHDR nmhdr;
	nmhdr.hwndFrom = GetSafeHwnd();
	nmhdr.idFrom = GetDlgCtrlID();
	nmhdr.code = TCN_SELCHANGING;
	SendMessage(WM_NOTIFY, TCN_SELCHANGING, (LPARAM)&nmhdr);

	iRes = CKDTabCtrl::SetCurSel(nItem);

	nmhdr.code = TCN_SELCHANGE;
	SendMessage(WM_NOTIFY, TCN_SELCHANGE, (LPARAM)&nmhdr);

	return iRes;
}

BEGIN_MESSAGE_MAP(CMagicKDTabCtrl, CKDTabCtrl)
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

void CMagicKDTabCtrl::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	TCHITTESTINFO infoTest;
	GetCursorPos(&infoTest.pt);
	ScreenToClient(&infoTest.pt);

	m_mContextMenu.CreatePopupMenu();
	int iPos = HitTest(&infoTest);
	if ((iPos != -1) && (infoTest.flags & TCHT_ONITEM)) {
		SetCurSel(iPos);
		m_mContextMenu.AppendMenu(MF_STRING, IDS_MENU_CLOSE, CResString(IDS_MENU_CLOSE));
		m_mContextMenu.SetDefaultItem(IDS_MENU_CLOSE);
	}

	if (m_mContextMenu.GetMenuItemCount()) {
		CPoint cpPopMenu;
		GetCursorPos(&cpPopMenu);
		m_mContextMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, cpPopMenu.x, cpPopMenu.y, GetParent());
	}
	m_mContextMenu.DestroyMenu();
}
