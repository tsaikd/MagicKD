#pragma once
#include "Others.h"

/*
	You should add the following lines to main function:

	g_pKDSysInfo = new CKDSysInfo;

	...
	...

	delete g_pKDSysInfo;

*/

class CKDSysInfo
{
public:
	CKDSysInfo();
	virtual ~CKDSysInfo();

	bool IsDesktopVisible();
	bool IsFullScreen();

private:
	enum {
		KDSYSINFO_NULL,
		KDSYSINFO_DESKTOPVISIBLE,
		KDSYSINFO_FULLSCREEN
	};
	bool SetSysInfo(int nID, bool bValue);
	bool IsOutOfDate(int nID);
	static BOOL CALLBACK AskMaxWindowSize(HWND hWnd, LPARAM lParam);

	bool m_bDesktopVisible;
	clock_t m_clockDesktopVisible;
	bool m_bFullScreen;
	clock_t m_clockFullScreen;
};

extern CKDSysInfo *g_pKDSysInfo;
