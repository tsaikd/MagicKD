#pragma once

class CKDIcons
{
public:
	CKDIcons();
	virtual ~CKDIcons();

	bool Init(LPCTSTR lpIconPath);
	void Destroy();

	HICON GetIcon(UINT nID, int cx = 0, int cy = 0);

private:
	HMODULE m_hModule;
	CString m_sIconPath;
};
