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

bool CWallEnablePicList::AddEnableItem(CStringArray *pItem)
{
	if (!pItem || !pItem->GetCount())
		return false;

	bool bRes = false;
	if (m_mux.Lock()) {
		if (m_lEnableItem.AddTail(pItem)) {
			bRes = true;
			m_uCount += pItem->GetCount();
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
				psaEnable = m_lEnableItem.GetAt(m_posNowList);
				if (psaEnable && (psaEnable->GetCount() > m_iNowArray))
					sRes = psaEnable->GetAt(m_iNowArray);
			}
			iRest = 0;
		} else if (m_uCount == 0) {
			iRest = 0;
		}

		int iArrayRest;
		while (iRest) {
			if (!m_posNowList)
				m_posNowList = m_lEnableItem.GetHeadPosition();

			psaEnable = m_lEnableItem.GetAt(m_posNowList);
			iArrayRest = psaEnable->GetCount() - m_iNowArray - 1;
			if (((iRest - iArrayRest) > 0) && iArrayRest) {
				iRest -= iArrayRest;
				m_lEnableItem.GetNext(m_posNowList);
				m_iNowArray = -1;
				continue;
			}
			sRes = _GetNextPic(iRest);
			iRest = 0;
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

bool CWallEnablePicList::RemoveEnableItem(CStringArray *pItem)
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
			m_uCount -= pItem->GetCount();
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

CStringArray *CWallEnablePicList::RemoveFind(LPCTSTR sMatch)
{
	if (!sMatch)
		return NULL;

	CStringArray *psaRes = NULL;
	if (m_mux.Lock()) {
		if (m_lEnableItem.IsEmpty()) {
			m_mux.Unlock();
			return NULL;
		}

		CStringArray *psaEnable = NULL;
		if (m_posNowList)
			psaEnable = m_lEnableItem.GetAt(m_posNowList);
		if (!psaEnable)
			psaEnable = m_lEnableItem.GetHead();

		if ((m_iNowArray>=0) && (psaEnable->GetAt(m_iNowArray)==sMatch)) {
			psaEnable->RemoveAt(m_iNowArray);
			psaRes = psaEnable;
		} else {
			int i, iCount;
			POSITION pos = m_lEnableItem.GetHeadPosition();
			while (pos) {
				psaEnable = m_lEnableItem.GetNext(pos);

				iCount = psaEnable->GetCount();
				for (i=0 ; i<iCount ; i++) {
					if (psaEnable->GetAt(i) == sMatch) {
						psaEnable->RemoveAt(i);
						pos = 0;
						psaRes = psaEnable;
						break;
					}
				}
			}
		}

		UpdateWallChangerDlg();
		m_mux.Unlock();
	}
	return psaRes;
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
	CStringArray *psaEnable;
	while (m_uCount && m_lEnableItem.GetCount()) {
		if (!m_posNowList) {
			m_posNowList = m_lEnableItem.GetHeadPosition();
			m_iNowArray = -1;
		}
		psaEnable = m_lEnableItem.GetAt(m_posNowList);
		while (!psaEnable->GetCount() && m_posNowList) {
			psaEnable = m_lEnableItem.GetNext(m_posNowList);
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
