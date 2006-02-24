#pragma once

#include "afxcmn.h"
#include "Ini.h"
#include "KDTabCtrl.h"
#include "KDTray.h"

#include "MainConfigDlg.h"
#include "FindDupFileDlg.h"

#include "WallChangerDlg.h"

extern CKDTray *pTheTray;

class CMagicKDDlg : public CDialog
{
	DECLARE_DYNAMIC(CMagicKDDlg)
	DECLARE_MESSAGE_MAP()
public:
	enum { IDD = IDD_MAGICKD };
	CMagicKDDlg(CWnd* pParent = NULL);
	virtual ~CMagicKDDlg();

	typedef enum {
		eFunc_WallChanger = 1,
		eFunc_FindDupFile
	} FuncList;

public:
	void DoSize();
	void SetFuncEnable(FuncList eFunc, bool bEnable, bool bRedraw = true);

	bool SetTransparency(BYTE uAlpha);
	BOOL ShowWindow(int nCmdShow);

	CKDTabCtrl m_cMainTab;
	CMainConfigDlg m_cMainConfigDlg;
	CWallChangerDlg *m_pWallChangerDlg;
	CFindDupFileDlg *m_pFindDupFileDlg;
protected:
private:
	bool m_bInit;
	bool m_bIniModify;
	bool m_bVisiable;
	CRect m_rcMainTab;

protected:
	HICON m_hIcon;

	virtual void DoDataExchange(CDataExchange* pDX);
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	virtual void OnCancel();
public:
	afx_msg void OnDestroy();
	afx_msg void OnTcnSelchangeMaintab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTcnSelchangingMaintab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
};
