#pragma once
#include "afxwin.h"
#include "CxImage/CxImage/xImage.h"

class CFindStaticShowPic : public CStatic
{
public:
	CFindStaticShowPic();
	virtual ~CFindStaticShowPic();

	bool SetPicPath(LPCTSTR lpPath);

private:
	CxImage *m_pImage;

public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
protected:
	virtual void PreSubclassWindow();
};
