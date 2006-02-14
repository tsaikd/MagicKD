#include "StdAfx.h"
#include "Others.h"
#include "WallOthers.h"

// if do resample, then return true
bool AutoPicSize(CxImage &img)
{
	CSize sizeImg(img.GetWidth(), img.GetHeight());

	CRect rcWin;
	GetWindowRect(GetDesktopWindow(), rcWin);

	if (CalcPicSize(sizeImg, CScreenSize())) {
		img.QIShrink(sizeImg.cx, sizeImg.cy);
//		img.Resample2(sizeImg.cx, sizeImg.cy, CxImage::IM_CATROM);
		return true;
	} else {
		return false;
	}
}
