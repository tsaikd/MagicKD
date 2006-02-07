#pragma once

/*

	if User click on tray icon,
	then in DefWindowProc()
		message = UWM_TRAY_CALLBACK,
		lParam = mouse event (ex: WM_LBUTTONDBLCLK, WM_RBUTTONDOWN)

*/

extern UINT WMU_TRAY_CALLBACK;

class CKDTray
{
public:
	CKDTray();
	virtual ~CKDTray();

	BOOL RegisterTray(HWND hWnd, HICON hIcon, LPCTSTR sTip = NULL);
	void UnRegisterTray();

	LPCTSTR GetTrayTip();
	BOOL SetTrayTip(LPCTSTR sTip);

	CMenu *GetTrayMenu();
	BOOL AppendMenu(UINT nFlags, UINT_PTR nIDNewItem = 0, LPCTSTR lpszNewItem = NULL, bool bDefault = false);
	BOOL InsertMenu(UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem = 0, LPCTSTR lpszNewItem = NULL, bool bDefault = false);
	int FindTrayMenuItem(LPCTSTR sMenuString);
	BOOL RemoveTrayMenuItem(LPCTSTR sMenuString);
	BOOL TrackPopupMenu(CWnd *pWnd, UINT nFlags = TPM_LEFTALIGN | TPM_LEFTBUTTON);

	UINT CheckMenuItem(UINT nIDCheckItem, UINT nCheck);

private:
	NOTIFYICONDATA m_trayData;
	CMenu m_mTrayMenu;
};
