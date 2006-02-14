#include "StdAfx.h"
#include "KDSingleApp.h"

UINT WMU_ARE_YOU_APP = RegisterWindowMessage(_T("Req") APPNAME APPUUID);
UINT WMU_I_AM_APP = RegisterWindowMessage(_T("Res") APPNAME APPUUID);

CKDSingleApp::CKDSingleApp()
	: m_hOtherWnd(NULL), m_hInstMutex(NULL)
{
}

CKDSingleApp::~CKDSingleApp()
{
	if (m_hInstMutex)
		CloseHandle(m_hInstMutex);
}

bool CKDSingleApp::IsWndRunning()
{
	m_hInstMutex = CreateMutex(NULL, FALSE, APPNAME APPUUID);

	if (GetLastError() == ERROR_ALREADY_EXISTS || GetLastError() == ERROR_ACCESS_DENIED) {
		EnumWindows(QueryApp, (LPARAM) &m_hOtherWnd);
		if (m_hOtherWnd)
			return true;
	}

	return false;
}

BOOL CALLBACK CKDSingleApp::QueryApp(HWND hWnd, LPARAM lParam)
{
	DWORD dwMsgResult;
	LRESULT res = ::SendMessageTimeout(hWnd, WMU_ARE_YOU_APP, 0, 0, SMTO_BLOCK | SMTO_ABORTIFHUNG, 10000, &dwMsgResult);

	if (res == 0)
		return TRUE;

	if (dwMsgResult == WMU_I_AM_APP) {
		*((HWND *) lParam) = hWnd;
		return FALSE; 
	}

	return TRUE; 
}
