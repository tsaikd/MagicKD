#include "StdAfx.h"
#include "Others.h"
#include "KDListCtrl.h"

enum {
	KDT_DRAGGING = 1
};

CKDListCtrl::CKDListCtrl()
	: m_bEnableToolTip(FALSE), m_pImageList(NULL)
{
}

CKDListCtrl::~CKDListCtrl()
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

void CKDListCtrl::MoveSelectedItems(int iDist)
{
	UINT uSelCount;
	if (!iDist || !(uSelCount = GetSelectedCount()))
		return;

//	bool bFreeData = (GetOptions() & KL_FREEDATA) != 0;

//	if (bFreeData)
//		SetOptions(GetOptions() & ~KL_FREEDATA);

	SetRedraw(FALSE);

	int iFrom, iTo, iCount = GetItemCount();
	POSITION pos = GetFirstSelectedItemPosition();
	iTo = GetNextSelectedItem(pos);

	LVITEM lvItem = {0};

	lvItem.stateMask = 0xFFFFFFFF;
	lvItem.pszText = LPSTR_TEXTCALLBACK;
	lvItem.iImage = I_IMAGECALLBACK;

	if (iDist > 0) {
		iFrom = iTo + uSelCount;
		for (;iDist > 0 && iFrom < iCount;iDist--) {
			lvItem.iItem = iFrom;
			lvItem.mask = LVIF_PARAM | LVIF_STATE;
			GetItem(&lvItem);
			DeleteItem(iFrom++);
			lvItem.iItem = iTo++;
			lvItem.mask = LVIF_PARAM | LVIF_TEXT | LVIF_IMAGE; // LVIF_STATE does not work when insertion. bug?
			InsertItem(&lvItem);
			SetItemState(lvItem.iItem, lvItem.state, 0xFFFFFFFF);
		}
	}
	else {
		iFrom = iTo - 1;
		iTo += uSelCount - 1;
		for (;iDist < 0 && iFrom >= 0;iDist++) {
			lvItem.iItem = iFrom;
			lvItem.mask = LVIF_PARAM | LVIF_STATE;
			GetItem(&lvItem);
			DeleteItem(iFrom--);
			lvItem.iItem = iTo--;
			lvItem.mask = LVIF_PARAM | LVIF_TEXT | LVIF_IMAGE; // LVIF_STATE does not work when insertion. bug?
			InsertItem(&lvItem);
			SetItemState(lvItem.iItem, lvItem.state, 0xFFFFFFFF);
		}
	}

	SetRedraw(TRUE);
	Invalidate();

//	if (bFreeData)
//		SetOptions(GetOptions() | KL_FREEDATA);
}

void CKDListCtrl::DeleteSelectItem()
{
	int nItem;
	POSITION pos;
	while (pos = GetFirstSelectedItemPosition()) {
		nItem = GetNextSelectedItem(pos);
		DeleteItem(nItem);
	}
}

void CKDListCtrl::CancleAllSelected()
{
	int nItem;
	POSITION pos;
	while (pos = GetFirstSelectedItemPosition()) {
		nItem = GetNextSelectedItem(pos);
		SetItemState(nItem, 0, LVIS_SELECTED);
	}
}

LPARAM CKDListCtrl::GetFirstSelectedItemLParam()
{
	POSITION pos = GetFirstSelectedItemPosition();
	return GetItemLParam(GetNextSelectedItem(pos));
}

#if (_WIN32_IE >= 0x0300)
void CKDListCtrl::SetSelectItemCheckState(bool bCheck)
{
	int nItem;
	POSITION pos = GetFirstSelectedItemPosition();
	while (pos) {
		nItem = GetNextSelectedItem(pos);
		ListView_SetCheckState(m_hWnd, nItem, bCheck);
		SetItemState(nItem, 0, LVIS_SELECTED);
	}
}
#endif

int CKDListCtrl::FindItemByText(LPCTSTR sText)
{
	LVFINDINFO itemInfo;
	itemInfo.flags = LVFI_STRING;
	itemInfo.psz = sText;
	return FindItem(&itemInfo);
}

LPARAM CKDListCtrl::GetItemLParam(int nItem)
{
	LVITEM lvitem = {0};
	lvitem.mask = LVIF_PARAM;
	lvitem.iItem = nItem;
	if (GetItem(&lvitem))
		return lvitem.lParam;
	else
		return NULL;
}

