#pragma once
#include "afxwin.h"
#include "WallListCtrl.h"
#include "WallDirListItem.h"


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
	afx_msg void OnBnClickedWallchangercheck();
	afx_msg void OnBnClickedBtnNewclass();
	afx_msg void OnLvnItemchangedWallclasslist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedWallenableclass(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMSetfocusWallenableclass(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMSetfocusWallclasslist(NMHDR *pNMHDR, LRESULT *pResult);

	virtual BOOL OnInitDialog();
	void AddClassToEnable();
	void DelEnableClass();
	bool SetSelClassItemToDirList(CWallListCtrl *pClassList, CWallListCtrl *pDirList);
	void NewClassList();
	void DelClassList();
	void AddClassDir();
	void DelClassDir();

	CButton m_EnableWallChanger;
	CWallListCtrl m_EnableClass;
	CWallListCtrl m_ClassList;
	CWallListCtrl m_DirList;
	CEdit m_WaitTime;
	CStatic m_Static1;
	CStatic m_Static2;
	CStatic m_Static3;
	CStatic m_Static4;
	bool m_bInit;
	CEdit m_Edit_NewClass;
	CButton m_BTN_NewClass;
	afx_msg void OnLvnGetdispinfoWalldirlist(NMHDR *pNMHDR, LRESULT *pResult);
};
