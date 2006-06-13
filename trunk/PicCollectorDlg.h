#pragma once
#include "KDListCtrl.h"


// CPicCollectorDlg dialog

class CPicCollectorDlg : public CDialog
{
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CPicCollectorDlg)
public:
	enum { IDD = IDD_MAGICKD_PICCOLLECTOR };
	CPicCollectorDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPicCollectorDlg();

	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	void Localize();

	CKDListCtrl m_list_Feed;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
};
