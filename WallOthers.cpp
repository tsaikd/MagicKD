#include "StdAfx.h"
#include "WallOthers.h"

// if do resample, then return true
bool AutoPicSize(CxImage &img)
{
	CSize sizeImg(img.GetWidth(), img.GetHeight());

	CRect rcWin;
	GetWindowRect(GetDesktopWindow(), rcWin);

	if (_AutoPicSize(sizeImg, CSize(rcWin.Width(), rcWin.Height()-30))) {
		img.QIShrink(sizeImg.cx, sizeImg.cy);
//		img.Resample2(sizeImg.cx, sizeImg.cy, CxImage::IM_CATROM);
		return true;
	} else {
		return false;
	}
}

// if need resample, then return true, and set new size at cpSizeSrc
// else return false
bool _AutoPicSize(CSize &sizeSrc, CSize const &sizeMax)
{
	if ((sizeMax.cx >= sizeSrc.cx) && (sizeMax.cy >= sizeSrc.cy))
		return false;

	double dRatioX, dRatioY;
	dRatioX = (double)sizeMax.cx / (double)sizeSrc.cx;
	dRatioY = (double)sizeMax.cy / (double)sizeSrc.cy;

	double dRatio = (dRatioX<dRatioY) ? dRatioX : dRatioY;
	sizeSrc.SetSize((int)((double)sizeSrc.cx * dRatio), (int)((double)sizeSrc.cy * dRatio));
	return true;
}
