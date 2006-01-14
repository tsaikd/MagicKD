// MagicKDDlg.h : 標頭檔 
//

#pragma once

#include "KDTabCtrl.h"
#include "MagicKDConfigDlg.h"
#include "WallChanger.h"


// CMagicKDDlg 對話方塊
class CMagicKDDlg : public CDialog
{
	DECLARE_MESSAGE_MAP()
// 建構
public:
	CMagicKDDlg(CWnd* pParent = NULL);	// 標準建構函式
	virtual ~CMagicKDDlg();

// 對話方塊資料
	enum { IDD = IDD_MAGICKD_DIALOG };

public:
	void MainConfigSyncTabEnable();

	typedef enum { eFunc_WallChanger = 1 } FuncList;
	void SetEnableFunc(FuncList eFunc, bool bEnable);

	CIni m_cIni;

protected:
	CKDTabCtrl m_MainTab;
	CMagicKDConfigDlg m_MainConfigDlg;
	CWallChanger *m_pWallChanger;

private:
	bool m_bInit;
	CRect m_rcTabWindow;

	void InitWindowRect();

// 程式碼實作
protected:
	HICON m_hIcon;

	// 產生的訊息對應函式
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支援
	virtual BOOL OnInitDialog();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	virtual void OnOK();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnTcnSelchangeMaintab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTcnSelchangingMaintab(NMHDR *pNMHDR, LRESULT *pResult);
};
