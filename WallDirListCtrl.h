#pragma once
#include "afxwin.h"
#include "WallListCtrl.h"
#include "WallDirListItem.h"

class CWallDirListCtrl : public CWallListCtrl
{
public:
	CWallDirListCtrl();
	virtual ~CWallDirListCtrl();

	void Init(LPCTSTR sClassListName, int iClassNum);
	void SetClassListName(LPCTSTR sClassListName);
	LPCTSTR GetClassListName();

	void AddItem(int iClassNum, LPCTSTR sPath, CStringArray *saPathList = NULL);
	LPCTSTR GetPicPathRand();
	void UpdateItemPicNum(int nItem);
	int FindPicPath(CWallDirListItem *pItem, bool bRecursive = false);
	void DeleteAllItemsIni();

	CIni *m_pIni;

private:
	bool m_bInit;
	CString m_sClassListName;
	CArray<CWallDirListItem*> m_apDirListItem;

	bool __bMatchSupport(LPCTSTR sPat);
	CStringList *__pFindPicPath(LPCTSTR sPath, bool bRecursive = false);


// Thread Stander Code
public:
	void CreateThread()
	{ VERIFY( m_hThread = ::CreateThread(NULL, 0, ThreadProc, (LPVOID) this, 0, &m_dwThreadId) ); }
	virtual DWORD ThreadProc();
protected:
	HANDLE m_hThread;
	DWORD m_dwThreadId;
	bool m_bCanThreadRun;
	HANDLE m_hThreadInit;
	CWallDirListItem *m_ThreadParamItem;
private:
	static DWORD WINAPI ThreadProc(LPVOID pParam) {
		CWallDirListCtrl *pThis = (CWallDirListCtrl *) pParam;
		return pThis->ThreadProc();
	}

public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnLvnGetdispinfo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnDeleteitem(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
