#pragma once

#include "Ini.h"
#include "afxwin.h"
#include "afxcmn.h"
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

// 對話方塊資料
	enum { IDD = IDD_MAGICCONFIGDIALOG };

public:
	void DoSize();
	virtual void SaveIni();

	bool IsStartMin();
	bool IsWallChanger();

	UINT GetSliderTransparency();

	CIni *m_pIni;
	CButton m_cbWallChanger;
	CKDButton m_checkStartMin;
	CSliderCtrl m_sliderTransparency;
protected:
private:
	UINT m_uTransparency;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedWallchangercheck();
	afx_msg void OnBnClickedCheckConfStartmin();
	afx_msg void OnDestroy();
protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
