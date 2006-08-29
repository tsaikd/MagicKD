#pragma once
#include <afxmt.h>
#include "sqlite/sqlite3.h"

class CKDSQLiteField
{
public:
	CKDSQLiteField() {}
	CKDSQLiteField(LPCTSTR lpName, LPCTSTR lpType = _T("VARCHAR")) { m_sName = lpName; m_sType = lpType; }

	inline bool IsEmpty() { return (m_sName.IsEmpty() || m_sType.IsEmpty()); }

	CString m_sName;
	CString m_sType;
};

class CKDSQLiteTable
{
public:
	CKDSQLiteTable() {}

	inline bool IsEmpty() { return (m_sTableName.IsEmpty() || m_aField.IsEmpty()); }
	inline void AddField(LPCTSTR lpName, LPCTSTR lpType = _T("VARCHAR")) { m_aField.Add(CKDSQLiteField(lpName, lpType)); }
	inline void Empty() { m_sTableName.Empty(); m_aField.RemoveAll(); }

	CString m_sTableName;
	CArray<CKDSQLiteField> m_aField;
};

class CKDSQLiteAPI
{
public:
	CKDSQLiteAPI();
	virtual ~CKDSQLiteAPI();

	virtual bool OpenDB(LPCTSTR lpDBPath);
	virtual void CloseDB();
	virtual bool ReloadDB();

	bool CreateTable(CKDSQLiteTable &table, CString *strErrMsg = NULL);
	bool CheckTableField(CKDSQLiteTable &table, CString *strErrMsg = NULL);

	bool IsDBLoaded();
	bool IsTableExists(LPCTSTR lpTableName);
	bool IsFieldExists(LPCTSTR lpTableName, LPCTSTR lpFieldName);

	void SetDBSync(bool bSync = true);
	int GetDataCount(LPCTSTR strSQL, CString *strErrMsg = NULL);

	bool ExecSQL(LPCTSTR strSQL, CString *strErrMsg = NULL);
	bool GetTableSQL(LPCTSTR strSQL, CStringArray &saTable, CString *strErrMsg = NULL, int *nFields = NULL, int *nRow = NULL);

protected:
	sqlite3		*m_pDB;
	CMutex		m_muxDB;
	CString		m_sDBPath;
};
