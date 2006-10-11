#include "StdAfx.h"
#include "PicCFeed.h"

CPicCFeed::CPicCFeed()
{
}

CPicCFeed::~CPicCFeed()
{
}

bool CPicCFeed::OpenDB(LPCTSTR sDBPath)
{
	bool bRes = CFeed::OpenDB(sDBPath);
	if (!bRes)
		return bRes;

	CKDSQLiteTable table;
	table.m_sTableName = _T("PicFeed");
	table.AddField(_T("FeedLink"), _T("VARCHAR PRIMARY KEY"));
	table.AddField(_T("name"));
	CheckTableField(table);

	table.Empty();
	table.m_sTableName = _T("PicUnDownload");
	table.AddField(_T("Url"), _T("VARCHAR PRIMARY KEY"));
	table.AddField(_T("Localpath"));
	CheckTableField(table);

	table.Empty();
	table.m_sTableName = _T("PicDownloadOver");
	table.AddField(_T("Url"), _T("VARCHAR PRIMARY KEY"));
	table.AddField(_T("Localpath"));
	CheckTableField(table);

	table.Empty();
	table.m_sTableName = _T("PicDownloadFailed");
	table.AddField(_T("Url"), _T("VARCHAR PRIMARY KEY"));
	table.AddField(_T("Localpath"));
	table.AddField(_T("MaxDLPercent"));
	CheckTableField(table);

	return bRes;
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
