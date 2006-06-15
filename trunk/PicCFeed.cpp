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
	strSQL.Format(_T("CREATE TABLE PicFeed (FeedLink VARCHAR, name VARCHAR);"));
	ExecSQL(strSQL);
}

CString CPicCFeed::GetFeedName(LPCTSTR lpURL)
{
	int nFields;
	int nRow;
	CStringArray saTable;
	CString strSQL;
	strSQL.Format(_T("SELECT DISTINCT name FROM PicFeed WHERE FeedLink = '%s';"), EscapeQuote(lpURL));
	GetTableSQL(strSQL, saTable, &nFields, &nRow);

	if (nRow)
		return saTable[1];
	else
		return _T("");
}