#pragma once

class CKDSimpleDlg : public CDialog
{
	DECLARE_DYNAMIC(CKDSimpleDlg)
	DECLARE_MESSAGE_MAP()
public:
	enum { IDD = 0 };

	CKDSimpleDlg(CWnd *pParent = NULL);
	virtual ~CKDSimpleDlg();

	virtual bool Create(CWnd *pParent = NULL, DWORD dwStyle = DS_SETFONT|WS_POPUP|WS_BORDER|WS_CAPTION, DWORD dwExStyle = 0);

	void SetWindowSize(int cx, int cy);

protected:
	CFont m_fontWin;
	CSize m_sizeWin;

protected:
	virtual BOOL OnInitDialog();
};
