#pragma once
#include "FeedSource.h"

class CPicCFeed : public CFeed
{
public:
	CPicCFeed();
	virtual ~CPicCFeed();

	void SetDBPath(LPCTSTR sDBPath);

	CString GetFeedName(LPCTSTR lpURL);

	using CFeed::GetTableSQL;
	using CFeed::ExecSQL;
	using CFeed::EscapeQuote;
};
