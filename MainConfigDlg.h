#pragma once

#include "Ini.h"
#include "afxwin.h"

// CMainConfigDlg 對話方塊

class CMainConfigDlg : public CDialog
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
	bool IsIniModify();
	void SetIniModify(bool bModify = true);
	void SaveIni();

	CIni *m_pIni;
	CButton m_cbWallChanger;
protected:
private:
	bool m_bModify;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedWallchangercheck();
};
