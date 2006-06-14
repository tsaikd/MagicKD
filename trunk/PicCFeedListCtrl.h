#pragma once
#include "KDListCtrl.h"

class CPicCFeedListCtrl : public CKDListCtrl
{
public:
	CPicCFeedListCtrl();
	virtual ~CPicCFeedListCtrl();
	virtual void Init();

private:
	bool m_bInit;
};
