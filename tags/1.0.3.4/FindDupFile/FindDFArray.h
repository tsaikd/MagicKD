#pragma once
#include "KDQArray.h"
#include "FindDupFileProc.h"

class CFindDFArray : public CKDQArray<CFindDupFileProc *, CFindDupFileProc *>
{
public:
	CFindDFArray();
	virtual ~CFindDFArray();

	virtual int QuickSortCmp(CFindDupFileProc *&x, CFindDupFileProc *&y);
};
