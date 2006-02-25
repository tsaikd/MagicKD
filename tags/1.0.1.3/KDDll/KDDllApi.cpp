#include "stdafx.h"
#include "KDDll.h"
#include "KDMessageBox.h"

#include "KDDllApi.h"

CKDMessageBox::CKDMessageBox(LPCTSTR lpTitle, LPCTSTR lpHelpText, CStringArray &saBtnText, CWnd *pParentWnd/* = NULL*/,
	int iOnOkBtn/* = -1*/, int iOnCancelBtn/* = -1*/, UINT uFlag/* = 0*/)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_pMsgBox = new _CKDMessageBox(pParentWnd);
	_CKDMessageBox *pMsgBox = (_CKDMessageBox *)m_pMsgBox;

	pMsgBox->Init(lpTitle, lpHelpText, saBtnText, iOnOkBtn, iOnCancelBtn, uFlag);
	pMsgBox->DoModal();
}

CKDMessageBox::~CKDMessageBox()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (m_pMsgBox) {
		_CKDMessageBox *pMsgBox = (_CKDMessageBox *)m_pMsgBox;
		delete pMsgBox;
	}
}

CKDMessageBox::operator int ()
{
	_CKDMessageBox *pMsgBox = (_CKDMessageBox *)m_pMsgBox;

	if (pMsgBox)
		return pMsgBox->GetChoice();
	else
		return KDMSG_IDNOCHOICE;
}
