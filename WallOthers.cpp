#include "StdAfx.h"
#include "WallOthers.h"

// if do resample, then return true
bool AutoPicSize(CxImage &img)
{
	CPoint cpSize(img.GetWidth(), img.GetHeight());

	if (_AutoPicSize(cpSize, CPoint(1024, 768))) {
		img.QIShrink(cpSize.x, cpSize.y);
//		img.Resample2(cpSize.x, cpSize.y, CxImage::IM_CATROM);
		return true;
	} else {
		return false;
	}
}

// if need resample, then return true, and set new size at cpSizeSrc
// else return false
bool _AutoPicSize(CPoint &cpSizeSrc, CPoint const &cpSizeMax)
{
	if ((cpSizeMax.x >= cpSizeSrc.x) && (cpSizeMax.y >= cpSizeSrc.y))
		return false;

	double dRatioX, dRatioY;
	dRatioX = (double)cpSizeMax.x / (double)cpSizeSrc.x;
	dRatioY = (double)cpSizeMax.y / (double)cpSizeSrc.y;

	double dRatio = (dRatioX<dRatioY) ? dRatioX : dRatioY;
	cpSizeSrc.SetPoint((int)((double)cpSizeSrc.x * dRatio), (int)((double)cpSizeSrc.y * dRatio));
	return true;
}
