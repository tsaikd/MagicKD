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

void CWallDirListItem::SaveIni()
{
	if (!IsIniModify())
		return;

	SetIniModify(false);
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

	if (IsItemEnable() && !m_bOnExit)
		::g_pWallEnablePicList->RemoveEnableItem(this);

	SaveIni();

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
	return GetText(0);
}

void CWallDirListItem::SetItemDirPath(LPCTSTR sPath)
{
	SetText(0, sPath);
}

int CWallDirListItem::GetItemFileFindNum()
{
	return _ttoi(GetText(1));
}

void CWallDirListItem::SetItemFileFindNum(int iNum)
{
	CString sBuf;
	sBuf.Format(_T("%d"), iNum);
	SetText(1, sBuf);
	Invalidate();
}

void CWallDirListItem::UpdateItemFileFindNum()
{
	SetItemFileFindNum(-1);
	SetOnFindPic();
	::g_pWallThreadFindPic->AddItem(this);
}

INT_PTR CWallDirListItem::SetItemPicPathArray(CStringArray &saPicPath)
{
	m_saPicPath.RemoveAll();
	return m_saPicPath.Append(saPicPath);
}

CStringArray *CWallDirListItem::GetItemPicPathArray()
{
	return &m_saPicPath;
}

// if not find, then return -1
// else return index
int CWallDirListItem::FindPath(CString &sPath)
{
	int iCount = m_saPicPath.GetCount();
	for (int i=0 ; i<iCount ; i++) {
		if (m_saPicPath[i] == sPath)
			return i;
	}
	return -1;
}

bool CWallDirListItem::RemoveAllPath(CString &sPath)
{
	bool bRes;
	int iIndex = FindPath(sPath);
	if (iIndex >= 0) {
		SetIniModify();
		bRes = true;
	} else {
		bRes = false;
	}
	while (iIndex >= 0) {
		m_saPicPath.RemoveAt(iIndex);
		iIndex = FindPath(sPath);
	}
	return bRes;
}

bool CWallDirListItem::IsDirOnFixDrive()
{
	if (m_uDirState & DIRSTATE_FIXEDDRIVE)
		return true;
	else
		return false;
}

bool CWallDirListItem::IsOnFindPic()
{
	return m_bOnFindPic;
}

bool CWallDirListItem::IsItemEnable()
{
	return m_bItemEnable;
}

void CWallDirListItem::SetItemEnable(bool bEnable)
{
	if (m_bItemEnable == bEnable)
		return;
	m_bItemEnable = bEnable;
	if (bEnable) {
		// Change from false to true
		if (!m_bOnFindPic)
			::g_pWallEnablePicList->AddEnableItem(this);
	} else {
		// Change from true to false
		::g_pWallEnablePicList->RemoveEnableItem(this);
	}
}

void CWallDirListItem::UpdateDirState()
{
	m_uDirState = 0;
	if (DRIVE_FIXED == RealDriveType(PathGetDriveNumber(GetItemDirPath()), 0))
		m_uDirState |= DIRSTATE_FIXEDDRIVE;
	m_pIni->WriteUInt(_T("DirState"), GetItemDirPath(), m_uDirState);
}

void CWallDirListItem::SetOnExit(bool bOnExit/* = true*/)
{
	m_bOnExit = bOnExit;
}

void CWallDirListItem::SetOnFindPic(bool bOnFindPic/* = true*/)
{
	m_bOnFindPic = bOnFindPic;
}

#undef DIRSTATE_FIXEDDRIVE
