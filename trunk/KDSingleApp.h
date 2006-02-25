#pragma once

/*
	define APPNAME and APPUUID in StdAfx.h

	inherit by WinApp
	add in InitInstance() of WinApp
		if (IsWndRunning())
			return FALSE;

	add in DefWindowProc() of Dialog
		if (message == WMU_ARE_YOU_APP) {
			ShowWindow(SW_SHOW);
			SetForegroundWindow();
			return WMU_I_AM_APP;
		}
*/

extern UINT WMU_ARE_YOU_APP;
extern UINT WMU_I_AM_APP;

class CKDSingleApp
{
public:
	CKDSingleApp();
	virtual ~CKDSingleApp();

	bool IsWndRunning();
	static BOOL CALLBACK QueryApp(HWND hWnd, LPARAM lParam);

private:
	HWND m_hOtherWnd;
	HANDLE m_hInstMutex;
};
