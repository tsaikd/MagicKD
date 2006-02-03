#pragma once
#include <afxcmn.h>
#include "KDListItem.h"

class CKDListCtrl : public CListCtrl
{
	DECLARE_MESSAGE_MAP()
public:
	CKDListCtrl();
	virtual ~CKDListCtrl();

	void DeleteSelectItem();
	void CancleAllSelected();
	LPARAM GetFirstSelectedItemLParam();
#if (_WIN32_IE >= 0x0300)
	void SetSelectItemCheckState(bool bCheck);
#endif

	int FindItemByText(LPCTSTR sText);
	LPARAM GetItemLParam(int nItem);
	BOOL SetItemSelected(int nItem);

	BOOL EnableToolTips(LPCTSTR sToolTip = NULL, BOOL bEnable = TRUE);
	BOOL IsEnableToolTips();
	void SetToolTips(LPCTSTR sToolTip);
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;

private:
	BOOL m_bEnableToolTip;
	CString m_sToolTip;

public:
	afx_msg void OnLvnGetdispinfo(NMHDR *pNMHDR, LRESULT *pResult);
};
