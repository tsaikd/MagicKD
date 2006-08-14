#include "stdafx.h"
#include "MagicKD.h"
#include "MagicKDEndDlg.h"


IMPLEMENT_DYNAMIC(CMagicKDEndDlg, CDialog)
CMagicKDEndDlg::CMagicKDEndDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMagicKDEndDlg::IDD, pParent), m_uTotalStep(0), m_posLastFindWnd(0)
{
}

CMagicKDEndDlg::~CMagicKDEndDlg()
{
}

BOOL CMagicKDEndDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_cProgressEnd.SetRange32(0, 65535);
	m_cProgressEnd.SetStep(65535);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX 屬性頁應傳回 FALSE
}

bool CMagicKDEndDlg::SignWnd(HWND hWnd, UINT uEndStep)
{
	if (!hWnd)
		return false;

	bool bRes = false;
	if (m_muxEndDlg.Lock()) {
		if (_FindSignWnd(hWnd))
			return false;

		SIGNWND signWnd = {hWnd, uEndStep};
		if (m_lSignWnd.AddTail(signWnd)) {
			m_uTotalStep += uEndStep;
			_UpdateStep();
			bRes = true;
		} else {
			bRes = false;
		}

		m_muxEndDlg.Unlock();
	}
	return bRes;
}

bool CMagicKDEndDlg::UnsignWnd(HWND hWnd)
{
	if (IsWindowVisible())
		return false;

	bool bRes = false;
	if (m_muxEndDlg.Lock()) {
		SIGNWND signWnd;
		POSITION pos = _FindSignWnd(hWnd);
		if (pos) {
			if (m_posLastFindWnd == pos)
				m_posLastFindWnd = 0;

			signWnd = m_lSignWnd.GetAt(pos);
			m_lSignWnd.RemoveAt(pos);
			m_uTotalStep -= signWnd.uEndStep;
			_UpdateStep();
			bRes = true;
		} else {
			bRes = false;
		}

		m_muxEndDlg.Unlock();
	}
	return bRes;
}

bool CMagicKDEndDlg::ProgressStepIt(HWND hWnd, LPCTSTR lpszText/* = NULL*/)
{
	if (!IsWindowVisible())
		return false;

	bool bRes = false;

	if (m_muxEndDlg.Lock()) {
		if (POSITION pos = _FindSignWnd(hWnd)) {
			LPSIGNWND pSignWnd = &m_lSignWnd.GetAt(pos);
			if (pSignWnd->uEndStep) {
				pSignWnd->uEndStep--;

				if (lpszText) {
					CString sText;
					m_editText.GetWindowText(sText);
					sText.AppendFormat(_T("%s\r\n"), lpszText);
					m_editText.SetWindowText(sText);
					m_editText.LineScroll(m_editText.GetLineCount());
					m_editText.RedrawWindow();
				}

				m_cProgressEnd.StepIt();
				bRes = true;
			}
		}

		m_muxEndDlg.Unlock();
	}
	ASSERT(bRes);
	return bRes;
}

void CMagicKDEndDlg::_UpdateStep()
{
	int nLower, nUpper;

	m_cProgressEnd.GetRange(nLower, nUpper);
	m_cProgressEnd.SetStep((nUpper-nLower) / m_uTotalStep);
}

POSITION CMagicKDEndDlg::_FindSignWnd(HWND hWnd)
{
	SIGNWND signWnd;

	if (m_posLastFindWnd) {
		signWnd = m_lSignWnd.GetAt(m_posLastFindWnd);
		if (hWnd == signWnd.hWnd)
			return m_posLastFindWnd;
	}

	m_posLastFindWnd = m_lSignWnd.GetHeadPosition();
	while (m_posLastFindWnd) {
		signWnd = m_lSignWnd.GetAt(m_posLastFindWnd);

		if (signWnd.hWnd == hWnd)
			return m_posLastFindWnd;

		m_lSignWnd.GetNext(m_posLastFindWnd);
	}

	return 0;
}

BEGIN_MESSAGE_MAP(CMagicKDEndDlg, CDialog)
END_MESSAGE_MAP()

void CMagicKDEndDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_END, m_cProgressEnd);
	DDX_Control(pDX, IDC_MAGICKDEND_EDIT_TEXT, m_editText);
}
