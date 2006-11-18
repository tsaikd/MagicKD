#pragma once
#include "FeedSource.h"

class CPicCFeed : public CFeed
{
public:
	CPicCFeed();
	virtual ~CPicCFeed();

	bool OpenDB(LPCTSTR sDBPath);

	CString GetFeedName(LPCTSTR lpURL);
	bool SetFeedName(LPCTSTR lpURL, LPCTSTR lpName);
};
