#pragma once

#include "MagicKD.h"
#include "afxwin.h"
// CMagicKDConfigDlg 對話方塊

class CMagicKDConfigDlg : public CDialog
{
	DECLARE_DYNAMIC(CMagicKDConfigDlg)
	DECLARE_MESSAGE_MAP()

public:
	CMagicKDConfigDlg(CWnd* pParent = NULL);   // 標準建構函式
	virtual ~CMagicKDConfigDlg();

// 對話方塊資料
	enum { IDD = IDD_MAIN_CONFIG_DIALOG };

public:
	CButton m_cbWallChanger;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援
	virtual void OnOK();
	virtual void OnCancel();
public:
	afx_msg void OnBnClickedCheckWallchanger();
};
