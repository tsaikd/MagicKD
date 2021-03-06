#pragma once
#include "WallListCtrl.h"

class CWallDirListCtrl :
	public CWallListCtrl
{
	DECLARE_MESSAGE_MAP()
public:
	CWallDirListCtrl();
	virtual ~CWallDirListCtrl();
	virtual void SaveIni();

	virtual void Init(CIni *pIni, LPCTSTR sListClassName, bool *pbShowDirLoadError);
	void DoSize();
	void Localize();

	void SetListClassName(LPCTSTR sName);
	CString GetListClassName();
	bool AddItem(LPCTSTR sDirPath);
	void UpdateAllItemFileFindNum();
	void UpdateSelectItemFileFindNum();
	bool IsFindPath();
	bool IsAllItemEnable();
	void SetAllItemEnable(bool bEnable);
	void SetOnExit(bool bOnExit = true);

	void RemoveFromPath(CString &sPath);

private:
	bool m_bInit;
	bool m_bFindPath;
	bool m_bAllItemEnable;
	bool *m_pbShowDirLoadError;
	CString m_sListClassName;

public:
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnLvnDeleteitem(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDropFiles(HDROP hDropInfo);
};
