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

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �䴩


// �{���X��@
protected:
	HICON m_hIcon;
	CTabCtrl m_MainTab;
	CWallChanger m_WallChanger;

	// ���ͪ��T�������禡
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
