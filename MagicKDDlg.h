// MagicKDDlg.h : 標頭檔 
//

#pragma once

#include "WallChanger.h"


// CMagicKDDlg 對話方塊
class CMagicKDDlg : public CDialog
{
// 建構
public:
	CMagicKDDlg(CWnd* pParent = NULL);	// 標準建構函式

// 對話方塊資料
	enum { IDD = IDD_MAGICKD_DIALOG };

private:
	bool m_bInit;

// 程式碼實作
protected:
	HICON m_hIcon;
	CTabCtrl m_MainTab;
	CWallChanger m_WallChanger;

	// 產生的訊息對應函式
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支援
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMove(int x, int y);
protected:
	virtual void OnOK();
};
