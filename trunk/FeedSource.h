// FeedSource.h: interface for the CFeedSource class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FEEDSOURCE_H__C37CC283_CFD2_455A_AB00_B7CA0197CA2D__INCLUDED_)
#define AFX_FEEDSOURCE_H__C37CC283_CFD2_455A_AB00_B7CA0197CA2D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxmt.h>
#include "sqlite/sqlite3.h"

class CFeedItem
{
public:
	CFeedItem();
	virtual ~CFeedItem();
public:
	CString		m_strAuthor;
	CString		m_strCategory;
	CString		m_strDescription;
	CString		m_strLink;
	CString		m_strPubDate;
	CString		m_strSubject;
	CString		m_strTitle;
	CString		m_strReadStatus;
};


class CFeedSource  
{
public:
	CFeedSource();
	virtual ~CFeedSource();

public:
	CString		m_strCopyright;
	CString		m_strDescription;
	CString		m_strGenerator;
	CString		m_strLanguage;
	CString		m_strLastBuildDate;
	CString		m_strLink;
	CString		m_strTitle;
	CString		m_strTtl;
	CString		m_strWebMaster;
	CString		m_strImageDescription;
	CString		m_strImageHeight;
	CString		m_strImageWidth;
	CString		m_strImageLink;
	CString		m_strImageTitle;
	CString		m_strImageUrl;
	CString		m_strVersion;
};


class CFeed
{
public:
	CFeed();
	CFeed(LPCTSTR sDBPath);
	CFeed(LPCTSTR sDBPath, LPCTSTR strXMLURL);
	virtual ~CFeed();

	void	SetDBPath(LPCTSTR sDBPath);
	void	BuildFromFile(LPCTSTR strXMLURL);
	void	Save(bool bSaveSource = true);
	void	LoadLocal(LPCTSTR strLink);

	void	GetFeedSourceList(CStringArray& strTitleArray, CStringArray& strLinkArray);
	void	MarkItemRead(LPCTSTR strLink);
	bool	IsItemRead(LPCTSTR strLink);

	//void	RefreshAll();
	//void	DeleteFeedSource( CString strLink );
	//void	DeleteListArray( CStringArray& strLinkArray );

	CFeedSource						m_source;		// Feed Source
	CArray<CFeedItem,CFeedItem>		m_item;			// Feed Item
	BOOL							m_bAdded;
	sqlite3							*m_pDB;

protected:
	bool	ExecSQL(LPCTSTR strSQL, CString *strErrMsg = NULL);
	bool	GetTableSQL(LPCTSTR strSQL, CStringArray &saTable, CString *strErrMsg = NULL, int *nFields = NULL, int *nRow = NULL);

//	CString GetFieldValue( FieldsPtr fields, long nIndex );
//	CString GetComError( _com_error& e );
//	BOOL ExecuteSQL( _ConnectionPtr& pCnn, CString& strSQL, CString& strMsg );

	CMutex							m_muxDB;
	CString EscapeQuote(CString strValue);
private:
	CString GetModuleFileDir();

	void	GetVersion(MSXML2::IXMLDOMNode *pNode);
	void	IterateChildNodes(MSXML2::IXMLDOMNode *pNode);
	void	BuildImage(MSXML2::IXMLDOMNode *pNode);
	void	BuildItem(MSXML2::IXMLDOMNode *pNode);
	void	ClearLoadedItems();

    MSXML2::IXMLDOMDocument2*		m_pDoc;			// XML DOM Document
	int								m_nDepth;
	bool							m_bDBPath;
};

#endif // !defined(AFX_FEEDSOURCE_H__C37CC283_CFD2_455A_AB00_B7CA0197CA2D__INCLUDED_)
