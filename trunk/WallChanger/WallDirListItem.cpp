#include "StdAfx.h"
#include "Resource.h"
#include "Language.h"
#include "WallEnablePicList.h"
#include "WallThreadFindPic.h"

#include "WallDirListItem.h"

#define DIRSTATE_FIXEDDRIVE 0x01

CWallDirListItem::CWallDirListItem()
	:	CKDListItem(2), m_bInit(false), m_bItemEnable(false), m_uDirState(0), m_bOnExit(false),
		m_bOnFindPic(false)
{
}

CWallDirListItem::~CWallDirListItem()
{
	Destroy();
}

void CWallDirListItem::Init(HWND pParent, CIni *pIni, LPCTSTR sDirPath, bool *pbShowDirLoadError)
{
	m_hWnd = pParent;
	m_pIni = pIni;
	SetItemDirPath(sDirPath);
	if (m_pIni->IsKeyExist(_T("DirState"), GetItemDirPath())) {
		m_uDirState = m_pIni->GetUInt(_T("DirState"), GetItemDirPath(), 0);
	} else {
		UpdateDirState();
	}

	SetItemFileFindNum(-1);

	if (PathFileExists(GetItemDirPath())) {
		UpdateItemFileFindNum();
	} else if (IsDirOnFixDrive()) {
		MessageBox(CResString(IDS_WALL_DIRNOTEXIST), MB_OK|MB_ICONERROR);
	} else {
		if (*pbShowDirLoadError) {
			int iRes;
			while (iRes = MessageBox(CResString(IDS_WALL_DIRNOTEXIST_ONNOTFIXEDDRIVE), MB_RETRYCANCEL|MB_ICONWARNING)) {
				if (iRes == IDCANCEL) {
					MessageBox(CResString(IDS_WALL_DIRNOTEXIST_UPDATEPATHLATER), MB_OK|MB_ICONINFORMATION);
					break;
				} else if ((iRes == IDRETRY) && (PathFileExists(GetItemDirPath()))) {
					break;
				}
			}
		}
		UpdateItemFileFindNum();
	}

	m_bInit = true;
}

void CWallDirListItem::Destroy()
{
	if (!m_bInit)
		return;

	if (!m_bOnExit) {
		if (IsOnFindPic())
			g_pWallThreadFindPic->RemoveItem(this);

		if (IsItemEnable())
			g_pWallEnablePicList->RemoveEnableItem(this);
	}

	m_bInit = false;
}

int CWallDirListItem::MessageBox(LPCTSTR lpText, UINT uType/* = MB_OK*/)
{
	return ::MessageBox(m_hWnd, lpText, _T("WallChanger"), uType);
}

BOOL CWallDirListItem::Invalidate(BOOL bErase/* = TRUE*/)
{
	return InvalidateRect(m_hWnd, NULL, bErase);
}

CString CWallDirListItem::GetItemDirPath()
{
	CString sRes;
	if (m_mux.Lock()) {
		sRes = GetText(0);

		m_mux.Unlock();
	}
	return sRes;
}

void CWallDirListItem::SetItemDirPath(LPCTSTR sPath)
{
	if (m_mux.Lock()) {
		SetText(0, sPath);

		m_mux.Unlock();
	}
}

int CWallDirListItem::GetItemFileFindNum()
{
	int iRes = -1;
	if (m_mux.Lock()) {
		iRes = _ttoi(GetText(1));

		m_mux.Unlock();
	}
	return iRes;
}

void CWallDirListItem::SetItemFileFindNum(int iNum)
{
	if (m_mux.Lock()) {
		CString sBuf;
		sBuf.Format(_T("%d"), iNum);
		SetText(1, sBuf);
		Invalidate();

		m_mux.Unlock();
	}
}

void CWallDirListItem::UpdateItemFileFindNum()
{
	if (m_mux.Lock()) {
		SetItemFileFindNum(-1);
		if (IsItemEnable())
			::g_pWallEnablePicList->RemoveEnableItem(this);
		SetOnFindPic();
		::g_pWallThreadFindPic->AddItem(this);

		m_mux.Unlock();
	}
}

INT_PTR CWallDirListItem::SetItemPicPathArray(CStringArray &saPicPath)
{
	INT_PTR iRes = 0;
	if (m_mux.Lock()) {
		m_saPicPath.RemoveAll();
		iRes = m_saPicPath.Append(saPicPath);

		m_mux.Unlock();
	}
	return iRes;
}

CStringArray *CWallDirListItem::GetItemPicPathArray()
{
	CStringArray *pRes = NULL;
	if (m_mux.Lock()) {
		pRes = &m_saPicPath;

		m_mux.Unlock();
	}
	return pRes;
}

