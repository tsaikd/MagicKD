#include "StdAfx.h"
#include "WallChangerDlg.h"
#include "WallEnablePicList.h"

CWallEnablePicList::CWallEnablePicList()
	:	m_uCount(0), m_posNowList(0), m_iNowArray(-1)
{
}

CWallEnablePicList::~CWallEnablePicList()
{
}

bool CWallEnablePicList::AddEnableItem(CWallDirListItem *pItem)
{
	if (!pItem || !pItem->GetItemPicPathArray()->GetCount())
		return false;

	bool bRes = false;
	if (m_mux.Lock()) {
		if (m_lEnableItem.AddTail(pItem)) {
			bRes = true;
			m_uCount += pItem->GetItemFileFindNum();
		} else {
			bRes = false;
		}

		UpdateWallChangerDlg();
		m_mux.Unlock();
	}
	return bRes;
}

CString CWallEnablePicList::GetRandPic()
{
	CString sRes;
	if (m_mux.Lock()) {
		CWallDirListItem *pItem;
		CStringArray *psaEnable;

		int iRest = 0;
		if (m_uCount > 10000) {
			iRest = rand() % (m_uCount/10) + 10;
		} else if (m_uCount > 1000) {
			iRest = rand() % (m_uCount/5) + 10;
		} else if (m_uCount > 2) {
			iRest = rand() % (m_uCount/2) + 1;
		} else if (m_uCount == 2) {
			iRest = 1;
		} else if (m_uCount == 1) {
			if (m_posNowList) {
				pItem = m_lEnableItem.GetAt(m_posNowList);
				if (pItem) {
					psaEnable = pItem->GetItemPicPathArray();
					if (psaEnable && (psaEnable->GetCount() > m_iNowArray))
						sRes = psaEnable->GetAt(m_iNowArray);
				}
			}
			iRest = 0;
		} else if (m_uCount == 0) {
			iRest = 0;
		}

		int iArrayRest;
		while (iRest && m_lEnableItem.GetCount()) {
			if (!m_posNowList)
				m_posNowList = m_lEnableItem.GetHeadPosition();

			pItem = m_lEnableItem.GetAt(m_posNowList);
			if (pItem->IsOnFindPic()) {
				m_lEnableItem.GetNext(m_posNowList);
				continue;
			}
			psaEnable = pItem->GetItemPicPathArray();
			iArrayRest = psaEnable->GetCount() - m_iNowArray - 1;
			if (((iRest - iArrayRest) > 0) && iArrayRest) {
				iRest -= iArrayRest;
				m_lEnableItem.GetNext(m_posNowList);
				m_iNowArray = -1;
				continue;
			}
			sRes = _GetNextPic(iRest);
			if (PathFileExists(sRes)) {
				break;
			} else {
				pItem->UpdateItemFileFindNum();
				RemoveEnableItem(pItem);
				iRest = 1;
			}
		}

		m_mux.Unlock();
	}
	return sRes;
}

LPCTSTR CWallEnablePicList::GetNextPic()
{
	CString sRes;
	if (m_mux.Lock()) {
		sRes = _GetNextPic();
		m_mux.Unlock();
	}
	return sRes;
}

bool CWallEnablePicList::RemoveEnableItem(CWallDirListItem *pItem)
{
	if (!pItem)
		return false;

	bool bRes = false;
	if (m_mux.Lock()) {
		POSITION pos = m_lEnableItem.Find(pItem);
		if (pos) {
			if (pos == m_posNowList) {
				m_posNowList = 0;
				m_iNowArray = -1;
			}
			m_uCount -= pItem->GetItemPicPathArray()->GetCount();
			m_lEnableItem.RemoveAt(pos);
			bRes = true;
		} else {
			bRes = false;
		}

		UpdateWallChangerDlg();
		m_mux.Unlock();
	}
	return bRes;
}

bool CWallEnablePicList::RemoveFind(LPCTSTR sMatch)
{
	if (!sMatch)
		return NULL;

	bool bRes = false;
	if (m_mux.Lock()) {
		if (m_lEnableItem.IsEmpty()) {
			m_mux.Unlock();
			return NULL;
		}

		CWallDirListItem *pItem = NULL;
		CStringArray *psaEnable = NULL;
		if (m_posNowList) {
			pItem = m_lEnableItem.GetAt(m_posNowList);
			psaEnable = pItem->GetItemPicPathArray();
		}
		if (!pItem) {
			pItem = m_lEnableItem.GetHead();
			psaEnable = pItem->GetItemPicPathArray();
		}

		if ((m_iNowArray>=0) && (psaEnable->GetAt(m_iNowArray)==sMatch)) {
			psaEnable->RemoveAt(m_iNowArray);
			bRes = true;
		} else {
			int i, iCount;
			POSITION pos = m_lEnableItem.GetHeadPosition();
			while (pos) {
				pItem = m_lEnableItem.GetNext(pos);
				psaEnable = pItem->GetItemPicPathArray();

				iCount = psaEnable->GetCount();
				for (i=0 ; i<iCount ; i++) {
					if (psaEnable->GetAt(i) == sMatch) {
						psaEnable->RemoveAt(i);
						pos = 0;
						bRes = true;
						break;
					}
				}
			}
		}

		if (bRes) {
			pItem->SetItemFileFindNum(pItem->GetItemPicPathArray()->GetCount());
			pItem->Invalidate();
		}
		UpdateWallChangerDlg();
		m_mux.Unlock();
	}
	return bRes;
}

void CWallEnablePicList::UpdateWallChangerDlg()
{
	if (::g_pWallChangerDlg)
		SendNotifyMessage(::g_pWallChangerDlg->GetSafeHwnd(), WMU_UPDATE_TOTALNUM, 0, 0);
}

bool CWallEnablePicList::IsEmpty()
{
	bool bRes = false;
	if (m_mux.Lock()) {
		bRes = m_lEnableItem.IsEmpty() != FALSE;
		m_mux.Unlock();
	}
	return bRes;
}

ULONG CWallEnablePicList::GetCount()
{
	ULONG uRes = 0;
	if (m_mux.Lock()) {
		uRes = m_uCount;
		m_mux.Unlock();
	}
	return uRes;
}

LPCTSTR CWallEnablePicList::_GetNextPic(UINT uJump/* = 1*/)
{
	CString sRes;
	CWallDirListItem *pItem;
	CStringArray *psaEnable;
	while (m_uCount && m_lEnableItem.GetCount()) {
		if (!m_posNowList) {
			m_posNowList = m_lEnableItem.GetHeadPosition();
			m_iNowArray = -1;
		}
		pItem = m_lEnableItem.GetAt(m_posNowList);
		psaEnable = pItem->GetItemPicPathArray();
		while (!psaEnable->GetCount() && m_posNowList) {
			pItem = m_lEnableItem.GetNext(m_posNowList);
			psaEnable = pItem->GetItemPicPathArray();
			m_iNowArray = -1;
		}
		m_iNowArray += uJump;
		if (psaEnable->GetCount() > m_iNowArray) {
			sRes = psaEnable->GetAt(m_iNowArray);
			break;
		} else {
			m_iNowArray = -1;
			uJump = 1;
		}
	}
	return sRes;
}
