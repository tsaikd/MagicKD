#include "StdAfx.h"
#include "Others.h"
#include "KDListCtrl.h"

CKDListCtrl::CKDListCtrl()
	:	m_bEnableToolTip(false), m_pImageList(NULL), m_bEnableDrag(false), m_bOnDraging(false)
{
}

CKDListCtrl::~CKDListCtrl()
{
}

void CKDListCtrl::Init()
{
}

bool CKDListCtrl::SwapItems(int iItem1, int iItem2)
{
	int iCount = GetItemCount();

	if (iItem1 > iCount)
		iItem1 = iCount - 1;
	if (iItem2 > iCount)
		iItem2 = iCount - 1;

	if (iItem1 == iItem2 || iItem1 < 0 || iItem2 < 0)
		return false;

	LVITEM lvItem1, lvItem2;

	lvItem1.mask = lvItem2.mask = LVIF_PARAM | LVIF_STATE;
	lvItem1.stateMask = lvItem2.stateMask = 0xFFFFFFFF;
	lvItem1.iItem = iItem1;
	lvItem2.iItem = iItem2;

	GetItem(&lvItem1);
	GetItem(&lvItem2);

	lvItem1.iItem = iItem2;
	lvItem2.iItem = iItem1;

	SetItem(&lvItem1);
	SetItem(&lvItem2);

	Update(iItem1);
	Update(iItem2);

	return true;
}

void CKDListCtrl::MoveSelectedItems(int iPos)
{
	if (!INRANGE(iPos, 0, GetItemCount()) || !GetSelectedCount())
		return;

	SetRedraw(FALSE);

	if (GetFirstSelectedItemNum() < iPos)
		iPos--;

	LVITEM lvItem = {0};
	POSITION pos = GetFirstSelectedItemPosition();
	int iFrom;
	while (pos) {
		iFrom = GetNextSelectedItem(pos);

		lvItem.stateMask = 0xFFFFFFFF;
		lvItem.pszText = LPSTR_TEXTCALLBACK;
		lvItem.iImage = I_IMAGECALLBACK;
		lvItem.iItem = iFrom;
		lvItem.mask = LVIF_PARAM | LVIF_STATE;

		GetItem(&lvItem);
		SetItemData(iFrom, 0); // prevent delete item data
		DeleteItem(iFrom);

		lvItem.iItem = iPos++;
		lvItem.mask = LVIF_PARAM | LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
		InsertItem(&lvItem);
		SetItemState(lvItem.iItem, lvItem.state, 0xFFFFFFFF);
	}

	SetRedraw(TRUE);
	Invalidate();
}

bool CKDListCtrl::EnableDrag(bool bEnable/* = true*/, COLORREF clrDragLine/* = RGB(255, 0, 0)*/, COLORREF clrBackGround/* = RGB(255, 255, 255)*/)
{
	m_clrDragLine = clrDragLine;
	m_clrBackGround = clrBackGround;
	m_bEnableDrag = bEnable;
	return m_bEnableDrag;
}

int CKDListCtrl::QueryDragPos(const POINT &pt)
{
	int iCount = GetItemCount(), iItem = 0;

	if (iCount > 0) {
		int iTop = GetTopIndex(), iHeight;
		CRect rcItem;
		GetItemRect(iTop, rcItem, LVIR_BOUNDS);
		iHeight = rcItem.Height();
		iItem = iTop + (pt.y - rcItem.top) / iHeight;
		if (((pt.y - rcItem.top) % iHeight) > (iHeight >> 1))
			iItem++;
		if (iItem > iCount)
			iItem = iCount;
		else if (iItem < 0)
			iItem = 0;
	}

	return iItem;
}

void CKDListCtrl::DrawDragMark(const POINT &pt, COLORREF clr/* = RGB(255, 0, 0)*/, COLORREF bgClr/* = RGB(255, 255, 255)*/)
{
	int iCount = GetItemCount(), iHeight;

	CRect rcItem;
	if (iCount > 0) {
		GetItemRect(0, rcItem, LVIR_BOUNDS);
		iHeight = rcItem.Height();
		iHeight *= (QueryDragPos(pt) - GetTopIndex());
		iHeight += rcItem.top;
		if (iHeight == 0)
			iHeight = 1;
	} else {
		iHeight = 1;
	}

	CPen pen(PS_SOLID, 2, clr), *pOldPen, penClean(PS_SOLID, 2, bgClr);
	CRect rc;
	GetClientRect(rc);
	CDC *pDC = GetDC();

	pOldPen = pDC->SelectObject(&penClean);
	int iTopIndex = GetTopIndex();
	int iButtonIndex = iTopIndex + GetCountPerPage();
	for (int i=iTopIndex ; i<iButtonIndex + 1 ; i++) {
		pDC->MoveTo(5, i * rcItem.Height() + rcItem.top);
		pDC->LineTo(rc.right - 5, i * rcItem.Height() + rcItem.top);
	}
	pDC->SelectObject(pOldPen);

	pOldPen = pDC->SelectObject(&pen);
	pDC->MoveTo(5, iHeight);
	pDC->LineTo(rc.right - 5, iHeight);
	pDC->SelectObject(pOldPen);

	ReleaseDC(pDC);
}

