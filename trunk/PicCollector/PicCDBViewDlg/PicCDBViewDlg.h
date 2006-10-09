#pragma once
#include "KDThread.h"
#include "PicCDBVListCtrl.h"
#include "PicCDBVWaitDlg.h"

class CPicCDBViewDlg : public CDialog, public CKDThread
{
	DECLARE_DYNAMIC(CPicCDBViewDlg)
	DECLARE_MESSAGE_MAP()
public:
	enum { IDD = IDD_MAGICKD_PICCDBVIEW };
	enum {
		ID_TABLE_NULL,
		ID_TABLE_UNDL,
		ID_TABLE_DLFAILED,
		ID_TABLE_DLOVER
	};
	CPicCDBViewDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPicCDBViewDlg();

	virtual BOOL OnInitDialog();
	void Localize();
	CString GetTableNameFromID(int nID);

	CPicCDBVListCtrl m_list_Item;

private:
	enum {
		THREAD_FUNC_NULL,
		THREAD_FUNC_COMBOCHANGE,
		THREAD_FUNC_DELRECODE
	};
	virtual DWORD ThreadProc() {
		switch (m_iThreadFunc) {
		case THREAD_FUNC_COMBOCHANGE:
			_ComboChanged();
			break;
		case THREAD_FUNC_DELRECODE:
			_DeleteRecode();
			break;
		}
		return 0;
	}
	void _ComboChanged();
	void _DeleteRecode();

	CPicCDBVWaitDlg m_dlgWait;
	bool m_bStopWait;
	int m_iThreadFunc;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
public:
	afx_msg void OnCbnSelchangePiccDbvComboTbname();
	afx_msg void OnEnSetfocusPiccDbvEditUrl();
	afx_msg void OnEnSetfocusPiccDbvEditLocalpath();
	afx_msg void OnBnClickedPiccDbvBtnReload();
	afx_msg void OnBnClickedPiccDbvBtnDelrecode();
	afx_msg void OnBnClickedPiccDbvBtnLpage();
	afx_msg void OnBnClickedPiccDbvBtnRpage();
	afx_msg void OnBnClickedPiccDbvBtnExport();
};
