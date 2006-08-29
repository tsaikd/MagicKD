#include "StdAfx.h"
#include "KDListItem.h"

CKDListItem::CKDListItem(UINT uColumn) : m_uColumn(uColumn)
{
	m_saText.SetSize(uColumn);
}

CKDListItem::~CKDListItem()
{
}

bool CKDListItem::SetText(UINT uColumn, LPCTSTR sText)
{
	if (uColumn >= m_uColumn)
		return false;

	m_saText[uColumn] = sText;
	return true;
}

LPCTSTR CKDListItem::GetText(UINT uColumn)
{
	if (uColumn >= m_uColumn)
		return NULL;

	return m_saText[uColumn];
}
