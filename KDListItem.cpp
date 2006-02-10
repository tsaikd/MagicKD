#include "StdAfx.h"
#include "KDListItem.h"

CKDListItem::CKDListItem(int iColumn) : m_pText(NULL)
{
	m_iColumn = iColumn;
	m_pText = new CString[iColumn];
}

CKDListItem::~CKDListItem()
{
	if (m_pText)
		delete [] (m_pText);
}

void CKDListItem::SetText(int iColumn, CString sText)
{
	if (iColumn > m_iColumn)
		return;
	m_pText[iColumn] = sText;
}

CString CKDListItem::GetText(int iColumn)
{
	if (iColumn > m_iColumn)
		return CString();
	return m_pText[iColumn];
}
