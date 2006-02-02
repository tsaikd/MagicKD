#pragma once
#include "Language.h"
#include "WallChangerEnablePicPath.h"
#include "KDListItem.h"

#include "WallListCtrl.h"

#define DIRSTATE_FIXEDDRIVE 0x01
class CWallDirListItem :
	public CKDListItem, public CKDIni
{
public:
	CWallDirListItem(HWND pParent, CIni *pIni, LPCTSTR sDirPath) :
		CKDListItem(2), m_bIsThreading(false), m_bCanThread(true), m_hWnd(pParent), m_pIni(pIni), m_bItemEnable(false),
		m_uDirState(0), m_bFindPath(false)
	{
		SetItemDirPath(sDirPath);
		if (m_pIni->IsKeyExist(_T("DirState"), GetItemDirPath())) {
			m_uDirState = m_pIni->GetUInt(_T("DirState"), GetItemDirPath(), 0);
		} else {
			UpdateDirState();
		}
		bool bGetFilePath = true;
		if (!PathFileExists(GetItemDirPath())) {
			if (m_uDirState & DIRSTATE_FIXEDDRIVE) {
				MessageBox(CResString(IDS_WALLDIRNOTEXIST), MB_OK|MB_ICONERROR);
				m_pIni->DeleteKey(_T("PicPath"), GetItemDirPath());
				m_pIni->DeleteEmptySection(_T("PicPath"));
			} else {
				int iRes = MessageBox(CResString(IDS_WALLDIRNOTEXIST_ONNOTFIXEDDRIVE), MB_OKCANCEL|MB_ICONWARNING);
				if (iRes == 2) {
					bGetFilePath = false;
				} else if ((iRes == 1) && (!PathFileExists(GetItemDirPath()))) {
					bGetFilePath = false;
					MessageBox(CResString(IDS_WALLDIRNOTEXIST_UPDATEPATHLATER), MB_OK|MB_ICONINFORMATION);
				}
			}
		}
		if (bGetFilePath) {
			if (m_pIni->IsKeyExist(_T("PicPath"), GetItemDirPath())) {
				m_pIni->GetArray(_T("PicPath"), GetItemDirPath(), &m_saPicPath);
				if (IsItemEnable())
					::g_slWallChangerEnablePicPath.AppendArray(m_saPicPath);
				SetItemFileFindNum(m_saPicPath.GetCount());
			} else {
				SetItemFileFindNum(-1);
				UpdateItemFileFindNum();
			}
			m_bFindPath = true;
		} else {
			SetItemFileFindNum(-1);
			m_bFindPath = false;
		}
	}
	virtual ~CWallDirListItem() {
		if (m_bIsThreading) {
			m_bCanThread = false;
			WaitForSingleObject(m_hThread, 1000);
		}

		if (IsItemEnable())
			::g_slWallChangerEnablePicPath.RemoveArray(m_saPicPath);

		SaveIni();
	}
	virtual void SaveIni() {
		if (!IsIniModify())
			return;

		if (m_saPicPath.GetCount()) {
			m_pIni->WriteArray(_T("PicPath"), GetItemDirPath(), &m_saPicPath);
		} else {
			m_pIni->DeleteKey(_T("PicPath"), GetItemDirPath());
			m_pIni->DeleteEmptySection(_T("PicPath"));
		}

		SetIniModify(false);
	}

	int MessageBox(LPCTSTR lpText, UINT uType = MB_OK) { return ::MessageBox(m_hWnd, lpText, _T("WallChanger"), uType); }
	CString GetItemDirPath() { return GetText(0); }
	void SetItemDirPath(LPCTSTR sPath) { SetText(0, sPath); }
	int GetItemFileFindNum() { return _ttoi(GetText(1)); }
	void SetItemFileFindNum(int iNum) { CString sBuf; sBuf.Format(_T("%d"), iNum); SetText(1, sBuf); }
	void UpdateItemFileFindNum() { if (!m_bIsThreading) CreateThread(); }
	CStringArray *GetItemPicPathArray() const { return (CStringArray * const)&m_saPicPath; }
	bool IsFindPath() { return m_bFindPath; }
	bool IsItemEnable() { return m_bItemEnable; }
	void SetItemEnable(bool bEnable) {
		if (m_bItemEnable == bEnable)
			return;
		m_bItemEnable = bEnable;
		if (m_bIsThreading)
			return;
		if (bEnable) {
			// Change from false to true
			::g_slWallChangerEnablePicPath.AppendArray(m_saPicPath);
		} else {
			// Change from true to false
			POSITION pos = ::g_imglCachePic.GetHeadPathPosition();
			POSITION posFind;
			CString sPicPath;
			while (pos) {
				sPicPath = ::g_imglCachePic.GetNextPath(pos);
				posFind = ::g_slWallChangerEnablePicPath.Find(sPicPath);
				if (posFind)
					::g_slWallChangerEnablePicPath.RemoveAt(posFind);
			}
			::g_slWallChangerEnablePicPath.RemoveArray(m_saPicPath);
		}
	}
	void UpdateDirState() {
		m_uDirState = 0;
		if (DRIVE_FIXED == RealDriveType(PathGetDriveNumber(GetItemDirPath()), 0))
			m_uDirState |= DIRSTATE_FIXEDDRIVE;
		m_pIni->WriteUInt(_T("DirState"), GetItemDirPath(), m_uDirState);
	}

	CStringArray m_saPicPath;
	HWND m_hWnd;
	CIni *m_pIni;

private:
	bool m_bItemEnable;
	bool m_bFindPath;
	UINT m_uDirState;

public:
	void CreateThread()
	{ VERIFY( m_hThread = ::CreateThread(NULL, 0, ThreadProc, (LPVOID) this, 0, &m_dwThreadId) ); }
	virtual DWORD ThreadProc() {
		m_bIsThreading = true;
		SetItemFileFindNum(FindPicPath());
		SetIniModify();
		if (IsItemEnable())
			::g_slWallChangerEnablePicPath.AppendArray(m_saPicPath);
		m_bFindPath = true;
		InvalidateRect(m_hWnd, NULL, TRUE);
		m_bIsThreading = false;
		return 0;
	}
protected:
	HANDLE m_hThread;
	DWORD m_dwThreadId;
private:
	static DWORD WINAPI ThreadProc(LPVOID pParam) {
		CWallDirListItem *pThis = (CWallDirListItem *) pParam;
		return pThis->ThreadProc();
	}

private:
	bool m_bIsThreading;
	bool m_bCanThread;
	int FindPicPath() {
		CStringList *pslPicPath = __pFindPicPath(GetItemDirPath() + _T("\\*.*"), true);
		if (!pslPicPath->IsEmpty()) {
			m_saPicPath.SetSize(pslPicPath->GetCount());
			int i = 0;
			POSITION pos = pslPicPath->GetHeadPosition();
			while (pos) {
				m_saPicPath[i++] = pslPicPath->GetAt(pos);
				printf("%s\n", pslPicPath->GetAt(pos));
				pslPicPath->GetNext(pos);
				if (!m_bCanThread) {
					delete pslPicPath;
					return -1;
				}
			}
		}
		delete pslPicPath;
		return m_saPicPath.GetCount();
	}

	CStringList *__pFindPicPath(LPCTSTR sPath, bool bRecursive = false) {
		CStringList *pslPicPath = new CStringList;
		CFileFind finder;
		BOOL bWorking = finder.FindFile(sPath);
		while (bWorking) {
			if (!m_bCanThread) {
				return pslPicPath;
			}
			bWorking = finder.FindNextFile();
			CString ttmmpp = finder.GetFilePath();
			if (finder.IsDots())
				continue;
			if (finder.IsDirectory()) {
				if (bRecursive) {
					CStringList *pslTmp = __pFindPicPath(finder.GetFilePath() + _T("\\*.*"), bRecursive);
					pslPicPath->AddTail(pslTmp);
					delete pslTmp;
				}
			} else if (__bMatchSupport(finder.GetFileName()))
				pslPicPath->AddTail(finder.GetFilePath());
		}
		return pslPicPath;
	}

	bool __bMatchSupport(LPCTSTR sPat) {
		if (PathMatchSpec(sPat, _T("*.jpg")))
			return true;
		else if (PathMatchSpec(sPat, _T("*.jpeg")))
			return true;
		else if (PathMatchSpec(sPat, _T("*.bmp")))
			return true;
		else if (PathMatchSpec(sPat, _T("*.gif")))
			return true;
		return false;
	}
};
#undef DIRSTATE_FIXEDDRIVE


class CWallDirListCtrl :
	public CWallListCtrl
{
public:
	CWallDirListCtrl();
	virtual ~CWallDirListCtrl();
	virtual void SaveIni();

	void Init(CIni *pIni, LPCTSTR sListClassName);
	void SetListClassName(LPCTSTR sName);
	CString GetListClassName();
	bool AddItem(LPCTSTR sDirPath);
	void UpdateSelectItemFileFindNum();
	bool IsFindPath();
	bool IsAllItemEnable();
	void SetAllItemEnable(bool bEnable);
private:
	bool m_bInit;
	bool m_bFindPath;
	bool m_bAllItemEnable;
	CString m_sListClassName;
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnDestroy();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnLvnDeleteitem(NMHDR *pNMHDR, LRESULT *pResult);
};
