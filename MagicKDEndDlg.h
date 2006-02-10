#pragma once
#include "Resource.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "afxmt.h"
#include "KDEdit.h"

// CMagicKDEndDlg 對話方塊

class CMagicKDEndDlg : public CDialog
{
	DECLARE_DYNAMIC(CMagicKDEndDlg)
	DECLARE_MESSAGE_MAP()
public:
	enum { IDD = IDD_MAGICKD_END };
	CMagicKDEndDlg(CWnd* pParent = NULL);   // 標準建構函式
	virtual ~CMagicKDEndDlg();

	bool SignWnd(HWND hWnd, UINT uEndStep);
	bool UnsignWnd(HWND hWnd);
	bool ProgressStepIt(HWND hWnd, LPCTSTR lpszText = NULL);

private:
	void _UpdateStep();
	POSITION _FindSignWnd(HWND hWnd);

private:
	typedef struct tagSIGNWND {
		HWND hWnd;
		UINT uEndStep;
	} SIGNWND, *LPSIGNWND;

private:
	CMutex m_muxEndDlg;
	UINT m_uTotalStep;
	CList<tagSIGNWND> m_lSignWnd;
	POSITION m_posLastFindWnd;

	CProgressCtrl m_cProgressEnd;
	CEdit m_editText;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援
public:
	virtual BOOL OnInitDialog();
};

extern CMagicKDEndDlg *pTheAppEndDlg;
