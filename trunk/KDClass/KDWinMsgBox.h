#pragma once
#include "KDThread.h"
/*
	This class provide MessageBox which will show at non-fullscreen mode

	You should add the following lines to main function:

	g_pKDWinMsgBox = new CKDSysInfo;

	...
	...

	delete g_pKDWinMsgBox;

*/

#define KDMessageBox(hWnd, lpText, lpCaption, uType)			\
	if (g_pKDWinMsgBox)											\
		g_pKDWinMsgBox->Add(hWnd, lpText, lpCaption, uType);	\
	else														\
		::MessageBox(hWnd, lpText, lpCaption, uType);

class CKDWinMsgBoxProc
{
public:
	CKDWinMsgBoxProc() : m_hWnd(NULL), m_uType(0) {}
	CKDWinMsgBoxProc(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType)
		:	m_hWnd(hWnd), m_sText(lpText), m_sCaption(lpCaption), m_uType(uType) {
		if (!lpCaption && IsWindow(hWnd)) {
			GetWindowText(hWnd, m_sCaption.GetBuffer(MAX_PATH), MAX_PATH);
			m_sCaption.ReleaseBuffer();
		}
	}

	HWND m_hWnd;
	CString m_sText;
	CString m_sCaption;
	UINT m_uType;
};

class CKDWinMsgBox : public CKDThread
{
public:
	CKDWinMsgBox();
	virtual ~CKDWinMsgBox();
	virtual DWORD ThreadProc();

	void Add(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType);

private:
	CMutex m_mux;
	CList<CKDWinMsgBoxProc> m_lMsg;
};

extern CKDWinMsgBox *g_pKDWinMsgBox;
