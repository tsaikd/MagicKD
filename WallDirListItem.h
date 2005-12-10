#pragma once

class CWallDirListItem
{
public:
	CWallDirListItem();
	virtual ~CWallDirListItem();

	CString GetFullPath() const { return m_sPath; }
	void SetFullPath(CString sPath) { m_sPath = sPath; }
	UINT GetFindNumber() const { return m_uFindNumber; }
	void SetFindNumber(UINT uFindNumber) { m_uFindNumber = uFindNumber; }

private:
	CString m_sPath;
	UINT m_uFindNumber;
};
