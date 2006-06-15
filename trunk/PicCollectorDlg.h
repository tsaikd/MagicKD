#pragma once
#include "Ini.h"
#include "FeedSource.h"
#include "PicCFeedListCtrl.h"


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

	void RefreshAllFeed();

	CIni				m_Ini;
	CPicCFeedListCtrl	m_list_Feed;
	CFeed				m_Feed;

public:
	afx_msg void OnBnClickedPiccBtnChangedldir();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	virtual void OnCancel();
};

extern CPicCollectorDlg *g_pPicCollectorDlg;
