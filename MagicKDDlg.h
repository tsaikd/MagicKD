// MagicKDDlg.h : ���Y�� 
//

#pragma once

#include "KDTabCtrl.h"
#include "MagicKDConfigDlg.h"
#include "WallChanger.h"


// CMagicKDDlg ��ܤ��
class CMagicKDDlg : public CDialog
{
	DECLARE_MESSAGE_MAP()
// �غc
public:
	CMagicKDDlg(CWnd* pParent = NULL);	// �зǫغc�禡
	virtual ~CMagicKDDlg();

// ��ܤ�����
	enum { IDD = IDD_MAGICKD_DIALOG };

public:
	void MainConfigSyncTabEnable();

	typedef enum { eFunc_WallChanger = 1 } FuncList;
	void SetEnableFunc(FuncList eFunc, bool bEnable);

	CIni m_cIni;

protected:
	CKDTabCtrl m_MainTab;
	CMagicKDConfigDlg m_MainConfigDlg;
	CWallChanger *m_pWallChanger;

private:
	bool m_bInit;
	CRect m_rcTabWindow;

	void InitWindowRect();

// �{���X��@
protected:
	HICON m_hIcon;

	// ���ͪ��T�������禡
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �䴩
	virtual BOOL OnInitDialog();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	virtual void OnOK();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnTcnSelchangeMaintab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTcnSelchangingMaintab(NMHDR *pNMHDR, LRESULT *pResult);
};
