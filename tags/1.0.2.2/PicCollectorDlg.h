#pragma once
#include "Resource.h"
#include "Ini.h"
#include "PicCFeed.h"
#include "PicCFeedListCtrl.h"
#include "KDThread.h"
#include "PicCHTMLEventHandler.h"
#include "PicCDLManager.h"


class CPicCollectorDlg : public CDialog, public CKDThread
{
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CPicCollectorDlg)
public:
	enum { IDD = IDD_MAGICKD_PICCOLLECTOR };
	CPicCollectorDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPicCollectorDlg();

	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual DWORD ThreadProc();
	void Localize();

	void AddNewFeed(LPCTSTR lpURL, LPCTSTR lpLocalName);
	void RefreshFeed(LPCTSTR lpURL);
	void RefreshAllFeed();

	CIni					m_Ini;
	CPicCFeed				m_Feed;
	CPicCDLManager			*m_pDownLoader;
	CPicCFeedListCtrl		m_list_Feed;

private:
	bool					m_bInit;
	CPicCHTMLEventHandler	m_HTMLEventHandler;
	CLiteHTMLReader			m_HTMLReader;
	UINT					m_uLastDLDay;
	UINT					m_uTimerShowDownload;
	UINT					m_uTimerRefresh;

public:
	afx_msg void OnBnClickedPiccBtnChangedldir();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedPiccBtnAddnewfeed();
	afx_msg void OnBnClickedPiccBtnRefreshfeed();
	afx_msg void OnBnClickedPiccBtnRemovefeed();
	afx_msg void OnBnClickedPiccBtnDelaydl();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	virtual void OnCancel();
};

extern CPicCollectorDlg *g_pPicCollectorDlg;
