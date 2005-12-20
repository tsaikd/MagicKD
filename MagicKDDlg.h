// MagicKDDlg.h : ���Y�� 
//

#pragma once

#include "WallChanger.h"


// CMagicKDDlg ��ܤ��
class CMagicKDDlg : public CDialog
{
// �غc
public:
	CMagicKDDlg(CWnd* pParent = NULL);	// �зǫغc�禡

// ��ܤ�����
	enum { IDD = IDD_MAGICKD_DIALOG };

private:
	bool m_bInit;

// �{���X��@
protected:
	HICON m_hIcon;
	CTabCtrl m_MainTab;
	CWallChanger m_WallChanger;

	// ���ͪ��T�������禡
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �䴩
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMove(int x, int y);
protected:
	virtual void OnOK();
};
