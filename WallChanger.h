#pragma once


// CWallChanger 對話方塊

class CWallChanger : public CDialog
{
	DECLARE_DYNAMIC(CWallChanger)

public:
	CWallChanger(CWnd* pParent = NULL);   // 標準建構函式
	virtual ~CWallChanger();

// 對話方塊資料
	enum { IDD = IDD_WALLCHANGER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

	DECLARE_MESSAGE_MAP()
};
