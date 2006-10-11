#pragma once
#include "Icons/resource.h"
#include "KDIcons.h"

class CMagicKDIcons : public CKDIcons
{
public:
	CMagicKDIcons();
	virtual ~CMagicKDIcons();
};

extern CMagicKDIcons *g_pTheIcons;
