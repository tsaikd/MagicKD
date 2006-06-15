#pragma once
#include "Ini.h"
#include "PicCFeed.h"
#include "PicCFeedListCtrl.h"
#include "KDGetHttpFile.h"
#include "PicCHTMLEventHandler.h"


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

	void AddNewFeed(LPCTSTR lpURL, LPCTSTR lpLocalName);
	void RefreshFeed(LPCTSTR lpURL);
	void RefreshAllFeed();

	CIni					m_Ini;
	CPicCFeedListCtrl		m_list_Feed;
	CPicCFeed				m_Feed;
	CKDGetHttpFile			m_DownLoader;

private:
	CPicCHTMLEventHandler	m_HTMLEventHandler;
	CLiteHTMLReader			m_HTMLReader;

public:
	afx_msg void OnBnClickedPiccBtnChangedldir();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	virtual void OnCancel();
};

extern CPicCollectorDlg *g_pPicCollectorDlg;
