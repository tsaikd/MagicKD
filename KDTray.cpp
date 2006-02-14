#include "StdAfx.h"
#include "KDTray.h"

#define TRAY_ICON_ID 1
UINT WMU_TRAY_CALLBACK = ::RegisterWindowMessage(_T("WMU_TRAY_CALLBACK-")
												 _T("{6b8c4821-e6a4-11D1-8370-00aa005c0507}"));

CKDTray::CKDTray()
{
	ZeroMemory(&m_trayData, sizeof(m_trayData));
	m_mTrayMenu.CreatePopupMenu();
}

CKDTray::~CKDTray()
{
	m_mTrayMenu.DestroyMenu();
	UnRegisterTray();
}

bool CKDTray::RegisterTray(HWND hWnd, HICON hIcon, LPCTSTR sTip/* = NULL*/)
{
	if (m_trayData.hWnd)
		UnRegisterTray();

	m_trayData.cbSize			= sizeof(m_trayData);
	m_trayData.hWnd				= hWnd;
	m_trayData.uID				= TRAY_ICON_ID;
	m_trayData.uCallbackMessage	= WMU_TRAY_CALLBACK;
	m_trayData.hIcon			= hIcon;
	if (sTip)
		_tcscpy(m_trayData.szTip, sTip);
	m_trayData.uFlags			= NIF_MESSAGE | NIF_ICON | NIF_TIP;

	return Shell_NotifyIcon(NIM_ADD, &m_trayData) != FALSE;
}

void CKDTray::UnRegisterTray()
{
	if (m_trayData.hWnd)
		Shell_NotifyIcon(NIM_DELETE, &m_trayData);
	memset(&m_trayData, 0, sizeof(m_trayData));
}

LPCTSTR CKDTray::GetTrayTip()
{
	return m_trayData.szTip;
}

bool CKDTray::SetTrayTip(LPCTSTR sTip)
{
	if (!m_trayData.hWnd)
		return false;

	_tcscpy(m_trayData.szTip, sTip);

	return Shell_NotifyIcon(NIM_MODIFY, &m_trayData) != FALSE;
}

CMenu *CKDTray::GetTrayMenu() {
	return &m_mTrayMenu;
}

bool CKDTray::AppendMenu(UINT nFlags, UINT_PTR nIDNewItem/* = 0*/, LPCTSTR lpszNewItem/* = NULL*/, bool bDefault/* = false*/) {
	bool bRes = m_mTrayMenu.AppendMenu(nFlags, nIDNewItem, lpszNewItem) != FALSE;
	if (!(nFlags & MF_SEPARATOR) && bDefault)
		bRes = bRes && m_mTrayMenu.SetDefaultItem(nIDNewItem);

	return bRes;
}

//nFlags:
//	MF_BYCOMMAND or MF_BYPOSITION
//	MF_OWNERDRAW or MF_STRING or MF_SEPARATOR
bool CKDTray::InsertMenu(UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem/* = 0*/, LPCTSTR lpszNewItem/* = NULL*/, bool bDefault/* = false*/) {
	bool bRes = m_mTrayMenu.InsertMenu(nPosition, nFlags, nIDNewItem, lpszNewItem) != FALSE;
	if (!(nFlags & MF_SEPARATOR) && bDefault)
		bRes = bRes && m_mTrayMenu.SetDefaultItem(nIDNewItem);

	return bRes;
}

int CKDTray::FindTrayMenuItem(LPCTSTR sMenuString) {
	CString	sBuf;
	int iCount = m_mTrayMenu.GetMenuItemCount();
	for (int i=0 ; i<iCount ; i++) {
		if ((m_mTrayMenu.GetMenuString(i, sBuf, MF_BYPOSITION)) && (_tcscmp(sBuf, sMenuString)==0))
			return i;
	}
	return -1;
}

bool CKDTray::RemoveTrayMenuItem(LPCTSTR sMenuString) {
	int iPos = FindTrayMenuItem(sMenuString);
	if (iPos > -1)
		return m_mTrayMenu.RemoveMenu(iPos, MF_BYPOSITION) != FALSE;
	return false;
}

bool CKDTray::TrackPopupMenu(CWnd *pWnd, UINT nFlags/* = TPM_LEFTALIGN | TPM_LEFTBUTTON*/)
{
	CPoint cpPopMenu;
	GetCursorPos(&cpPopMenu);
	return m_mTrayMenu.TrackPopupMenu(nFlags, cpPopMenu.x, cpPopMenu.y, pWnd) != FALSE;
}

//nCheck:
//	MF_BYCOMMAND or MF_BYPOSITION
//	MF_CHECKED or MF_UNCHECKED
//return:
//	if success, MF_CHECKED or MF_UNCHECKED
//	failed, INFINITE
UINT CKDTray::CheckMenuItem(UINT nIDCheckItem, UINT nCheck)
{
	return m_mTrayMenu.CheckMenuItem(nIDCheckItem, nCheck);
}
