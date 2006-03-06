#pragma once

#include "afxwin.h"
#include "afxcmn.h"
#include "Resource.h"
#include "KDButton.h"
#include "KDGetHttpFile.h"

class CMagicKDConfDlg : public CDialog
{
	DECLARE_DYNAMIC(CMagicKDConfDlg)
	DECLARE_MESSAGE_MAP()
public:
	enum { IDD = IDD_MAGICKD_CONFIG };
	CMagicKDConfDlg(CWnd* pParent = NULL);   // 標準建構函式
	virtual ~CMagicKDConfDlg();

	void DoSize();

	void UpdateFuncCheck();
	bool IsAppNeedUpdate();

	UINT StartUpdateTimer();
	UINT StopUpdateTimer();

	CKDButton m_checkStartMin;
	CKDButton m_checkShowCloseWindow;
	CKDButton m_check_CheckUpdate;
	CSliderCtrl m_sliderTransparency;
	CProgressCtrl m_progress_Update;
protected:
private:
	bool m_bInit;
	UINT m_uUpdateTimer;
	CString m_sLastVer;
	CString m_sLastUrl;
	CKDGetHttpFile m_GetHttpFile;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBnClickedWallchangercheck();
	afx_msg void OnBnClickedCheckConfStartmin();
	afx_msg void OnBnClickedConfCheckShowclosewindow();
	afx_msg void OnBnClickedConfBtnRestart();
	afx_msg void OnBnClickedConfCheckFinddf();
	afx_msg void OnBnClickedConfBtnCheckupdate();
	afx_msg void OnBnClickedConfCheckCheckupdate();
};
