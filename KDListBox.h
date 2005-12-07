#pragma once


// CKDListBox

class CKDListBox : public CListBox
{
	DECLARE_DYNAMIC(CKDListBox)

public:
	CKDListBox();
	virtual ~CKDListBox();

	void DeleteAllString();

protected:
	DECLARE_MESSAGE_MAP()
};


