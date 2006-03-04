#pragma once

#define DEL(x)					if ((x)) { delete (x); (x) = NULL; }

#define INRANGE(v, m, M)		((v) >= (m) && (v) <= (M))
#define MAKE_INRANGE(v, m, M)	((v) = min(max((v), (m)), (M)))

#define INRANGE_R(v, m, M)		((v) >= (m) && (v) < (M))
#define INRANGE_L(v, m, M)		((v) > (m) && (v) <= (M))
#define INRANGE_LR(v, m, M)		((v) > (m) && (v) < (M))

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

#define KDMOVEDLGITEM_WAY_LEFT		0x01
#define KDMOVEDLGITEM_WAY_TOP		0x02
#define KDMOVEDLGITEM_WAY_RIGHT		0x03
#define KDMOVEDLGITEM_WAY_BOTTOM	0x04
#define KDMOVEDLGITEM_WAY_F_OUTSIDE	0x00
#define KDMOVEDLGITEM_WAY_F_INSIDE	0x10
bool KDMoveDlgItem(CWnd *pItemWnd, CWnd *pRefWnd, int iRefWay, int iRefDist, bool bExtend = false, CPoint *ptOffset = NULL);

//	Add #include <wininet.h> before #include <afxdtctl.h> in "StdAfx.h"
bool ChooseFolder(CString &sFolder, HWND hWnd = 0, LPCTSTR lpTitle = NULL);
bool ChooseFolder(LPTSTR lpFolder, HWND hWnd = 0, LPCTSTR lpTitle = NULL);

bool RemoveFileDlg(LPCTSTR lpFiles, HWND hWnd = NULL, bool bUndo = true);
bool MoveFileDlg(LPCTSTR lpFileFrom, LPCTSTR lpFIleTo, HWND hWnd = NULL, bool bUndo = true);

bool CalcPicSize(CSize &sizeSrc, const CSize &sizeMax);

void GetTempFilePath(LPTSTR lpTempFilePath, LPCTSTR lpTempDir = NULL, LPCTSTR lpPreFix = NULL, LPCTSTR lpPostFix = NULL);
class CTempFilePath : public CString
{
public:
	CTempFilePath(LPCTSTR lpTempDir = NULL, LPCTSTR lpPreFix = NULL, LPCTSTR lpPostFix = NULL)
	{
		LPTSTR lpTempFilePath = GetBuffer(MAX_PATH);
		GetTempFilePath(lpTempFilePath, lpTempDir, lpPreFix, lpPostFix);
		ReleaseBuffer();
	}
};

class CScreenSize : public CSize
{
public:
	CScreenSize(int iOffsetX = 0, int iOffsetY = 0)
	{
		CRect rcWin;
		GetWindowRect(GetDesktopWindow(), rcWin);
		SetSize(rcWin.Width() + iOffsetX, rcWin.Height() + iOffsetY);
	}
};

#ifdef _UNICODE
/*
	Must Turn On Unicode
*/
bool SetWallpaper(LPCTSTR lpFilePath, DWORD dwStyle = WPSTYLE_STRETCH);
bool AddDesktopPic(LPCTSTR lpPicPath, const CSize &sizePic);
bool RemoveDesktopPic(LPCTSTR lpPicPath);
#endif // _UNICODE

bool DownloadFileFromHttp(LPCTSTR lpURL, LPCTSTR lpLocalPath, int iQuerySize = 8192);

int GetOnInternet(LPCSTR lpszTestHost = "www.google.com", u_short uTestPort = 80);
int GetOnOffline();
