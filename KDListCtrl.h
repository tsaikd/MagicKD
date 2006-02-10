#pragma once
#include <afxcmn.h>
#include "KDListItem.h"

class CKDListCtrl : public CListCtrl
{
	DECLARE_MESSAGE_MAP()
public:
	CKDListCtrl();
	virtual ~CKDListCtrl();

	bool SwapItems(int iItem1, int iItem2);
	void MoveSelectedItems(int iDist);

/*
How To Use Drag:
	1. EnableDrag() in Initial
	2. Add to DefWindowProc:
		switch (message) {
		case WM_LBUTTONUP:
			if (m_bEnableDrag) {
				CPoint pt(lParam);
				int iPos = QueryDragPos(pt); // return the target position
				// Then do what you want
				// Begin Drag INFO in "m_nmlvBeginDrag"
			}
			break;
		}
*/
	bool EnableDrag(bool bEnable = true, COLORREF clrDragLine = RGB(255, 0, 0), COLORREF clrBackGround = RGB(255, 255, 255));
	//BOOL ScrollByItem(int iItems, bool bDrawDragMark = false, LPPOINT pPt = NULL);
	int QueryDragPos(const POINT &pt);
	void DrawDragMark(const POINT &pt, COLORREF clr = RGB(255, 0, 0), COLORREF bgClr = RGB(255, 255, 255));

	void DeleteSelectItem();
	void CancleAllSelected();
	LPARAM GetFirstSelectedItemLParam();
	int GetFirstSelectedItemNum();
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

protected:
	bool		m_bEnableDrag;
	NMLISTVIEW	m_nmlvBeginDrag;
	CImageList*	m_pImageList;

private:
	BOOL		m_bEnableToolTip;
	CString		m_sToolTip;
	COLORREF	m_clrDragLine;
	COLORREF	m_clrBackGround;

protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnLvnGetdispinfo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);
};
