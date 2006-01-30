#pragma once

#include "Ini.h"
#include "afxwin.h"

// CMainConfigDlg ��ܤ��

class CMainConfigDlg : public CDialog
{
public:
	CMainConfigDlg(CWnd* pParent = NULL);   // �зǫغc�禡
	virtual ~CMainConfigDlg();

public:
	void DoSize();
	bool IsIniModify();
	void SetIniModify(bool bModify = true);
	void SaveIni();

	CIni *m_pIni;
	CButton m_cbWallChanger;
protected:
private:
	bool m_bModify;

	DECLARE_DYNAMIC(CMainConfigDlg)
	DECLARE_MESSAGE_MAP()
// ��ܤ�����
	enum { IDD = IDD_MAGICCONFIGDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �䴩

public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedWallchangercheck();
};