// if not find, then return -1
// else return index
int CWallDirListItem::FindPath(CString &sPath)
{
	int iRes = -1;
	if (m_mux.Lock()) {
		int iCount = m_saPicPath.GetCount();
		for (int i=0 ; i<iCount ; i++) {
			if (m_saPicPath[i] == sPath) {
				iRes = i;
				break;
			}
		}

		m_mux.Unlock();
	}
	return iRes;
}

bool CWallDirListItem::RemoveAllPath(CString &sPath)
{
	bool bRes = false;
	if (m_mux.Lock()) {
		int iIndex = FindPath(sPath);
		if (iIndex >= 0) {
			bRes = true;
		} else {
			bRes = false;
		}
		while (iIndex >= 0) {
			m_saPicPath.RemoveAt(iIndex);
			iIndex = FindPath(sPath);
		}

		m_mux.Unlock();
	}
	return bRes;
}

bool CWallDirListItem::IsDirOnFixDrive()
{
	bool bRes = false;
	if (m_mux.Lock()) {
		if (m_uDirState & DIRSTATE_FIXEDDRIVE)
			bRes = true;
		else
			bRes = false;

		m_mux.Unlock();
	}
	return bRes;
}

bool CWallDirListItem::IsOnFindPic()
{
	bool bRes = false;
	if (m_mux.Lock()) {
		bRes = m_bOnFindPic;

		m_mux.Unlock();
	}
	return bRes;
}

bool CWallDirListItem::IsItemEnable()
{
	bool bRes = false;
	if (m_mux.Lock()) {
		bRes = m_bItemEnable;

		m_mux.Unlock();
	}
	return bRes;
}

void CWallDirListItem::SetItemEnable(bool bEnable)
{
	if (m_bItemEnable == bEnable)
		return;

	if (m_mux.Lock()) {
		m_bItemEnable = bEnable;
		if (bEnable) {
			// Change from false to true
			if (!m_bOnFindPic)
				g_pWallEnablePicList->AddEnableItem(this);
		} else {
			// Change from true to false
			g_pWallEnablePicList->RemoveEnableItem(this);
		}

		m_mux.Unlock();
	}
}

void CWallDirListItem::UpdateDirState()
{
	if (m_mux.Lock()) {
		m_uDirState = 0;
		if (DRIVE_FIXED == RealDriveType(PathGetDriveNumber(GetItemDirPath()), 0))
			m_uDirState |= DIRSTATE_FIXEDDRIVE;
		m_pIni->WriteUInt(_T("DirState"), GetItemDirPath(), m_uDirState);

		m_mux.Unlock();
	}
}

void CWallDirListItem::SetOnExit(bool bOnExit/* = true*/)
{
	if (m_mux.Lock()) {
		m_bOnExit = bOnExit;

		m_mux.Unlock();
	}
}

void CWallDirListItem::SetOnFindPic(bool bOnFindPic/* = true*/)
{
	if (m_mux.Lock()) {
		m_bOnFindPic = bOnFindPic;

		m_mux.Unlock();
	}
}

void CWallDirListItem::On_FileAdded(const CString &strFileName)
{
	if (!::g_pWallThreadFindPic->IsMatchSupport(strFileName))
		return;

	m_saPicPath.Add(strFileName);
	SetItemFileFindNum(m_saPicPath.GetCount());
	if (IsItemEnable())
		::g_pWallEnablePicList->ModifyCount(1);
}

void CWallDirListItem::On_FileNameChanged(const CString &strOldFileName, const CString &strNewFileName)
{
	if (!::g_pWallThreadFindPic->IsMatchSupport(strNewFileName))
		return;

	int i, iCount = m_saPicPath.GetCount();
	for (i=0 ; i<iCount ; i++) {
		if (m_saPicPath[i] == strOldFileName) {
			m_saPicPath[i] = strNewFileName;
			return;
		}
	}
}

void CWallDirListItem::On_FileRemoved(const CString &strFileName)
{
	if (!::g_pWallThreadFindPic->IsMatchSupport(strFileName))
		return;

	int i, iCount = m_saPicPath.GetCount();
	for (i=0 ; i<iCount ; i++) {
		if (m_saPicPath[i] == strFileName) {
			m_saPicPath.RemoveAt(i);
			SetItemFileFindNum(m_saPicPath.GetCount());
			if (IsItemEnable())
				::g_pWallEnablePicList->ModifyCount(-1);
			return;
		}
	}
}

#undef DIRSTATE_FIXEDDRIVE
