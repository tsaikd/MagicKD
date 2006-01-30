#pragma once
#include "StdAfx.h"

//	Add #include <wininet.h> before #include <afxdtctl.h> in "StdAfx.h"
bool ChooseFolder(LPTSTR sFolder, HWND hWnd = 0);

bool RemoveFileDlg(HWND hWnd, LPCTSTR sFiles, bool bUndo = true);

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
