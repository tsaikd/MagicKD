#pragma once

#include "MagicKD.h"
#include "afxwin.h"
// CMagicKDConfigDlg ��ܤ��

class CMagicKDConfigDlg : public CDialog
{
	DECLARE_DYNAMIC(CMagicKDConfigDlg)
	DECLARE_MESSAGE_MAP()

public:
	CMagicKDConfigDlg(CWnd* pParent = NULL);   // �зǫغc�禡
	virtual ~CMagicKDConfigDlg();

// ��ܤ�����
	enum { IDD = IDD_MAIN_CONFIG_DIALOG };

public:
	CButton m_cbWallChanger;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �䴩
	virtual void OnOK();
	virtual void OnCancel();
public:
	afx_msg void OnBnClickedCheckWallchanger();
};
