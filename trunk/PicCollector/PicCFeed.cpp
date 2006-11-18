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
	table.m_sTableName = _T("FeedSource");
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

	{
	// Update from version <= 1.0.3.9
	CStringArray saTable;
	CString strSQL;
	strSQL.Format(_T("SELECT name, FeedLink FROM PicFeed;"));
	GetTableSQL(strSQL, saTable);

	INT_PTR i, iCount = saTable.GetCount();
	for (i=2 ; i<(iCount-1) ; i+=2) {
		strSQL.Format(_T("UPDATE FeedSource SET name='%s' WHERE FeedLink='%s';"),
			EscapeQuote(saTable[i]), EscapeQuote(saTable[i+1]));
		ExecSQL(strSQL);
	}
	ExecSQL(_T("DROP TABLE IF EXISTS PicFeed;"));
	}

	return bRes;
}

CString CPicCFeed::GetFeedName(LPCTSTR lpURL)
{
	CStringArray saTable;
	CString strSQL;
	strSQL.Format(_T("SELECT name FROM FeedSource WHERE FeedLink = '%s';"), EscapeQuote(lpURL));
	GetTableSQL(strSQL, saTable);

	if (saTable.GetCount() > 1)
		return saTable[1];
	else
		return _T("");
}

bool CPicCFeed::SetFeedName(LPCTSTR lpURL, LPCTSTR lpName)
{
	CStringArray saTable;
	CString strSQL;
	strSQL.Format(_T("SELECT name FROM FeedSource WHERE FeedLink = '%s';"), EscapeQuote(lpURL));
	GetTableSQL(strSQL, saTable);

	if (saTable.GetCount() <= 1)
		return false;

	strSQL.Format(_T("UPDATE FeedSource SET name = '%s' WHERE FeedLink = '%s';"),
		EscapeQuote(lpURL),
		EscapeQuote(lpName));
	ExecSQL(strSQL);

	return true;
}
