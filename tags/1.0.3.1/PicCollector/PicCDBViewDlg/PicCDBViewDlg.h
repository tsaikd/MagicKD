#pragma once
#include "PicCDBVListCtrl.h"

class CPicCDBViewDlg : public CDialog
{
	DECLARE_DYNAMIC(CPicCDBViewDlg)
	DECLARE_MESSAGE_MAP()
public:
	enum { IDD = IDD_MAGICKD_PICCDBVIEW };
	enum {
		ID_TABLE_NULL,
		ID_TABLE_DLFAILED,
		ID_TABLE_DLOVER,
		ID_TABLE_UNDL
	};
	CPicCDBViewDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPicCDBViewDlg();

	virtual BOOL OnInitDialog();
	void Localize();
	CString GetTableNameFromID(int nID);

	CPicCDBVListCtrl m_list_Item;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
public:
	afx_msg void OnCbnSelchangePiccDbvComboTbname();
	afx_msg void OnEnSetfocusPiccDbvEditUrl();
	afx_msg void OnEnSetfocusPiccDbvEditLocalpath();
	afx_msg void OnBnClickedPiccDbvBtnReload();
	afx_msg void OnBnClickedPiccDbvBtnDelrecode();
};
