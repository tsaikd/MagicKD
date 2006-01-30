// MagicKDDlg.h : 標頭檔 
//

#pragma once

#include "afxcmn.h"
#include "MainConfigDlg.h"
#include "Ini.h"
#include "KDTabCtrl.h"
#include "KDIni.h"

#include "WallChangerDlg.h"

// CMagicKDDlg 對話方塊
class CMagicKDDlg : public CDialog, public CKDIni
{
// 建構
public:
	CMagicKDDlg(CWnd* pParent = NULL);	// 標準建構函式

// 對話方塊資料
	enum { IDD = IDD_MAGICKD_DIALOG };
	typedef enum { eFunc_WallChanger = 1 } FuncList;

public:
	void InitTabRect();
	void DoSize();
	void SetFuncEnable(FuncList eFunc, bool bEnable, bool bRedraw = true);
	virtual void SaveIni();

	CIni *m_pIni;
	CKDTabCtrl m_cMainTab;
	CMainConfigDlg m_cMainConfigDlg;
	CWallChangerDlg *m_pWallChangerDlg;
protected:
private:
	CRect m_rcMainTab;
	bool m_bIniModify;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支援


	DECLARE_MESSAGE_MAP()
// 程式碼實作
protected:
	HICON m_hIcon;

	// 產生的訊息對應函式
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	virtual void OnCancel();
public:
	afx_msg void OnDestroy();
	afx_msg void OnTcnSelchangeMaintab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTcnSelchangingMaintab(NMHDR *pNMHDR, LRESULT *pResult);
};
