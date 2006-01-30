#pragma once

class CKDListItem
{
public:
	CKDListItem(int iColumn);
	virtual ~CKDListItem();

	void SetText(int iColumn, CString sText);
	CString GetText(int iColumn);

private:
	int m_iColumn;
	CString *m_pText;
};
