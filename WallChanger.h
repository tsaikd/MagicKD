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

	bool m_bInit;

private:
	CTreeCtrl m_FileTree;
	CListCtrl m_FileList;
	CStatic m_TreeStatic;
	CStatic m_ListStatic;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
};
