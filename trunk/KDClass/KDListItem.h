#pragma once

class CKDListItem
{
public:
	CKDListItem(UINT uColumn);
	virtual ~CKDListItem();

	bool SetText(UINT uColumn, LPCTSTR sText);
	LPCTSTR GetText(UINT uColumn);

private:
	UINT m_uColumn;
	CStringArray m_saText;
};
