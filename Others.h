#pragma once
#include "StdAfx.h"

//	Add #include <wininet.h> before #include <afxdtctl.h> in "StdAfx.h"
bool ChooseFolder(CString &sFolder, HWND hWnd = 0);
bool ChooseFolder(LPTSTR sFolder, HWND hWnd = 0);

bool RemoveFileDlg(HWND hWnd, LPCTSTR sFiles, bool bUndo = true);

CString GetTempFilePath(LPSTR lpTempFilePath = NULL, LPCTSTR lpTempDir = NULL, LPCTSTR lpPreFix = NULL);

#ifdef _UNICODE
/*
	Must Turn On Unicode
*/
bool SetWallpaper(LPCTSTR sFilePath, DWORD dwStyle = WPSTYLE_STRETCH);
#endif // _UNICODE

inline bool IsCtrlPressed()
{
	return (GetKeyState(VK_CONTROL) & 0x8000) != 0;
}

inline bool IsShiftPressed()
{
	return (GetKeyState(VK_SHIFT) & 0x8000) != 0;
}

inline bool IsAltPressed()
{
	return (GetKeyState(VK_MENU) & 0x8000) != 0;
}

inline bool IsWinPressed()
{
	return (GetKeyState(VK_LWIN) & 0x8000) != 0 || (GetKeyState(VK_RWIN) & 0x8000) != 0;
}

#define INRANGE(v, m, M)		((v) >= (m) && (v) <= (M))
#define MAKE_INRANGE(v, m, M)	((v) = min(max((v), (m)), (M)))

#define INRANGE_R(v, m, M)		((v) >= (m) && (v) < (M))
#define INRANGE_L(v, m, M)		((v) > (m) && (v) <= (M))
#define INRANGE_LR(v, m, M)		((v) > (m) && (v) < (M))

#define RECT_WIDTH(rc)		((rc).right - (rc).left)
#define RECT_HEIGHT(rc)		((rc).bottom - (rc).top)

int GetOnInternet(LPCSTR lpszTestHost = "www.google.com", u_short uTestPort = 80);
int GetOnOffline();
