#include "StdAfx.h"
#include "KDEvent.h"

CKDEvent::CKDEvent()
	:	CEvent(FALSE, TRUE), m_bAcquired(FALSE)
{
}

CKDEvent::~CKDEvent()
{
	Unlock();
}
