#pragma once
#include "afxwin.h"
#include "MagicKD.h"
#include "WallEnableCtrl.h"
#include "WallClassListCtrl.h"
#include "WallDirListCtrl.h"

#include "WallListCtrl.h"


// CWallChanger 對話方塊

class CWallChanger : public CDialog
{
public:
	CWallChanger(CWnd* pParent = NULL);   // 標準建構函式
	virtual ~CWallChanger();

	void SetEnableWallChanger(bool bEnable);
	void DoOnSize();
	void AddClassToEnable();
	void DelEnableClass();
	void NewClassList(LPCTSTR sClassName = NULL);
	void DelClassList();
	void AddClassDir();
	void DelClassDir();
	bool SetDirList(CWallDirListCtrl *pDirList, LPCTSTR sDirListName);
	void SyncListSel(CWallListCtrl *pMasterList, CWallListCtrl *pSlaveList);

	CIni m_cIni;
	CButton m_EnableWallChanger;
	CWallEnableCtrl m_EnableClass;
	CWallClassListCtrl m_ClassList;
	CWallDirListCtrl m_DirList;
	CWallDirListCtrl *m_pCurDirList;
	CEdit m_WaitTime;
	CStatic m_Static1;
	CStatic m_Static2;
	CStatic m_Static3;
	CStatic m_Static4;
	CEdit m_Edit_NewClass;
	CButton m_BTN_NewClass;

private:
	UINT m_uWaitTime;
	bool m_bEnableWallChanger;
	bool m_bInit;

// Thread Stander Code
public:
	void CreateThread()
	{ VERIFY( m_hThread = ::CreateThread(NULL, 0, ThreadProc, (LPVOID) this, 0, &m_dwThreadId) ); }
	virtual DWORD ThreadProc();
protected:
	HANDLE m_hThread;
	DWORD m_dwThreadId;
	bool m_bIsThreadRunning;
private:
	static DWORD WINAPI ThreadProc(LPVOID pParam) {
		CWallChanger *pThis = (CWallChanger *) pParam;
		return pThis->ThreadProc();
	}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

// 對話方塊資料
public:
	enum { IDD = IDD_WALLCHANGER };

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedWallchangercheck();
	afx_msg void OnBnClickedBtnNewclass();
	afx_msg void OnLvnItemchangedWallclasslist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedWallenableclass(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMSetfocusWallenableclass(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMSetfocusWallclasslist(NMHDR *pNMHDR, LRESULT *pResult);

	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();

	DECLARE_DYNAMIC(CWallChanger)
	DECLARE_MESSAGE_MAP()
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
