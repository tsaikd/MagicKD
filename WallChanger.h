#pragma once
#include "afxwin.h"
#include "KDListBox.h"


// CWallChanger 對話方塊

class CWallChanger : public CDialog
{
	DECLARE_DYNAMIC(CWallChanger)

public:
	CWallChanger(CWnd* pParent = NULL);   // 標準建構函式
	virtual ~CWallChanger();

// 對話方塊資料
	enum { IDD = IDD_WALLCHANGER };

private:
	static DWORD WINAPI ThreadProc(LPVOID pParam)
	{
		CWallChanger *pThis = (CWallChanger *) pParam;
		return pThis->ThreadProc();
	}
	bool ChangeList(int iListID);
	UINT m_uWaitTime;
	bool m_bEnableWallChanger;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援
	HANDLE m_hThread;
	DWORD m_dwThreadId;

	DECLARE_MESSAGE_MAP()
public:
	void CreateThread()
	{ VERIFY( m_hThread = ::CreateThread(NULL, 0, ThreadProc, (LPVOID) this, 0, &m_dwThreadId) ); }
	virtual DWORD ThreadProc();

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLbnSelchangeWallclasslist();
	virtual BOOL OnInitDialog();
	CButton m_EnableWallChanger;
	CKDListBox m_ClassList;
	CKDListBox m_DirList;
	CKDListBox m_EnableClass;
	CEdit m_WaitTime;
	CStatic m_Static1;
	CStatic m_Static2;
	CStatic m_Static3;
	CStatic m_Static4;
	bool m_bInit;
	afx_msg void OnBnClickedWallchangercheck();
};
