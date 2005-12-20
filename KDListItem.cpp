#include "StdAfx.h"
#include "KDListItem.h"

CKDListItem::CKDListItem() : m_bInit(false), m_iColumn(-1), m_iValidNum(-1), m_pText(NULL), m_pNext(NULL)
{
}

CKDListItem::~CKDListItem()
{
	Destroy();
}

bool CKDListItem::Init(int iColumn, int iValidNum) {
	if (m_bInit)
		Destroy();
	m_iColumn = iColumn;
	m_iValidNum = iValidNum;
	m_pText = new CString[iColumn];
	m_bInit = true;
	return true;
}

void CKDListItem::Destroy() {
	if (m_pText) {
		delete [] (m_pText);
		m_pText = NULL;
	}
}

void CKDListItem::SetText(int iColumn, CString sText)
{
	if ((!m_bInit) || (iColumn > m_iColumn))
		return;
	m_pText[iColumn] = sText;
}

CString CKDListItem::GetText(int iColumn)
{
	if ((!m_bInit) || (iColumn > m_iColumn))
		return NULL;
	return m_pText[iColumn];
}
