#include "StdAfx.h"
#include "KDSQLiteAPI.h"

CKDSQLiteAPI::CKDSQLiteAPI()
	:	m_pDB(NULL)
{
}

CKDSQLiteAPI::~CKDSQLiteAPI()
{
	CloseDB();
}

bool CKDSQLiteAPI::OpenDB(LPCTSTR lpDBPath)
{
	CloseDB();

	if (!PathFileExists(lpDBPath) || PathIsDirectory(lpDBPath))
		return false;

	if (SQLITE_OK != sqlite3_open16(lpDBPath, &m_pDB))
		return false;

	m_sDBPath = lpDBPath;
	return true;
}

void CKDSQLiteAPI::CloseDB()
{
	if (m_pDB) {
		sqlite3_close(m_pDB);
		m_pDB = NULL;
	}
	m_sDBPath.Empty();
}

bool CKDSQLiteAPI::ReloadDB()
{
	if (!IsDBLoaded())
		return false;

	return OpenDB(m_sDBPath);
}

bool CKDSQLiteAPI::CreateTable(CKDSQLiteTable &table, CString *strErrMsg/* = NULL*/)
{
	if (!IsDBLoaded())
		return false;
	if (table.IsEmpty())
		return false;

	int i, iCount;
	CString strSQL;

	if (IsTableExists(table.m_sTableName))
		return false;

	strSQL.Format(_T("CREATE TABLE %s (%s %s"), table.m_sTableName, table.m_aField[0].m_sName, table.m_aField[0].m_sType);
	iCount = table.m_aField.GetCount();
	for (i=1 ; i<iCount ; i++) {
		if (table.m_aField[i].IsEmpty())
			continue;

		strSQL.AppendFormat(_T(",%s %s"), table.m_aField[i].m_sName, table.m_aField[i].m_sType);
	}
	strSQL.Append(_T(")"));

	return ExecSQL(strSQL, strErrMsg);
}

bool CKDSQLiteAPI::CheckTableField(CKDSQLiteTable &table, CString *strErrMsg/* = NULL*/)
{
	if (!IsDBLoaded())
		return false;
	if (table.IsEmpty())
		return false;

	int i, iCount;
	CString strSQL;

	if (!IsTableExists(table.m_sTableName))
		return CreateTable(table, strErrMsg);

	iCount = table.m_aField.GetCount();
	for (i=0 ; i<iCount ; i++) {
		if (table.m_aField[i].IsEmpty())
			continue;

		if (!IsFieldExists(table.m_sTableName, table.m_aField[i].m_sName)) {
			strSQL.Format(_T("ALTER TABLE %s ADD %s %s"), table.m_sTableName, table.m_aField[i].m_sName, table.m_aField[i].m_sType);
			if (!ExecSQL(strSQL, strErrMsg))
				return false;
		}
	}

	return true;
}

bool CKDSQLiteAPI::IsDBLoaded()
{
	if (!m_pDB || m_sDBPath.IsEmpty())
		return false;

	return true;
}

bool CKDSQLiteAPI::IsTableExists(LPCTSTR lpTableName)
{
	if (!IsDBLoaded())
		return false;

	CString strSQL;

	strSQL.Format(_T("SELECT * FROM %s LIMIT 0"), lpTableName);
	return ExecSQL(strSQL);
}

bool CKDSQLiteAPI::IsFieldExists(LPCTSTR lpTableName, LPCTSTR lpFieldName)
{
	if (!IsDBLoaded())
		return false;

	CString strSQL;

	strSQL.Format(_T("SELECT %s FROM %s LIMIT 0"), lpFieldName, lpTableName);
	return ExecSQL(strSQL);
}

int CKDSQLiteAPI::GetDataCount(LPCTSTR strSQL, CString *strErrMsg/* = NULL*/)
{
	if (!IsDBLoaded())
		return -1;

	int iRes = 0;

	m_muxDB.Lock();
	sqlite3_stmt *stmt = NULL;
	if (SQLITE_OK == sqlite3_prepare16(m_pDB, strSQL, -1, &stmt, 0)) {
		while (sqlite3_step(stmt) == SQLITE_ROW)
			iRes++;
	} else {
		iRes = -1;
	}
	if (strErrMsg)
		*strErrMsg = CString((char *)sqlite3_errmsg(m_pDB));
	sqlite3_finalize(stmt);
	m_muxDB.Unlock();

	return iRes;
}

bool CKDSQLiteAPI::ExecSQL(LPCTSTR strSQL, CString *strErrMsg/* = NULL*/)
{
	if (!IsDBLoaded())
		return false;

	bool bRes = true;

	m_muxDB.Lock();
	sqlite3_stmt *stmt = NULL;
	bRes = (SQLITE_OK == sqlite3_prepare16(m_pDB, strSQL, -1, &stmt, 0));
	if (strErrMsg)
		*strErrMsg = CString((char *)sqlite3_errmsg(m_pDB));
	sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	m_muxDB.Unlock();

	return bRes;
}

bool CKDSQLiteAPI::GetTableSQL(LPCTSTR strSQL, CStringArray &saTable, CString *strErrMsg/* = NULL*/, int *nFields/* = NULL*/, int *nRow/* = NULL*/)
{
	if (!IsDBLoaded())
		return false;

	m_muxDB.Lock();
	int nCol;
	sqlite3_stmt *stmt = NULL;
	if (SQLITE_OK != sqlite3_prepare16(m_pDB, strSQL, -1, &stmt, 0)) {
#ifdef DEBUG
		if (strErrMsg)
			*strErrMsg = CString((char *)sqlite3_errmsg(m_pDB));
#endif //DEBUG
		if (!stmt) {
			m_muxDB.Unlock();
			return false;
		}
	}
	int iFields = sqlite3_column_count(stmt);
	int iRow = 0;
	saTable.RemoveAll();

	for(nCol=0; nCol < iFields; nCol++)
		saTable.Add(LPCTSTR(sqlite3_column_name16(stmt, nCol)));

	while (sqlite3_step(stmt) == SQLITE_ROW) {
         for(nCol=0; nCol < iFields; nCol++)
            saTable.Add(LPCTSTR(sqlite3_column_text16(stmt, nCol)));

         iRow++;
	}

	sqlite3_finalize(stmt);
	m_muxDB.Unlock();

	if (nFields)
		*nFields = iFields;
	if (nRow)
		*nRow = iRow;
	return true;
}
