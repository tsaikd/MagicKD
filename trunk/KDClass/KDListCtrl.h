#pragma once
#include <afxcmn.h>
#include "KDListItem.h"

class CKDListCtrl : public CListCtrl
{
	DECLARE_MESSAGE_MAP()
public:
	CKDListCtrl();
	virtual ~CKDListCtrl();
	virtual void Init();

	bool SwapItems(int iItem1, int iItem2);
	void MoveSelectedItems(int iPos);

/*
How To Use Drag:
	1. EnableDrag() in Initial
	2. Add to DefWindowProc:
		switch (message) {
		case WM_LBUTTONUP:
			if (m_bOnDraging) {
				CPoint pt(lParam);
				int iPos = QueryDragPos(pt); // return the target position
				// Then do what you want
				// Begin Drag INFO in "m_nmlvBeginDrag"
			}
			break;
		}
*/
	bool EnableDrag(bool bEnable = true, COLORREF clrDragLine = RGB(255, 0, 0), COLORREF clrBackGround = RGB(255, 255, 255));
	int QueryDragPos(const POINT &pt);
	void DrawDragMark(const POINT &pt, COLORREF clr = RGB(255, 0, 0), COLORREF bgClr = RGB(255, 255, 255));
protected:
	bool m_bOnDraging;
	NMLISTVIEW	m_nmlvBeginDrag;
	CImageList*	m_pImageList;
private:
	bool m_bEnableDrag;
	COLORREF	m_clrDragLine;
	COLORREF	m_clrBackGround;

public:
	void DeleteSelectItem();
	LPARAM GetFirstSelectedItemLParam();
	int GetFirstSelectedItemNum();
	void SetSelectItemCheckState(bool bCheck);

	int FindItemByText(LPCTSTR sText);
	bool SetItemSelected(int nItem, bool bSelected = true);
	void SetItemAllSelected(bool bSelected = true);

	bool EnableToolTips(LPCTSTR sToolTip = NULL, bool bEnable = true);
	bool IsEnableToolTips();
	void SetToolTips(LPCTSTR sToolTip);
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;

protected:
	CMenu		m_mContextMenu;

private:
	bool		m_bEnableToolTip;
	CString		m_sToolTip;

protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnLvnGetdispinfo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
};
