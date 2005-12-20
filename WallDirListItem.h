#pragma once
#include "KDListItem.h"

class CWallDirListItem : public CKDListItem
{
public:
	CWallDirListItem() : m_iClassNum(-1), m_iFindNum(0) {}
	virtual ~CWallDirListItem() {}

	int GetClassNum()  { return m_iClassNum; }
	void SetClassNum(int iClassNum) { m_iClassNum = iClassNum; }
	CString GetPath() { return m_sPath; }
	void SetPath(CString sPath) { m_sPath = sPath; }
	int GetFindNum() { return m_iFindNum; }
	void SetFindNum(int iFindNum) { m_iFindNum = iFindNum; }

private:
	int m_iClassNum;
	CString m_sPath;
	int m_iFindNum;
};
