#include "StdAfx.h"
#include "KDSysInfo.h"

#define DEFAULT_CLOCK_DESKTOPVISIBLE	2000 // (ms)
#define DEFAULT_CLOCK_FULLSCREEN		1000 // (ms)

CKDSysInfo *g_pKDSysInfo = NULL;

CKDSysInfo::CKDSysInfo()
	:	m_bDesktopVisible(true), m_clockDesktopVisible(0)
		,m_bFullScreen(false), m_clockFullScreen(0)
{
}

CKDSysInfo::~CKDSysInfo()
{
}

bool CKDSysInfo::IsDesktopVisible()
{
	if (!IsOutOfDate(KDSYSINFO_DESKTOPVISIBLE))
		return m_bDesktopVisible;

	CRect rcTmp;
	CSize sizeWin(0, 0);
	CSize sizeMax;
	HWND hDesktop = GetDesktopWindow();
	VERIFY(IsWindow(hDesktop));

	EnumWindows(AskMaxWindowSize, (LPARAM) &sizeWin);

	GetWindowRect(hDesktop, rcTmp);
	sizeMax.SetSize(min(rcTmp.Width(), GetSystemMetrics(SM_CXMAXIMIZED)),
		min(rcTmp.Height(), GetSystemMetrics(SM_CYMAXIMIZED)));

	if ((sizeWin.cx >= sizeMax.cx) && (sizeWin.cy >= sizeMax.cy))
		return SetSysInfo(KDSYSINFO_DESKTOPVISIBLE, false);

	return SetSysInfo(KDSYSINFO_DESKTOPVISIBLE, true);
}

bool CKDSysInfo::IsFullScreen()
{
	if (!IsOutOfDate(KDSYSINFO_FULLSCREEN))
		return m_bFullScreen;

	CRect rcTmp;
	GetWindowRect(GetForegroundWindow(), rcTmp);

	if ((rcTmp.Width() >= GetSystemMetrics(SM_CXSCREEN)) && (rcTmp.Height() >= GetSystemMetrics(SM_CYSCREEN)))
		return SetSysInfo(KDSYSINFO_FULLSCREEN, true);

	return SetSysInfo(KDSYSINFO_FULLSCREEN, false);
}

bool CKDSysInfo::SetSysInfo(int nID, bool bValue)
{
	switch (nID) {
	case KDSYSINFO_DESKTOPVISIBLE:
		m_bDesktopVisible = bValue;
		m_clockDesktopVisible = clock();
		break;
	case KDSYSINFO_FULLSCREEN:
		m_bFullScreen = bValue;
		m_clockFullScreen = clock();
		break;
	}
	return bValue;
}

bool CKDSysInfo::IsOutOfDate(int nID)
{
	switch (nID) {
	case KDSYSINFO_DESKTOPVISIBLE:
		if ((clock() - m_clockDesktopVisible) < (DEFAULT_CLOCK_DESKTOPVISIBLE * (1000/CLOCKS_PER_SEC)))
			return false;
		break;
	case KDSYSINFO_FULLSCREEN:
		if ((clock() - m_clockFullScreen) < (DEFAULT_CLOCK_FULLSCREEN * (1000/CLOCKS_PER_SEC)))
			return false;
		break;
	}
	return true;
}

BOOL CKDSysInfo::AskMaxWindowSize(HWND hWnd, LPARAM lParam)
{
	while (IsWindow(hWnd) && IsWindow(GetParent(hWnd)))
		hWnd = GetParent(hWnd);

	if (!IsUserVisiableAppWnd(hWnd))
		return TRUE;

	CRect rcTmp;
	CSize *psizeMax = (CSize *)lParam;

	GetWindowRect(hWnd, rcTmp);

	if (psizeMax->cx < rcTmp.Width() || psizeMax->cy < rcTmp.Height())
		psizeMax->SetSize(max(psizeMax->cx, rcTmp.Width()), max(psizeMax->cy, rcTmp.Height()));

	return TRUE;
}
