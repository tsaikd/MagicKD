#pragma once

#include "afxwin.h"
#include "afxcmn.h"
#include "Resource.h"
#include "KDButton.h"

class CMainConfigDlg : public CDialog
{
	DECLARE_DYNAMIC(CMainConfigDlg)
	DECLARE_MESSAGE_MAP()
public:
	enum { IDD = IDD_MAGICKD_CONFIG };
	CMainConfigDlg(CWnd* pParent = NULL);   // 標準建構函式
	virtual ~CMainConfigDlg();

	void DoSize();

	void UpdateFuncCheck();

	CButton m_checkWallChanger;
	CKDButton m_checkStartMin;
	CKDButton m_checkShowCloseWindow;
	CSliderCtrl m_sliderTransparency;
protected:
private:
	bool m_bInit;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedWallchangercheck();
	afx_msg void OnBnClickedCheckConfStartmin();
	afx_msg void OnBnClickedConfCheckShowclosewindow();
	afx_msg void OnBnClickedConfBtnRestart();
};
