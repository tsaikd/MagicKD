#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "afxmt.h"
#include "Resource.h"
#include "Ini.h"
#include "xImage.h"
#include "KDIni.h"
#include "KDEdit.h"
#include "KDButton.h"
#include "KDComboBox.h"
#include "WallClassListCtrl.h"
#include "WallDirListCtrl.h"

// CWallChangerDlg User Message
enum {
	WMU_UPDATE_CACHENUM = (WM_USER + 1),
	WMU_UPDATE_TOTALNUM,
	WMU_FIRST_FIND_PIC
};

// CWallChangerDlg 對話方塊

class CWallChangerDlg : public CDialog, public CKDIni
{
	DECLARE_DYNAMIC(CWallChangerDlg)
	DECLARE_MESSAGE_MAP()
public:
	CWallChangerDlg(CWnd* pParent = NULL);   // 標準建構函式
	virtual ~CWallChangerDlg();

// 對話方塊資料
	enum { IDD = IDD_MAGICKD_WALLCHANGER };

public:
	void DoSize();
	virtual void SaveIni();

	void	SetHistoryNum(UINT uNum);
	void	SetWaitTime(UINT uWaitTime);
	bool	SetCachePicNum(UINT uCachePicNum);
	UINT	GetCachePicNum();
	bool	SetRandWallPager(bool bWay);
	LPCTSTR	GetRandPicPath();
	bool	DeletePicFile(LPCTSTR sFilePath, bool bAskForSure = true);

	UINT	StartTimer();
	UINT	StopTimer();

	BOOL	EnableToolTips(BOOL bEnable = TRUE);

	CIni	m_cIni;
	CMutex	m_muxRandPic;
	CMutex	m_muxSetRandWallPager;

	CStatic m_staticTime;
	CStatic m_staticNowPicPath;
	CStatic m_staticCachePicNum;
	CStatic m_staticPicTotalNum;
	CButton m_btn_EnableToolTip;
	CKDButton m_btn_RandPic;
	CKDButton m_btn_DelPic;
	CKDButton m_btn_AddClass;
	CKDButton m_checkShowLoadDirError;
	CKDEdit m_editWaitTime;
	CKDEdit m_editAddClass;
	CKDEdit m_editHistoryNum;
	CKDEdit m_editCacheNum;
	CKDComboBox m_comboxImageLoadError;
	CWallClassListCtrl m_listClass;
	CWallDirListCtrl m_listDirPath;
	CWallDirListCtrl *m_pCurListDirPath;
protected:
private:
	CPoint _AutoPicSize(CPoint &cpSizeSrc, CPoint const &cpSizeMax);

	bool m_bInit;
	bool m_bSetCachePicNum;
	bool m_bShowDirLoadError;
	BOOL m_bEnableTip;
	UINT m_uWaitTime;
	UINT m_uCachePicNum;
	int m_iTestOfflineCount;
	// at least 1. if in history, then don't set to wallpager
	// if larger than total list number, then equal to set 1
	UINT m_uPicPathHistory;
	UINT m_uTimer;
	CString m_sComboxMsg;
	CxImage m_xImgNowPic;
	CString m_sNowPicPath;
	CString m_sTempFilePath;
	CStringList m_slPicPathHistory;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援
	virtual void OnOK();
	virtual void OnCancel();
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonAddclasslist();
	afx_msg void OnLvnItemchangedListClass(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonRandpic();
	afx_msg void OnBnClickedButtonDelpic();
	afx_msg void OnBnClickedButtonEnabletooltip();
	afx_msg void OnCbnSelchangeComboImageloaderror();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBnClickedWallCheckShowdirloaderror();
	afx_msg void OnBnClickedWallBtnMovepic();
};

extern CWallChangerDlg *g_pWallChangerDlg;
