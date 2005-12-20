#pragma once

class CKDListItem
{
public:
	CKDListItem();
	virtual ~CKDListItem();

	bool Init(int iColumn, int iValidNum);
	virtual void Destroy();

	void SetText(int iColumn, CString sText);
	CString GetText(int iColumn);

	CKDListItem *m_pNext;

private:
	bool m_bInit;
	int m_iColumn;
	int m_iValidNum;
	CString *m_pText;
};
