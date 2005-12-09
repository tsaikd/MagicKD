#pragma once


// CKDListBox

class CKDListBox : public CListBox
{
	DECLARE_DYNAMIC(CKDListBox)

public:
	CKDListBox();
	virtual ~CKDListBox();

	void DeleteAllString();
	int MouseInItem();			// return LB_ERR if no match, else return nItemID

protected:
	DECLARE_MESSAGE_MAP()
};


