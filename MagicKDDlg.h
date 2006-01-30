// MagicKDDlg.h : ���Y�� 
//

#pragma once

#include "afxcmn.h"
#include "MainConfigDlg.h"
#include "Ini.h"
#include "KDTabCtrl.h"
#include "KDIni.h"

#include "WallChangerDlg.h"

// CMagicKDDlg ��ܤ��
class CMagicKDDlg : public CDialog, public CKDIni
{
// �غc
public:
	CMagicKDDlg(CWnd* pParent = NULL);	// �зǫغc�禡

// ��ܤ�����
	enum { IDD = IDD_MAGICKD_DIALOG };
	typedef enum { eFunc_WallChanger = 1 } FuncList;

public:
	void InitTabRect();
	void DoSize();
	void SetFuncEnable(FuncList eFunc, bool bEnable, bool bRedraw = true);
	virtual void SaveIni();

	CIni *m_pIni;
	CKDTabCtrl m_cMainTab;
	CMainConfigDlg m_cMainConfigDlg;
	CWallChangerDlg *m_pWallChangerDlg;
protected:
private:
	CRect m_rcMainTab;
	bool m_bIniModify;

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
};
