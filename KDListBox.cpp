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

BEGIN_MESSAGE_MAP(CKDListBox, CListBox)
END_MESSAGE_MAP()


// CKDListBox 訊息處理常式
