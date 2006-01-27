#pragma once
#include "KDListItem.h"

class CWallDirListItem : public CKDListItem
{
public:
	CWallDirListItem() : m_iClassNum(-1) {}
	virtual ~CWallDirListItem() {}

	int GetClassNum()  { return m_iClassNum; }
	void SetClassNum(int iClassNum) { m_iClassNum = iClassNum; }
	CString GetPath() { return m_sPath; }
	void SetPath(CString sPath) { m_sPath = sPath; }
	int GetFindNum() { return m_saPicPath.GetCount(); }
//	void SetFindNum(int iFindNum = -1) { m_iFindNum = iFindNum; }
	CString GetPicPathRand() { return m_saPicPath[rand()%m_saPicPath.GetCount()]; }
	CStringArray *GetPicPathArray() { return &m_saPicPath; }
	void SetPicPathArray(const CStringArray *saPicPath) { m_saPicPath.RemoveAll(); m_saPicPath.Append(*saPicPath); }

private:
	int m_iClassNum;
	CString m_sPath;
	CStringArray m_saPicPath;
};
