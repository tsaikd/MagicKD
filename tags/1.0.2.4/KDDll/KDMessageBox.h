#pragma once
#include "afxwin.h"
#include "afxtempl.h"
#include "Resource.h"

// CKDMessageBox 對話方塊

class _CKDMessageBox : public CDialog
{
	DECLARE_DYNAMIC(_CKDMessageBox)
	DECLARE_MESSAGE_MAP()
public:
	enum { IDD = IDD_MSGBOX };
	_CKDMessageBox(CWnd* pParent = NULL);
	virtual ~_CKDMessageBox();

	void Init(LPCTSTR lpTitle, LPCTSTR lpHelpText, CStringArray &saBtnText,
		int iOnOkBtn = -1, int iOnCancelBtn = -1, UINT uFlag = 0);
	int GetChoice();

private:
	int				m_iChoice;
	int				m_iOnOkBtn;
	int				m_iOnCancelBtn;
	UINT			m_uFlag;
	UINT			m_uBtnBaseID;
	CString			m_sTitle;
	CString			m_sHelpText;
	CStringArray*	m_psaBtnText;

	CStatic			m_staticHelpText;
	CArray<CButton*, CButton*> m_aBtn;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
protected:
	virtual void OnOK();
	virtual void OnCancel();
};
