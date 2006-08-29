#pragma once
#include "FeedSource.h"

class CPicCFeed : public CFeed
{
public:
	CPicCFeed();
	virtual ~CPicCFeed();

	bool OpenDB(LPCTSTR sDBPath);
	using CFeed::ReloadDB;

	CString GetFeedName(LPCTSTR lpURL);

	using CFeed::SetDBSync;
	using CFeed::GetDataCount;

	using CFeed::GetTableSQL;
	using CFeed::ExecSQL;
	using CFeed::EscapeQuote;
};
