#include "StdAfx.h"
#include "Others.h"
#include "WallOthers.h"

// if do resample, then return true
bool AutoPicSize(CxImage &img, CSize *pSizeMax/* = NULL*/)
{
	CSize sizeImg(img.GetWidth(), img.GetHeight());
	CScreenSize sizeScreen;
	if (!pSizeMax)
		pSizeMax = &sizeScreen;

	CRect rcWin;
	GetWindowRect(GetDesktopWindow(), rcWin);

	if (CalcPicSize(sizeImg, *pSizeMax)) {
		img.QIShrink(sizeImg.cx, sizeImg.cy);
//		img.Resample2(sizeImg.cx, sizeImg.cy, CxImage::IM_CATROM);
		return true;
	} else {
		return false;
	}
}
