#pragma once

#include "WallChanger.h"
#include "KDTabCtrl.h"

// CMagicKDDlg ��ܤ��

class CMagicKDDlg : public CDialog
{
	DECLARE_DYNAMIC(CMagicKDDlg)

public:
	CMagicKDDlg(CWnd* pParent = NULL);   // �зǫغc�禡
	virtual ~CMagicKDDlg();

// ��ܤ�����
	enum { IDD = IDD_MAGICKDDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �䴩
	virtual BOOL OnInitDialog();

private:
	CWallChanger m_WallChanger;
	CKDTabCtrl m_MainTab;

	DECLARE_MESSAGE_MAP()
};