BOOL CKDListCtrl::SetItemSelected(int nItem)
{
	return SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);
}

BOOL CKDListCtrl::EnableToolTips(LPCTSTR sToolTip/* = NULL*/, BOOL bEnable/* = TRUE*/)
{
	if (bEnable)
		SetToolTips(sToolTip);
	return CListCtrl::EnableToolTips(bEnable);
}

BOOL CKDListCtrl::IsEnableToolTips()
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
	return -1;
}

//BOOL CKDListCtrl::ScrollByItem(int iItems, bool bDrawDragMark/* = false*/, LPPOINT pPt/* = NULL*/)
//{
//	int iCount;
//	if (!(iCount = GetItemCount()) || !iItems)
//		return FALSE;
//
//	CRect rcItem;
//	GetItemRect(0, rcItem, LVIR_BOUNDS);
//
//	int iTop = GetTopIndex();
//
//	if (iItems < 0 && iTop == 0 || iItems > 0 && iTop + iItems > iCount)
//		return FALSE;
//
//	if (bDrawDragMark)
//		RedrawWindow();
//
//	BOOL bRes = Scroll(CSize(0, rcItem.Height() * iItems));
//
//	if (bDrawDragMark)
//		DrawDragMark(*pPt);
//
//	return bRes;
//}
//
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

void CKDListCtrl::DrawDragMark(const POINT &pt, COLORREF clr/* = RGB(255, 0, 0)*/)
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

	CPen pen(PS_SOLID, 2, clr), *pOldPen, penClean(PS_SOLID, 2, RGB(255, 255, 255));
	CRect rc;
	GetClientRect(rc);
	CDC *pDC = GetDC();

	pOldPen = pDC->SelectObject(&penClean);
	for (int i=0 ; i<iCount+1 ; i++) {
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

BEGIN_MESSAGE_MAP(CKDListCtrl, CListCtrl)
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, OnLvnGetdispinfo)
	ON_NOTIFY_REFLECT(LVN_BEGINDRAG, OnLvnBegindrag)
END_MESSAGE_MAP()

void CKDListCtrl::OnLvnGetdispinfo(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);

	if (pDispInfo->item.mask & LVIF_TEXT) {
		TCHAR sText[MAX_PATH] = {0};
		CKDListItem *pItem = (CKDListItem *) pDispInfo->item.lParam;
		_stprintf(sText, _T("%s"), pItem->GetText(pDispInfo->item.iSubItem));
		pDispInfo->item.pszText = sText;
	}

	*pResult = 0;
}

void CKDListCtrl::OnLvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if (m_pImageList) {
		delete m_pImageList;
		m_pImageList = NULL;
	} else {
		m_pImageList = CreateDragImage(pNMLV->iItem, &CPoint(0, 0));
	}
	if (!m_pImageList)
		return;

	m_pImageList->BeginDrag(0, CPoint(0, 0));
	m_pImageList->DragShowNolock(TRUE);
//	m_pImageList->DragEnter(GetDesktopWindow(), pNMLV->ptAction);
	SetCapture();

	// TODO: 在此加入控制項告知處理常式程式碼
	*pResult = 0;
}

LRESULT CKDListCtrl::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_MOUSEMOVE:
		if (m_pImageList) {
//			SetRedraw(FALSE);
//			Invalidate(FALSE);
			CPoint pt(lParam);
					//ClientToScreen(&pt);
					//	CPoint cpt(pt);
					//	ScreenToClient(&cpt);
			DrawDragMark(pt);
			CPoint ptCur;
			GetCursorPos(&ptCur);
			m_pImageList->DragMove(ptCur);
//			Invalidate();
//			RedrawWindow();
		}
		break;
	case WM_LBUTTONUP:
		if (m_pImageList) {
			m_pImageList->EndDrag();
			delete m_pImageList;
			m_pImageList = NULL;
			Invalidate(TRUE);
//			SetRedraw(TRUE);
		}
		break;
	}

	// TODO: 在此加入特定的程式碼和 (或) 呼叫基底類別

	return CListCtrl::DefWindowProc(message, wParam, lParam);
}
