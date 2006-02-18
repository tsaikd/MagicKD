#pragma once
#include "afxmt.h"
#include "Ini.h"
#include "KDIni.h"
#include "KDListItem.h"

class CWallDirListItem
	:	public CKDListItem, public CKDIni
{
public:
	CWallDirListItem();
	virtual ~CWallDirListItem();
	virtual void SaveIni();

	void Init(HWND pParent, CIni *pIni, LPCTSTR sDirPath, bool *pbShowDirLoadError);
	void Destroy();

	int MessageBox(LPCTSTR lpText, UINT uType = MB_OK);
	BOOL Invalidate(BOOL bErase = TRUE);

	CString GetItemDirPath();
	void SetItemDirPath(LPCTSTR sPath);
	int GetItemFileFindNum();
	void SetItemFileFindNum(int iNum);
	void UpdateItemFileFindNum();
	INT_PTR SetItemPicPathArray(CStringArray &saPicPath);
	CStringArray *GetItemPicPathArray();
	int FindPath(CString &sPath);
	bool RemoveAllPath(CString &sPath);
	bool IsDirOnFixDrive();
	bool IsOnFindPic();
	bool IsItemEnable();
	void SetItemEnable(bool bEnable);
	void UpdateDirState();
	void SetOnExit(bool bOnExit = true);
	void SetOnFindPic(bool bOnFindPic = true);

private:
	HWND m_hWnd;
	CIni *m_pIni;
	bool m_bInit;
	bool m_bOnExit;
	bool m_bOnFindPic;
	bool m_bItemEnable;
	bool m_bFindPath;
	UINT m_uDirState;
	CMutex m_mux;
	CStringArray m_saPicPath;
};
