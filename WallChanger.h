#pragma once


// CWallChanger ��ܤ��

class CWallChanger : public CDialog
{
	DECLARE_DYNAMIC(CWallChanger)

public:
	CWallChanger(CWnd* pParent = NULL);   // �зǫغc�禡
	virtual ~CWallChanger();

// ��ܤ�����
	enum { IDD = IDD_WALLCHANGER };

	bool m_bInit;

private:
	CTreeCtrl m_FileTree;
	CListCtrl m_FileList;
	CStatic m_TreeStatic;
	CStatic m_ListStatic;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �䴩

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
};
