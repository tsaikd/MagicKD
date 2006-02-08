// MagicKDDlg.h : ���Y�� 
//

#pragma once

#include "afxcmn.h"
#include "MainConfigDlg.h"
#include "Ini.h"
#include "KDTabCtrl.h"
#include "KDTray.h"

#include "WallChangerDlg.h"

extern CKDTray theTray;

// CMagicKDDlg ��ܤ��
class CMagicKDDlg : public CDialog
{
// �غc
public:
	CMagicKDDlg(CWnd* pParent = NULL);	// �зǫغc�禡

// ��ܤ�����
	enum { IDD = IDD_MAGICKD };
	typedef enum { eFunc_WallChanger = 1 } FuncList;

public:
	void InitTabRect();
	void DoSize();
	void SetFuncEnable(FuncList eFunc, bool bEnable, bool bRedraw = true);

	bool SetTransparency(UINT uAlpha);

	CIni *m_pIni;
	CKDTabCtrl m_cMainTab;
	CMainConfigDlg m_cMainConfigDlg;
	CWallChangerDlg *m_pWallChangerDlg;
protected:
private:
	CRect m_rcMainTab;
	bool m_bIniModify;
	bool m_bVisiable;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �䴩


	DECLARE_MESSAGE_MAP()
// �{���X��@
protected:
	HICON m_hIcon;

	// ���ͪ��T�������禡
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
protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
