#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "CxImage/CxImage/xImage.h"
#include "KDThread.h"
#include "FindDFListCtrl.h"
#include "FindDFTreeCtrl.h"
#include "FindStaticShowPic.h"

class CFindDupFileDlg : public CDialog, public CKDThread
{
	DECLARE_DYNAMIC(CFindDupFileDlg)
	DECLARE_MESSAGE_MAP()
public:
	enum { IDD = IDD_MAGICKD_FINDDUPFILE };
	CFindDupFileDlg(CWnd* pParent = NULL);
	virtual ~CFindDupFileDlg();
	virtual DWORD ThreadProc();

	void DoSize();

	void EnableToolTips(bool bEnable = true);
	void SetFindResultTreeCheck(bool bCheck = true);
//	void SetFindResultTreeExpand(bool bExpand = true);

	CIni m_cIni;

	CFindStaticShowPic m_static_ShowPic;
	CFindDFListCtrl m_list_FindDupFileList;
	CFindDFTreeCtrl m_tree_FindResult;

private:
	void _FindAllFileAndAddToArray(void *pArray, LPCTSTR sPath);

	bool m_bInit;

protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnCancel();
	virtual void OnOK();
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedFindBtnEnabletooltip();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedFindBtnStartfind();
	afx_msg void OnTvnSelchangedFindTreeFindresult(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedFindBtnSelectdup();
	afx_msg void OnBnClickedFindBtnSelectnone();
	afx_msg void OnBnClickedFindBtnDeletedup();
};
