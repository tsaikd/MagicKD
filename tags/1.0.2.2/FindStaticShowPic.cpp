#include "StdAfx.h"
#include "Others.h"

#include "FindStaticShowPic.h"

CFindStaticShowPic::CFindStaticShowPic()
	:	m_pImage(NULL)
{
}

CFindStaticShowPic::~CFindStaticShowPic()
{
	if (m_pImage)
		delete m_pImage;
}

bool CFindStaticShowPic::SetPicPath(LPCTSTR lpPath)
{
	if (!lpPath) {
		if (m_pImage) {
			delete m_pImage;
			m_pImage = NULL;
		}
		return true;
	}

	if (!m_pImage)
		m_pImage = new CxImage;

	bool bRes = m_pImage->Load(lpPath);

	if (!bRes) {
		delete m_pImage;
		m_pImage = NULL;
	}

	Invalidate();

	return bRes;
}

void CFindStaticShowPic::DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/)
{
	HDC hDC = GetDC()->GetSafeHdc();
	if (hDC == INVALID_HANDLE_VALUE)
		return;

	CRect rcWin;
	GetClientRect(rcWin);

	FillRect(hDC, rcWin, GetSysColorBrush(COLOR_3DFACE));

	if (m_pImage) {
		CSize sizeWin(rcWin.Width(), rcWin.Height());
		CSize sizeImg(m_pImage->GetWidth(), m_pImage->GetHeight());

		CalcPicSize(sizeImg, sizeWin);
		CRect rcImg((sizeWin.cx - sizeImg.cx) >> 1, (sizeWin.cy - sizeImg.cy) >> 1, sizeWin.cx, sizeWin.cy);
		rcImg.right -= rcImg.left;
		rcImg.bottom -= rcImg.top;

		m_pImage->Draw(hDC, rcImg);
	}
}

void CFindStaticShowPic::PreSubclassWindow()
{
	// TODO: 在此加入特定的程式碼和 (或) 呼叫基底類別
	ModifyStyle(SS_TYPEMASK, SS_OWNERDRAW);

	CStatic::PreSubclassWindow();
}
