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

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �䴩

	DECLARE_MESSAGE_MAP()
};
