#pragma once
#include "KDLog.h"

class CMagicKDLog : public CKDLog
{
public:
	CMagicKDLog() {}
	virtual ~CMagicKDLog() {}
};

extern CMagicKDLog *g_pTheLog;