void CKDListCtrl::DeleteSelectItem()
{
	int nItem;
	POSITION pos = GetFirstSelectedItemPosition();

	while (pos) {
		nItem = GetNextSelectedItem(pos);
		DeleteItem(nItem);

		pos = GetFirstSelectedItemPosition();
	}
}

void CKDListCtrl::CancleAllSelected()
{
	int nItem;
	POSITION pos = GetFirstSelectedItemPosition();
	while (pos) {
		nItem = GetNextSelectedItem(pos);
		SetItemState(nItem, 0, LVIS_SELECTED);

		pos = GetFirstSelectedItemPosition();
	}
}

LPARAM CKDListCtrl::GetFirstSelectedItemLParam()
{
	if (!GetSelectedCount())
		return NULL;
	POSITION pos = GetFirstSelectedItemPosition();
	return (LPARAM)GetItemData(GetNextSelectedItem(pos));
}

int CKDListCtrl::GetFirstSelectedItemNum()
{
	if (!GetSelectedCount())
		return -1;
	POSITION pos = GetFirstSelectedItemPosition();
	return GetNextSelectedItem(pos);
}

void CKDListCtrl::SetSelectItemCheckState(bool bCheck)
{
	int nItem;
	POSITION pos = GetFirstSelectedItemPosition();
	while (pos) {
		nItem = GetNextSelectedItem(pos);
		ListView_SetCheckState(GetSafeHwnd(), nItem, bCheck);
		SetItemState(nItem, 0, LVIS_SELECTED);
	}
}

int CKDListCtrl::FindItemByText(LPCTSTR sText)
{
	LVFINDINFO itemInfo;
	itemInfo.flags = LVFI_STRING;
	itemInfo.psz = sText;
	return FindItem(&itemInfo);
}

bool CKDListCtrl::SetItemSelected(int nItem)
{
	return SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED) != FALSE;
}

bool CKDListCtrl::EnableToolTips(LPCTSTR sToolTip/* = NULL*/, bool bEnable/* = true*/)
{
	if (bEnable)
		SetToolTips(sToolTip);
	return CListCtrl::EnableToolTips(bEnable) != FALSE;
}

bool CKDListCtrl::IsEnableToolTips()
{
	return m_bEnableToolTip;
}

void CKDListCtrl::SetToolTips(LPCTSTR sToolTip)
{
	if (sToolTip)
		m_sToolTip = sToolTip;
}

INT_PTR CKDListCtrl::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	if (pTI && !m_sToolTip.IsEmpty()) {
		pTI->hwnd = m_hWnd;
		pTI->uId = (UINT_PTR) m_hWnd;
		pTI->uFlags |= TTF_ALWAYSTIP;
		pTI->lpszText = _tcsdup(m_sToolTip);
		GetClientRect(&pTI->rect);

		return GetDlgCtrlID();
	}
	return CListCtrl::OnToolHitTest(point, pTI);
}

BEGIN_MESSAGE_MAP(CKDListCtrl, CListCtrl)
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, OnLvnGetdispinfo)
	ON_NOTIFY_REFLECT(LVN_BEGINDRAG, OnLvnBegindrag)
	ON_NOTIFY_REFLECT(LVN_BEGINLABELEDIT, OnLvnBeginlabeledit)
END_MESSAGE_MAP()

void CKDListCtrl::OnLvnGetdispinfo(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);

	if (pDispInfo->item.mask & LVIF_TEXT) {
		CKDListItem *pItem = (CKDListItem *) pDispInfo->item.lParam;
		pDispInfo->item.pszText = (LPTSTR)pItem->GetText(pDispInfo->item.iSubItem);
	}

	*pResult = 0;
}

void CKDListCtrl::OnLvnBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);

	CEdit *pEdit = GetEditControl();
	if (pEdit)
		pEdit->SetWindowText(GetItemText(pDispInfo->item.iItem, pDispInfo->item.iSubItem));

	*pResult = 0;
}

void CKDListCtrl::OnLvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if (!m_bEnableDrag)
		return;

	CPoint ptZero(0, 0);
	if (m_pImageList) {
		delete m_pImageList;
		m_pImageList = NULL;
	} else {
		m_pImageList = CreateDragImage(pNMLV->iItem, &ptZero);
	}
	if (!m_pImageList)
		return;
	m_nmlvBeginDrag = *pNMLV;

	m_bOnDraging = true;
	m_pImageList->BeginDrag(0, ptZero);
	m_pImageList->DragShowNolock(TRUE);
	SetCapture();

	*pResult = 0;
}

LRESULT CKDListCtrl::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_MOUSEMOVE:
		if (m_bEnableDrag && m_bOnDraging && m_pImageList) {
			CPoint pt(lParam);
			DrawDragMark(pt, m_clrDragLine, m_clrBackGround);
			CPoint ptCur;
			GetCursorPos(&ptCur);
			m_pImageList->DragMove(ptCur);
		}
		break;
	case WM_LBUTTONUP:
		if (m_bEnableDrag && m_bOnDraging && m_pImageList) {
			m_pImageList->EndDrag();
			delete m_pImageList;
			m_pImageList = NULL;
			m_bOnDraging = false;
			Invalidate();
		}
		break;
	}

	return CListCtrl::DefWindowProc(message, wParam, lParam);
}
