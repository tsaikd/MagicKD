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

	bool RegisterTray(HWND hWnd, HICON hIcon, LPCTSTR sTip = NULL);
	void UnRegisterTray();

	LPCTSTR GetTrayTip();
	bool SetTrayTip(LPCTSTR sTip);

	CMenu *GetTrayMenu();
	bool AppendMenu(UINT nFlags, UINT_PTR nIDNewItem = 0, LPCTSTR lpszNewItem = NULL, bool bDefault = false);
	bool InsertMenu(UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem = 0, LPCTSTR lpszNewItem = NULL, bool bDefault = false);
	bool ModifyMenu(UINT nID, LPCTSTR lpszNewItem);
	int FindTrayMenuItem(LPCTSTR sMenuString);
	bool RemoveTrayMenuItem(LPCTSTR sMenuString);
	bool RemoveTrayMenuItem(int nMenuID);
	bool TrackPopupMenu(CWnd *pWnd, UINT nFlags = TPM_LEFTALIGN | TPM_LEFTBUTTON);

	UINT CheckMenuItem(UINT nIDCheckItem, UINT nCheck);

private:
	NOTIFYICONDATA m_trayData;
	CMenu m_mTrayMenu;
};
