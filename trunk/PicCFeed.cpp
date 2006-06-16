#include "StdAfx.h"
#include "PicCFeed.h"

CPicCFeed::CPicCFeed()
{
}

CPicCFeed::~CPicCFeed()
{
}

void CPicCFeed::SetDBPath(LPCTSTR sDBPath)
{
	CFeed::SetDBPath(sDBPath);

	CString strSQL;
	strSQL.Format(_T("CREATE TABLE PicFeed (FeedLink VARCHAR PRIMARY KEY, name VARCHAR);"));
	ExecSQL(strSQL);
	strSQL.Format(_T("CREATE TABLE PicUnDownload (Url VARCHAR PRIMARY KEY, Localpath VARCHAR);"));
	ExecSQL(strSQL);
}

CString CPicCFeed::GetFeedName(LPCTSTR lpURL)
{
	CStringArray saTable;
	CString strSQL;
	strSQL.Format(_T("SELECT DISTINCT name FROM PicFeed WHERE FeedLink = '%s';"), EscapeQuote(lpURL));
	GetTableSQL(strSQL, saTable);

	if (saTable.GetCount() > 1)
		return saTable[1];
	else
		return _T("");
}
