#pragma once

#include "WallChanger.h"
#include "KDTabCtrl.h"

// CMagicKDDlg 對話方塊

class CMagicKDDlg : public CDialog
{
	DECLARE_DYNAMIC(CMagicKDDlg)

public:
	CMagicKDDlg(CWnd* pParent = NULL);   // 標準建構函式
	virtual ~CMagicKDDlg();

// 對話方塊資料
	enum { IDD = IDD_MAGICKDDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援
	virtual BOOL OnInitDialog();

private:
	CWallChanger m_WallChanger;
	CKDTabCtrl m_MainTab;

	DECLARE_MESSAGE_MAP()
};
