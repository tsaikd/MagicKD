// KDListBox.cpp : 實作檔
//

#include "stdafx.h"
#include "KDListBox.h"


// CKDListBox

IMPLEMENT_DYNAMIC(CKDListBox, CListBox)
CKDListBox::CKDListBox()
{
}

CKDListBox::~CKDListBox()
{
}

void CKDListBox::DeleteAllString()
{
	int i = GetCount();
	for (i=GetCount() ; i>=0 ; i--)
		DeleteString(i);
}

int CKDListBox::MouseInItem()
{
	CPoint ptBuf;
	BOOL bOutside;
	int iNearPos, iGuessPos, iItemH;

	iNearPos = ItemFromPoint(ptBuf, bOutside);
	iItemH = GetItemHeight(0);
	iGuessPos = ptBuf.y / iItemH;

	return LB_ERR;
}

BEGIN_MESSAGE_MAP(CKDListBox, CListBox)
END_MESSAGE_MAP()


// CKDListBox 訊息處理常式
