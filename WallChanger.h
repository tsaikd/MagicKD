#pragma once
#include "afxwin.h"
#include "WallListBox.h"


// CWallChanger ��ܤ��

class CWallChanger : public CDialog
{
	DECLARE_DYNAMIC(CWallChanger)

public:
	CWallChanger(CWnd* pParent = NULL);   // �зǫغc�禡
	virtual ~CWallChanger();

// ��ܤ�����
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �䴩
	HANDLE m_hThread;
	DWORD m_dwThreadId;

	DECLARE_MESSAGE_MAP()
public:
	void CreateThread()
	{ VERIFY( m_hThread = ::CreateThread(NULL, 0, ThreadProc, (LPVOID) this, 0, &m_dwThreadId) ); }
	virtual DWORD ThreadProc();

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLbnSelchangeWallclasslist();
	afx_msg void OnBnClickedWallchangercheck();
	virtual BOOL OnInitDialog();
	bool AddEnableClass();
	bool DelEnableClass();

	CButton m_EnableWallChanger;
	CWallListBox m_ClassList;
	CWallListBox m_DirList;
	CWallListBox m_EnableClass;
	CEdit m_WaitTime;
	CStatic m_Static1;
	CStatic m_Static2;
	CStatic m_Static3;
	CStatic m_Static4;
	bool m_bInit;
};
