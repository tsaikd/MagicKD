#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "afxmt.h"
#include "Resource.h"
#include "Ini.h"
#include "xImage.h"
#include "KDEdit.h"
#include "KDButton.h"
#include "KDComboBox.h"
#include "WallClassListCtrl.h"
#include "WallDirListCtrl.h"

// CWallChangerDlg User Message
enum {
	WMU_UPDATE_TOTALNUM = (WM_USER + 1),
	WMU_FIRST_FIND_PIC
};

// CWallChangerDlg 對話方塊

class CWallChangerDlg : public CDialog
{
	DECLARE_DYNAMIC(CWallChangerDlg)
	DECLARE_MESSAGE_MAP()
public:
	enum { IDD = IDD_MAGICKD_WALLCHANGER };
	CWallChangerDlg(CWnd* pParent = NULL);   // 標準建構函式
	virtual ~CWallChangerDlg();

public:
	void DoSize();

	void	SetHistoryNum(UINT uNum);
	void	SetWaitTime(UINT uWaitTime);
	bool	SetRandWallPager();
	LPCTSTR	GetRandPicPath();
	bool	DeletePicFile(LPCTSTR sFilePath, bool bAskForSure = true);

	UINT	StartTimer();
	UINT	StopTimer();

	bool	EnableToolTips(bool bEnable = true);

	CIni	m_cIni;
	CMutex	m_muxRandPic;
	CMutex	m_muxSetRandWallPager;

	CStatic m_staticTime;
	CStatic m_staticNowPicPath;
	CStatic m_staticPicTotalNum;
	CButton m_btn_EnableToolTip;
	CKDButton m_btn_RandPic;
	CKDButton m_btn_DelPic;
	CKDButton m_btn_AddClass;
	CKDButton m_btn_MovePic;
	CKDButton m_checkShowLoadDirError;
	CKDEdit m_editWaitTime;
	CKDEdit m_editAddClass;
	CKDEdit m_editHistoryNum;
	CKDComboBox m_comboxImageLoadError;
	CWallClassListCtrl m_listClass;
	CWallDirListCtrl m_listDirPath;
	CWallDirListCtrl *m_pCurListDirPath;
protected:
private:
	CPoint _AutoPicSize(CPoint &cpSizeSrc, CPoint const &cpSizeMax);

	bool m_bInit;
	bool m_bShowDirLoadError;
	int m_iTestOfflineCount;
	UINT m_uTimer;
	CxImage m_imgNowPic;
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
