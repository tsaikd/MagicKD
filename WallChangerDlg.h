#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "KDIni.h"
#include "Ini.h"
#include "xImage.h"
#include "WallClassListCtrl.h"
#include "WallDirListCtrl.h"

// CWallChangerDlg 對話方塊

class CWallChangerDlg : public CDialog, public CKDIni
{
	DECLARE_DYNAMIC(CWallChangerDlg)
	DECLARE_MESSAGE_MAP()
public:
	CWallChangerDlg(CWnd* pParent = NULL);   // 標準建構函式
	virtual ~CWallChangerDlg();

// 對話方塊資料
	enum { IDD = IDD_WALLCHANGERDIALOG };

public:
	void DoSize();
	virtual void SaveIni();

	void SetHistoryNum(UINT uNum);
	bool SetCachePicNum(UINT uCachePicNum);
	UINT GetCachePicNum();
	const CString GetRandPicPath();

	CIni m_cIni;
	bool m_bCanThread;

	CStatic m_staticTime;
	CStatic m_staticNowPicPath;
	CEdit m_editWaitTime;
	CEdit m_editAddClass;
	CEdit m_editHistoryNum;
	CEdit m_editCacheNum;
	CWallClassListCtrl m_listClass;
	CWallDirListCtrl m_listDirPath;
	CWallDirListCtrl *m_pCurListDirPath;
protected:
private:
	CPoint _AutoPicSize(CPoint &cpSizeSrc, CPoint const &cpSizeMax);

	bool m_bInit;
	bool m_bIsThreading;
	UINT m_uWaitTime;
	UINT m_uCachePicNum;
	// at least 1. if in history, then don't set to wallpager
	// if larger than total list number, then equal to set 1
	UINT m_uPicPathHistory;
	CxImage m_xImgNowPic;
	CString m_sNowPicPath;
	CString m_sTempFilePath;
	CStringList m_slPicPathHistory;

// Thread Stander Code
public:
	void CreateThread()
	{ VERIFY( m_hThread = ::CreateThread(NULL, 0, ThreadProc, (LPVOID) this, 0, &m_dwThreadId) ); }
	virtual DWORD ThreadProc();
protected:
	HANDLE m_hThread;
	DWORD m_dwThreadId;
private:
	static DWORD WINAPI ThreadProc(LPVOID pParam) {
		CWallChangerDlg *pThis = (CWallChangerDlg *) pParam;
		return pThis->ThreadProc();
	}
// Thread Stander Code End

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援
	virtual void OnOK();
	virtual void OnCancel();
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonSetwaittime();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonAddclasslist();
	afx_msg void OnLvnItemchangedListClass(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonRandpic();
	afx_msg void OnBnClickedButtonDelpic();
};
