#pragma once

#include "afxwin.h"
#include "afxcmn.h"
#include "Resource.h"
#include "Ini.h"
#include "KDIni.h"
#include "KDButton.h"

// CMainConfigDlg 對話方塊

class CMainConfigDlg : public CDialog, public CKDIni
{
	DECLARE_DYNAMIC(CMainConfigDlg)
	DECLARE_MESSAGE_MAP()
public:
	CMainConfigDlg(CWnd* pParent = NULL);   // 標準建構函式
	virtual ~CMainConfigDlg();

	enum { IDD = IDD_MAGICKD_CONFIG };

public:
	void DoSize();
	virtual void SaveIni();

	bool IsStartMin();
	bool IsShowCloseWindow();
	bool IsWallChanger();

	UINT GetSliderTransparency();
	void UpdateFuncCheck();

	CIni *m_pIni;
	CButton m_checkWallChanger;
	CKDButton m_checkStartMin;
	CKDButton m_checkShowCloseWindow;
	CSliderCtrl m_sliderTransparency;
protected:
private:
	UINT m_uTransparency;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedWallchangercheck();
	afx_msg void OnBnClickedCheckConfStartmin();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedConfCheckShowclosewindow();
};
