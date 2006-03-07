#include "stdafx.h"
#include "KDDllApi.h"

#include "KDMessageBox.h"

IMPLEMENT_DYNAMIC(_CKDMessageBox, CDialog)
_CKDMessageBox::_CKDMessageBox(CWnd* pParent /*=NULL*/)
	:	CDialog(_CKDMessageBox::IDD, pParent), m_uBtnBaseID(300), m_iChoice(0), m_uFlag(0),
		m_iOnOkBtn(-1), m_iOnCancelBtn(-1)
{
}

_CKDMessageBox::~_CKDMessageBox()
{
}

void _CKDMessageBox::Init(LPCTSTR lpTitle, LPCTSTR lpHelpText, CStringArray &saBtnText,
						  int iOnOkBtn/* = -1*/, int iOnCancelBtn/* = -1*/, UINT uFlag/* = 0*/)
{
	m_sTitle = lpTitle;
	m_sHelpText = lpHelpText;
	m_psaBtnText = &saBtnText;
	m_iOnOkBtn = iOnOkBtn;
	m_iOnCancelBtn = iOnCancelBtn;
	m_uFlag = uFlag;
}

BOOL _CKDMessageBox::OnInitDialog()
{
	CDialog::OnInitDialog();

	UINT uBtnMinWidthPix = 50;
	UINT uMarginLRPix = 20;
	UINT uMarginBPic = 20;
	UINT uBtnDistPix = 5;
	UINT uBtnTextMarginLRPix = 10;

	SetWindowText(m_sTitle);
	m_staticHelpText.SetWindowText(m_sHelpText);

	m_aBtn.SetSize(m_psaBtnText->GetSize());
	INT_PTR i, iCount = m_psaBtnText->GetSize();

	CRect rcWin;
	GetWindowRect(rcWin);
	CRect rcBox;
	GetClientRect(rcBox);

	CButton *pBtn;
	CSize sizeBtn(uBtnMinWidthPix, 25);	// default Button size
	CDC dcBtn;
	CRect rcBtn(0, 0, sizeBtn.cx, sizeBtn.cy);
	rcBtn.MoveToX((rcBox.Width() - 55*iCount) >> 1);
	rcBtn.MoveToY(rcBox.Height() - uMarginBPic - sizeBtn.cy);
	if (rcBtn.left <= (int)uMarginLRPix)
		rcBtn.MoveToX(uMarginLRPix);
	if (iCount) {
		i = 0;

		rcBtn.right = rcBtn.left + uBtnMinWidthPix;
		m_aBtn[i] = pBtn = new CButton;
		pBtn->Create(m_psaBtnText->GetAt(i), WS_VISIBLE | BS_BOTTOM | BS_CENTER, rcBtn, this, m_uBtnBaseID + i);
		sizeBtn = pBtn->GetDC()->GetTextExtent(m_psaBtnText->GetAt(i));
		if (rcBtn.right < (rcBtn.left + sizeBtn.cx + uBtnTextMarginLRPix))
			rcBtn.right = rcBtn.left + sizeBtn.cx + uBtnTextMarginLRPix;
		pBtn->MoveWindow(rcBtn, FALSE);
	}
	for (i=1 ; i<iCount ; i++) {
		rcBtn.MoveToX(rcBtn.right + uBtnDistPix);
		if (rcBtn.right > (rcBox.right - uMarginLRPix)) {
			rcWin.right += rcBtn.right + uMarginLRPix - rcBox.right;
			rcBox.right = rcBtn.right + uMarginLRPix;
		}

		rcBtn.right = rcBtn.left + uBtnMinWidthPix;
		m_aBtn[i] = pBtn = new CButton;
		pBtn->Create(m_psaBtnText->GetAt(i), WS_VISIBLE | BS_BOTTOM | BS_CENTER, rcBtn, this, m_uBtnBaseID + i);
		sizeBtn = pBtn->GetDC()->GetTextExtent(m_psaBtnText->GetAt(i));
		if (rcBtn.right < (rcBtn.left + sizeBtn.cx + uBtnTextMarginLRPix))
			rcBtn.right = rcBtn.left + sizeBtn.cx + uBtnTextMarginLRPix;
		pBtn->MoveWindow(rcBtn, FALSE);
	}
	MoveWindow(rcWin, FALSE);

	if (m_iOnOkBtn) {
		pBtn = m_aBtn[m_iOnOkBtn-1];
		pBtn->SetButtonStyle(pBtn->GetButtonStyle() | BS_DEFPUSHBUTTON, FALSE);
		pBtn->SetFocus();
		return FALSE;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX 屬性頁應傳回 FALSE
}

void _CKDMessageBox::OnDestroy()
{
	CDialog::OnDestroy();

	INT_PTR i, iCount = m_aBtn.GetCount();
	CButton *pBtn;
	for (i=0 ; i<iCount ; i++) {
		pBtn = m_aBtn.GetAt(i);
		if (pBtn)
			delete pBtn;
	}
	m_aBtn.RemoveAll();
}

int _CKDMessageBox::GetChoice()
{
	return m_iChoice;
}

BEGIN_MESSAGE_MAP(_CKDMessageBox, CDialog)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

void _CKDMessageBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSGBOX_STATIC_HELPTEXT, m_staticHelpText);
}

void _CKDMessageBox::OnOK()
{
	if (m_iOnOkBtn) {
		m_iChoice = m_iOnOkBtn;
		CDialog::OnOK();
	}
}

void _CKDMessageBox::OnCancel()
{
	if (m_uFlag & KDMSG_F_CANCANCEL) {
		if (m_iOnCancelBtn)
			m_iChoice = m_iOnCancelBtn;
		CDialog::OnCancel();
	}
}

LRESULT _CKDMessageBox::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_COMMAND:
		{
			int i, iCount = m_aBtn.GetCount();
			CButton *pBtn;
			for (i=0 ; i<iCount ; i++) {
				pBtn = m_aBtn.GetAt(i);
				if (wParam == pBtn->GetDlgCtrlID()) {
					m_iChoice = i + 1;
					CDialog::OnOK();
					break;
				}
			}
		}
		break;
	}

	return CDialog::DefWindowProc(message, wParam, lParam);
}
