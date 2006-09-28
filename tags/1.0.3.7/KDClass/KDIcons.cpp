#include "StdAfx.h"

#include "KDIcons.h"

CKDIcons::CKDIcons()
	:	m_hModule(NULL)
{
}

CKDIcons::~CKDIcons()
{
	Destroy();
}

bool CKDIcons::Init(LPCTSTR lpIconPath)
{
	HMODULE hModule = LoadLibrary(lpIconPath);
	if (hModule == NULL)
		return false;

	m_hModule = hModule;
	m_sIconPath = lpIconPath;
	return true;
}

void CKDIcons::Destroy()
{
	if (m_hModule) {
		FreeLibrary(m_hModule);
		m_hModule = NULL;
	}

	m_sIconPath.Empty();
}

HICON CKDIcons::GetIcon(UINT nID, int cx/* = 0*/, int cy/* = 0*/)
{
	HICON hIcon = (HICON)LoadImage(m_hModule, MAKEINTRESOURCE(nID), IMAGE_ICON, cx, cy, LR_DEFAULTCOLOR);

	return hIcon;
}
