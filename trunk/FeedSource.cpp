/******************************************************************************
*
*       (c) Copyright 2003 AgileInfoSoftware LLC. - All Rights Reserved
*		This code is released to public. You may make any change as appropriate
*		as long as this copyright notice is on the top. If you use it to build
*		commerial application, please email info@AgileInfoSoftware.com for license.
*
*                
******************************************************************************/

// FeedSource.cpp: implementation of the CFeedSource class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FeedSource.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CFeedItem::CFeedItem()
{
}

CFeedItem::~CFeedItem()
{
}

CFeedSource::CFeedSource()
{
}

CFeedSource::~CFeedSource()
{
}

CFeed::CFeed()
	: m_pDoc(NULL), m_nDepth(0), m_bAdded(FALSE), m_bDBPath(false), m_pDB(NULL)
{
}

CFeed::CFeed(LPCTSTR sDBPath)
	: m_pDoc(NULL), m_nDepth(0), m_bAdded(FALSE), m_bDBPath(false), m_pDB(NULL)
{
	SetDBPath(sDBPath);
}

CFeed::CFeed(LPCTSTR sDBPath, LPCTSTR strXMLURL)
	: m_pDoc(NULL), m_nDepth(0), m_bAdded(FALSE), m_bDBPath(false), m_pDB(NULL)
{
	SetDBPath(sDBPath);
	BuildFromFile(strXMLURL);
}

CFeed::~CFeed()
{
	if (m_pDB)
		sqlite3_close(m_pDB);
}

bool CFeed::ExecSQL(LPCTSTR strSQL, CString *strErrMsg/* = NULL*/)
{
	if (!m_bDBPath)
		return false;

	m_muxDB.Lock();
	sqlite3_stmt *stmt = NULL;
	sqlite3_prepare16(m_pDB, strSQL, -1, &stmt, 0);
	if (strErrMsg)
		*strErrMsg = CString((char *)sqlite3_errmsg(m_pDB));
	sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	m_muxDB.Unlock();

	return true;
}

bool CFeed::GetTableSQL(LPCTSTR strSQL, CStringArray &saTable, CString *strErrMsg/* = NULL*/, int *nFields/* = NULL*/, int *nRow/* = NULL*/)
{
	if (!m_bDBPath)
		return false;

	m_muxDB.Lock();
	int nCol;
	sqlite3_stmt *stmt = NULL;
	if (SQLITE_OK != sqlite3_prepare16(m_pDB, strSQL, -1, &stmt, 0)) {
		if (strErrMsg)
			*strErrMsg = CString((char *)sqlite3_errmsg(m_pDB));
		//CString sErrMsg = CString((char *)sqlite3_errmsg(m_pDB));
		//sErrMsg.AppendFormat(_T("\n%s\nSQL ERROR: Prepare stmt Failed"), strSQL);
		//AfxMessageBox(sErrMsg);
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

void CFeed::SetDBPath(LPCTSTR sDBPath)
{
	sqlite3_open16(sDBPath, &m_pDB);

	m_bDBPath = true;

	CString strSQL;
	strSQL.Format(_T("CREATE TABLE FeedItem (FeedLink VARCHAR, title VARCHAR, link VARCHAR UNIQUE, description VARCHAR, pubdate VARCHAR, author VARCHAR, category VARCHAR, subject VARCHAR, readstatus VARCHAR);"));
	ExecSQL(strSQL);

	strSQL.Format(_T("CREATE TABLE FeedSource (FeedLink VARCHAR PRIMARY KEY, description VARCHAR, title VARCHAR, version VARCHAR, copyright VARCHAR, generator VARCHAR, feedlanguage VARCHAR, lastbuilddate VARCHAR, ttl VARCHAR, webmaster VARCHAR, imagedescription VARCHAR, imageheight VARCHAR, imagewidth VARCHAR, imagelink VARCHAR, imagetitle VARCHAR, imageurl VARCHAR);"));
	ExecSQL(strSQL);
}

/////////////////////////////////////////////////////////////////////////////
// Build XML Feed Information from an XML File
//
// strXMLURL:		Parsed in XML File Name
// This function will build Feed Object from scratch by parsing XML Feed Information
// Result is stored in m_source and m_item objects
//
#define RETURN { CoUninitialize(); return; }
void CFeed::BuildFromFile(LPCTSTR strXMLURL)
{
	if (!m_bDBPath)
		return;

	ClearLoadedItems();
	CString strTmpFile = GetModuleFileDir() + _T("\\FeedSource_tmp.xml");
	CoInitialize(NULL);

	// Step 0. Download XML File
	if (( URLDownloadToFile( NULL, strXMLURL, strTmpFile,0, NULL ) != S_OK )
		|| (!PathFileExists(strTmpFile)))
	{
		AfxMessageBox(_T("Failed to download ") + CString(strXMLURL));
		RETURN;
	}
	//while (!PathFileExists(strTmpFile))
	//	Sleep(2000);

	// Step 1. Open XML Document, if open fails, then return
	if ( m_pDoc != NULL )
	{
		m_pDoc->Release();
		m_pDoc = NULL;
	}
	if ( !SUCCEEDED (CoCreateInstance(MSXML2::CLSID_DOMDocument,
										NULL,
										CLSCTX_INPROC_SERVER,
										MSXML2::IID_IXMLDOMDocument,
										reinterpret_cast<void**>(&m_pDoc))))
	{
		// Failed to load XML Document, report error message
		AfxMessageBox( _T("Failed to CoCreateInstance") );
		RETURN;
	}

	m_pDoc->put_async( VARIANT_FALSE );
	if ( m_pDoc->load( _bstr_t(strTmpFile) ) == VARIANT_FALSE )
	{
		//MSXML2::IXMLDOMParseErrorPtr errPtr = m_pDoc->GetparseError();
		//_bstr_t bstrErr(errPtr->reason);
		//CString sErrMsg;
		//sErrMsg.Format(_T("Reason: %s\nError Code: 0x%x\nLine: %ldPos: %ld\n"), CString((char*)bstrErr), errPtr->errorCode, errPtr->line, errPtr->linepos);

		// Failed to load XML Document, report error message
		AfxMessageBox(_T("Failed to load XML Document"));
		RETURN;
	}

	// Step 2. Get version property if it is available
	// Step 3. Iterate to channel node, get the following child items
	// title
	// link
	// description
	// language
	// copyright
	// webMaster
	// lastBuildDate
	// ttl
	// generator
	// Then go to image node, get the following items
	//		title
	//		url
	//		link
	//		width
	//		height
	//		description

	// Step 4. go to item node, get the following items
	//		title
	//		description
	//		link
	//		author
	//		category
	//		pubDate
	//		subject
	MSXML2::IXMLDOMNode		*pNode = NULL;
	if ( SUCCEEDED(m_pDoc->QueryInterface(MSXML2::IID_IXMLDOMNode, 
				reinterpret_cast<void**>(&pNode))))
	{
		IterateChildNodes(pNode);
		pNode->Release();
		pNode = NULL;
	}

	DeleteFile(strTmpFile);
	m_source.m_strLink = strXMLURL;

	// We are not using smart pointer, so we have to release it outself
	if ( m_pDoc )
	{
		m_pDoc->Release();
		m_pDoc = NULL;
	}
	RETURN;
}
#undef RETURN

/////////////////////////////////////////////////////////////////////////////
// Save Feed Information into Database
//
void CFeed::Save(bool bSaveSource /*= true*/)
{
	CString	strSQL;
	int		nIndex;

	if (!m_bDBPath)
		return;
	if (m_source.m_strLink.IsEmpty())
		return;

	// Step 3. Execute Insert Statement on FeedSource
	// In double click case, we don't want to save this
	if ( bSaveSource )
	{
		strSQL.Format( _T("INSERT INTO FeedSource (FeedLink, description, title, version, copyright, generator, feedlanguage, lastbuilddate, ttl, webmaster, imagedescription, imageheight, imagewidth, imagelink, imagetitle, imageurl ) values('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', %d, '%s', '%s', '%s', '%s', '%s', '%s', '%s')"),
			EscapeQuote( m_source.m_strLink ),
			EscapeQuote( m_source.m_strDescription ),
			EscapeQuote( m_source.m_strTitle ),
			_T("2.0"),
			EscapeQuote( m_source.m_strCopyright ),
			EscapeQuote( m_source.m_strGenerator ),
			EscapeQuote( m_source.m_strLanguage ),
			EscapeQuote( m_source.m_strLastBuildDate ),
			EscapeQuote( m_source.m_strTtl ),
			EscapeQuote( m_source.m_strWebMaster ),
			EscapeQuote( m_source.m_strImageDescription ),
			EscapeQuote( m_source.m_strImageHeight ),
			EscapeQuote( m_source.m_strImageWidth ),
			EscapeQuote( m_source.m_strImageLink ),
			EscapeQuote( m_source.m_strImageTitle ),
			EscapeQuote( m_source.m_strImageUrl ) );
		if ( ExecSQL(strSQL) )
			m_bAdded = TRUE;
	}

	// Step 4. Execute Insert Statement on FeedItem
	for( nIndex = 0; nIndex < m_item.GetSize(); nIndex++ )
	{
		strSQL.Format( _T("INSERT INTO FeedItem (FeedLink, title, link, description, pubdate, author, category, subject) values('%s','%s','%s','%s','%s','%s','%s','%s')"),
			EscapeQuote( m_source.m_strLink ),
			EscapeQuote( m_item[nIndex].m_strTitle ),
			EscapeQuote( m_item[nIndex].m_strLink ),
			EscapeQuote( m_item[nIndex].m_strDescription ),
			EscapeQuote( m_item[nIndex].m_strPubDate ),
			EscapeQuote( m_item[nIndex].m_strAuthor ),
			EscapeQuote( m_item[nIndex].m_strCategory ),
			EscapeQuote( m_item[nIndex].m_strSubject )
			);
		ExecSQL(strSQL);
	}
}

/////////////////////////////////////////////////////////////////////////////
// Load Feed Information from database
//
void CFeed::LoadLocal(LPCTSTR strLink)
{
	CString			strSQL;
	CStringArray	saTable;

	if (!m_bDBPath)
		return;

	ClearLoadedItems();
	strSQL.Format(_T("SELECT DISTINCT description, title, version, copyright, generator, feedlanguage, lastbuilddate, ttl, webmaster, imagedescription, imageheight, imagewidth, imagelink, imagetitle, imageurl FROM FeedSource WHERE FeedLink = '%s'"), EscapeQuote(strLink));
	if (!GetTableSQL(strSQL, saTable))
		return;

	m_source.m_strLink				= strLink;
	m_source.m_strDescription		= saTable[15 + 0];
	m_source.m_strTitle				= saTable[15 + 1];
	m_source.m_strVersion			= saTable[15 + 2];
	m_source.m_strCopyright			= saTable[15 + 3];
	m_source.m_strGenerator			= saTable[15 + 4];
	m_source.m_strLanguage			= saTable[15 + 5];
	m_source.m_strLastBuildDate		= saTable[15 + 6];
	m_source.m_strTtl				= saTable[15 + 7];
	m_source.m_strWebMaster			= saTable[15 + 8];
	m_source.m_strImageDescription	= saTable[15 + 9];
	m_source.m_strImageHeight		= saTable[15 + 10];
	m_source.m_strImageWidth		= saTable[15 + 11];
	m_source.m_strImageLink			= saTable[15 + 12];
	m_source.m_strImageTitle		= saTable[15 + 13];
	m_source.m_strImageUrl			= saTable[15 + 14];


	// Step 4. Read FeedItem and populate it into m_item object
	strSQL.Format(_T("SELECT DISTINCT title, link, description, pubdate, author, category, subject, readstatus FROM FeedItem WHERE FeedLink = '%s' ORDER BY pubdate DESC, title ASC"), EscapeQuote(strLink));
	if (!GetTableSQL(strSQL, saTable))
		return;

	int i;
	CFeedItem item;
	for (i=8 ; i<saTable.GetCount() ; ) {
		item.m_strTitle			= saTable[i++];
		item.m_strLink			= saTable[i++];
		item.m_strDescription	= saTable[i++];
		item.m_strPubDate		= saTable[i++];
		item.m_strAuthor		= saTable[i++];
		item.m_strCategory		= saTable[i++];
		item.m_strSubject		= saTable[i++];
		item.m_strReadStatus	= saTable[i++];

		m_item.Add(item);
	}
}

void CFeed::GetFeedSourceList(CStringArray& strTitleArray, CStringArray& strLinkArray)
{
	CString			strSQL;
	CStringArray	saTable;

	if (!m_bDBPath)
		return;

	// Step 3. Read FeedSource and populate it into m_source object
	strSQL = _T("SELECT DISTINCT title, FeedLink FROM FeedSource ORDER BY title");
	GetTableSQL(strSQL, saTable);

	int i;
	for (i=2 ; i<saTable.GetCount() ; ) {
		strTitleArray.Add(saTable[i++]);
		strLinkArray.Add(saTable[i++]);
	}
}

/////////////////////////////////////////////////////////////////////////////
// Mark an item as read
//
void CFeed::MarkItemRead(LPCTSTR strLink)
{
	if (!m_bDBPath)
		return;

	CString strSQL;
	strSQL.Format(_T("UPDATE FeedItem SET readstatus = 1 WHERE link = '%s'"), EscapeQuote(strLink));
	ExecSQL(strSQL);
}

bool CFeed::IsItemRead(LPCTSTR strLink)
{
	CStringArray saTable;
	CString strSQL;
	strSQL.Format(_T("SELECT DISTINCT readstatus FROM FeedItem WHERE link = '%s'"), EscapeQuote(strLink));
	GetTableSQL(strSQL, saTable);

	if ((saTable.GetCount() < 2) || (saTable[1]!=_T("1")))
		return false;

	return true;
}

/////////////////////////////////////////////////////////////////////////////
// Escape Single Quote
//
CString CFeed::EscapeQuote(CString strValue)
{
	strValue.Replace( _T("'"), _T("''") );
	return strValue;
}

/////////////////////////////////////////////////////////////////////////////
// Get Module File Path
//
CString CFeed::GetModuleFileDir()
{
	DWORD	dwLength, dwSize;
	TCHAR	szFileName [MAX_PATH];
	CString	strFileName;
	int		nPos;
	
	dwSize = sizeof (szFileName) / sizeof (szFileName [0]);
	dwLength = ::GetModuleFileName (AfxGetInstanceHandle(), szFileName, dwSize);
	if (dwLength <= 0) 
	{
		return _T("");
	}

	strFileName = szFileName;
	nPos = strFileName.ReverseFind( '\\' );
	return strFileName.Left( nPos );
}

/////////////////////////////////////////////////////////////////////////////
// Get Feed Version Information
//
void CFeed::GetVersion(MSXML2::IXMLDOMNode *pNode)
{
	ASSERT(pNode);
}

/////////////////////////////////////////////////////////////////////////////
// Iterate Child Node
//
void CFeed::IterateChildNodes(MSXML2::IXMLDOMNode *pNode)
{
	BSTR		bstrNodeName;

	if ( pNode )
	{
		m_nDepth++;
		CString strOutput;
		pNode->get_nodeName(&bstrNodeName);

		//
		// Find out the node type (as a string).
		//
		BSTR bstrNodeType;
		pNode->get_nodeTypeString(&bstrNodeType);
		CString strType;
		strType = CString( bstrNodeType );
		SysFreeString(bstrNodeType);

		MSXML2::DOMNodeType eEnum;
		pNode->get_nodeType(&eEnum);

		CString strValue;
		BSTR bstrValue;
		switch( eEnum )
		{
			case MSXML2::NODE_TEXT:
			{
				// Text string in the XML document
				BSTR bstrValue;
				pNode->get_text(&bstrValue);
				strOutput = CString( bstrValue );
				SysFreeString(bstrValue);
				break;
			}
			case MSXML2::NODE_COMMENT:
			{
				// Comment in the XML document
				VARIANT vValue;
				pNode->get_nodeValue(&vValue);
				VariantClear(&vValue);
				break;
			}
			case MSXML2::NODE_PROCESSING_INSTRUCTION:
			{
				// Processing instruction
				strOutput = CString( bstrNodeName );
				break;
			}
			case MSXML2::NODE_ELEMENT:
			{
				// Element
				strOutput = CString( bstrNodeName );
				if ( strOutput == _T("rss") )
				{
					GetVersion( pNode );
				}
				else if ( strOutput == _T("copyright") )
				{
					pNode->get_text(&bstrValue);
					m_source.m_strCopyright = CString( bstrValue );
				}
				else if ( strOutput == _T("title") && m_nDepth == 4 )
				{
					pNode->get_text(&bstrValue);
					m_source.m_strTitle = CString( bstrValue );
				}
				else if ( strOutput == _T("link") && m_nDepth == 4 )
				{
					pNode->get_text(&bstrValue);
					m_source.m_strLink = CString( bstrValue );
				}
				else if ( strOutput == _T("description") && m_nDepth == 4 )
				{
					pNode->get_text(&bstrValue);
					m_source.m_strDescription = CString( bstrValue );
				}
				else if ( strOutput == _T("language") )
				{
					pNode->get_text(&bstrValue);
					m_source.m_strLanguage = CString( bstrValue );
				}
				else if ( strOutput == _T("webMaster") )
				{
					pNode->get_text(&bstrValue);
					m_source.m_strWebMaster = CString( bstrValue );
				}
				else if ( strOutput == _T("lastBuildDate") )
				{
					pNode->get_text(&bstrValue);
					m_source.m_strLastBuildDate = CString( bstrValue );
				}
				else if ( strOutput == _T("ttl") )
				{
					pNode->get_text(&bstrValue);
					m_source.m_strTtl = CString( bstrValue );
				}
				else if ( strOutput == _T("generator") )
				{
					pNode->get_text(&bstrValue);
					m_source.m_strGenerator = CString( bstrValue );
				}
				else if ( strOutput == _T("image") )
				{
					BuildImage( pNode );
				}
				else if ( strOutput == _T("item") )
				{
					BuildItem( pNode );
				}
				break;
			}
			case MSXML2::NODE_DOCUMENT:
			{
				// Document
				strOutput = CString( bstrNodeName ) + _T(" - ") + CString( strType );
				break;
			}
			case MSXML2::NODE_DOCUMENT_TYPE:
			{
				// Document Type
				strOutput = CString( bstrNodeName ) + _T(" - ") + CString( strType );
				break;
			}
			case MSXML2::NODE_DOCUMENT_FRAGMENT:
			{
				// Document Fragment
				strOutput = CString( bstrNodeName ) + _T(" - ") + CString( strType );
				break;
			}
			case MSXML2::NODE_NOTATION:
			{
				// Notation
				strOutput = CString( bstrNodeName ) + _T(" - ") + CString( strType );
				break;
			}
			case MSXML2::NODE_ENTITY:
			{
				// Entity
				strOutput = CString( bstrNodeName ) + _T(" - ") + CString( strType );
				break;
			}
			case MSXML2::NODE_ENTITY_REFERENCE:
			{
				// Entity Reference
				strOutput = CString( bstrNodeName ) + _T(" - ") + CString( strType );
			}
			case MSXML2::NODE_CDATA_SECTION:
			{
				// CData section
				strOutput = CString( bstrNodeName ) + _T(" - ") + CString( strType );
			}
		}
		SysFreeString(bstrNodeName);
	}

	//
	// Any child nodes of this node need displaying too.
	//
	MSXML2::IXMLDOMNode *pNext = NULL;
	MSXML2::IXMLDOMNode *pChild;
	pNode->get_firstChild(&pChild);
	while( pChild )
	{
		IterateChildNodes(pChild);
		pChild->get_nextSibling(&pNext);
		pChild->Release();
		pChild = pNext;
	}
	m_nDepth--;
}

/////////////////////////////////////////////////////////////////////////////
// Build Image Section
//
void CFeed::BuildImage(MSXML2::IXMLDOMNode *pNode)
{
	MSXML2::IXMLDOMNode *pNext = NULL;
	MSXML2::IXMLDOMNode *pChild;
	CString		strOutput;
	BSTR		bstrNodeName;
	BSTR		bstrValue;
	pNode->get_firstChild(&pChild);
	while( pChild )
	{
		pChild->get_nodeName(&bstrNodeName);
		strOutput = CString( bstrNodeName );
		if ( strOutput == _T("title") )
		{
			pChild->get_text(&bstrValue);
			m_source.m_strImageTitle = CString( bstrValue );
		}
		else if ( strOutput == _T("url") )
		{
			pChild->get_text(&bstrValue);
			m_source.m_strImageUrl = CString( bstrValue );
		}
		else if ( strOutput == _T("link") )
		{
			pChild->get_text(&bstrValue);
			m_source.m_strImageLink = CString( bstrValue );
		}
		else if ( strOutput == _T("width") )
		{
			pChild->get_text(&bstrValue);
			m_source.m_strImageWidth = CString( bstrValue );
		}
		else if ( strOutput == _T("height") )
		{
			pChild->get_text(&bstrValue);
			m_source.m_strImageHeight = CString( bstrValue );
		}
		else if ( strOutput == _T("description") )
		{
			pChild->get_text(&bstrValue);
			m_source.m_strImageDescription = CString( bstrValue );
		}

		pChild->get_nextSibling(&pNext);
		pChild->Release();
		pChild = pNext;
	}
}

/////////////////////////////////////////////////////////////////////////////
// Build Item Section
//
void CFeed::BuildItem(MSXML2::IXMLDOMNode *pNode)
{
	MSXML2::IXMLDOMNode *pNext = NULL;
	MSXML2::IXMLDOMNode *pChild;
	CString		strOutput;
	BSTR		bstrNodeName;
	CFeedItem	item;
	BSTR		bstrValue;
	pNode->get_firstChild(&pChild);
	while( pChild )
	{
		pChild->get_nodeName(&bstrNodeName);
		strOutput = CString( bstrNodeName );
		if ( strOutput == _T("title") )
		{
			pChild->get_text(&bstrValue);
			item.m_strTitle = CString( bstrValue );
		}
		else if ( strOutput == _T("description") )
		{
			pChild->get_text(&bstrValue);
			item.m_strDescription = CString( bstrValue );
		}
		else if ( strOutput == _T("link") )
		{
			pChild->get_text(&bstrValue);
			item.m_strLink = CString( bstrValue );
		}
		else if ( strOutput == _T("author") )
		{
			pChild->get_text(&bstrValue);
			item.m_strAuthor = CString( bstrValue );
		}
		else if ( strOutput == _T("category") )
		{
			pChild->get_text(&bstrValue);
			item.m_strCategory = CString( bstrValue );
		}
		else if ( strOutput == _T("pubDate") )
		{
			pChild->get_text(&bstrValue);
			item.m_strPubDate = CString( bstrValue );
		}
		else if ( strOutput == _T("subject") )
		{
			pChild->get_text(&bstrValue);
			item.m_strSubject = CString( bstrValue );
		}

		pChild->get_nextSibling(&pNext);
		pChild->Release();
		pChild = pNext;
	}
	m_item.Add( item );
}

void CFeed::ClearLoadedItems()
{
	m_item.RemoveAll();
}

/////////////////////////////////////////////////////////////////////////////
// Execute SQL Statement and return error message if any
//
//BOOL CFeed::ExecuteSQL(_ConnectionPtr &pCnn, CString &strSQL, CString& strMsg)
//{
//	try
//	{
//		pCnn->Execute( _bstr_t( strSQL ), NULL, adCmdText );
//	}
//	catch( _com_error& e )
//	{
//		strMsg = GetComError( e );
//		return FALSE;
//	}
//	return TRUE;
//}

/////////////////////////////////////////////////////////////////////////////
// Retrieve Com Error Information
//
//CString CFeed::GetComError( _com_error& e )
//{
//	CString		strMsg;
//
//    _bstr_t bstrSource(e.Source());
//    _bstr_t bstrDescription(e.Description());
//    
//    strMsg.Format( _T("Code = %08lx\nCode meaning = %s\nSource = %s\nDescription = %s"),
//		e.Error(),
//		e.ErrorMessage(),
//		(LPCSTR) bstrSource,
//		(LPCSTR) bstrDescription );
//	return strMsg;
//}

/////////////////////////////////////////////////////////////////////////////
// Get Field Value
//
//CString CFeed::GetFieldValue(FieldsPtr fields, long nIndex)
//{
//	_variant_t vt = fields->GetItem( nIndex )->Value;
//	if ( vt.vt == VT_NULL || vt.vt == VT_EMPTY )
//	{
//		return _T("");
//	}
//	return CString( (char*)_bstr_t( vt ) );
//}

/////////////////////////////////////////////////////////////////////////////
// Delete list from Database
//
//void CFeed::DeleteListArray(CStringArray &strLinkArray)
//{
//	_ConnectionPtr		pCnn = NULL;
//	CString				strSQL;
//	CString				strMsg;
//	int					nIndex;
//
//	if (!m_bDBPath)
//		return;
//
//	// Step 1. Create object
//	pCnn.CreateInstance( __uuidof( Connection ) );
//	if ( pCnn == NULL )
//	{
//		AfxMessageBox( _T("Can not create connection object, please install MDAC!") );
//		return;
//	}
//
//	// Step 2. Open connection
//	try
//	{
//		CString		strCnn;
//		strCnn.Format( _T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s;Jet OLEDB:Database Password=philips;"), _T("") );
//		pCnn->Open( _bstr_t(strCnn), "", "", adConnectUnspecified );
//	}
//	catch( _com_error& e )
//	{
//		AfxMessageBox( _T("Can not open connection:\n") + GetComError( e ) );
//		pCnn.Release();
//		return;
//	}
//
//	// Step 3. Delete Link from FeedList table
//	for(nIndex = 0; nIndex < strLinkArray.GetSize(); nIndex++ )
//	{
//		strSQL.Format( _T("delete from FeedItem where link = '%s'"), EscapeQuote( strLinkArray[nIndex] ) );
//		ExecuteSQL( pCnn, strSQL, strMsg );
//	}
//
//	// Step 4. Done!
//	pCnn.Release();
//}

/////////////////////////////////////////////////////////////////////////////
// Delete Feed Source
//
//void CFeed::DeleteFeedSource(CString strLink)
//{
//	_ConnectionPtr		pCnn = NULL;
//	CString				strSQL;
//	CString				strMsg;
//
//	if (!m_bDBPath)
//		return;
//
//	// Step 1. Create object
//	pCnn.CreateInstance( __uuidof( Connection ) );
//	if ( pCnn == NULL )
//	{
//		AfxMessageBox( _T("Can not create connection object, please install MDAC!") );
//		return;
//	}
//
//	// Step 2. Open connection
//	try
//	{
//		CString		strCnn;
//		strCnn.Format( _T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s;Jet OLEDB:Database Password=philips;"), _T("") );
//		pCnn->Open( _bstr_t(strCnn), "", "", adConnectUnspecified );
//	}
//	catch( _com_error& e )
//	{
//		AfxMessageBox( _T("Can not open connection:\n") + GetComError( e ) );
//		pCnn.Release();
//		return;
//	}
//
//	// Step 3. Delete Link from FeedList table
//	strSQL.Format( _T("delete from FeedSource where FeedLink = '%s'"), EscapeQuote( strLink ) );
//	ExecuteSQL( pCnn, strSQL, strMsg );
//
//	// Step 4. Done!
//	pCnn.Release();
//}

/////////////////////////////////////////////////////////////////////////////
// Refresh All SubScriptions
//
//void CFeed::RefreshAll()
//{
//	_ConnectionPtr		pCnn = NULL;
//	CString				strSQL;
//	CString				strMsg;
//	CStringArray		strLinkArray;
//	_RecordsetPtr		rs = NULL;
//
//	if (!m_bDBPath)
//		return;
//
//	// Step 1. Create object
//	pCnn.CreateInstance( __uuidof( Connection ) );
//	if ( pCnn == NULL )
//	{
//		AfxMessageBox( _T("Can not create connection object, please install MDAC!") );
//		return;
//	}
//
//	// Step 2. Open connection
//	try
//	{
//		CString		strCnn;
//		strCnn.Format( _T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s;Jet OLEDB:Database Password=philips;"), _T("") );
//		pCnn->Open( _bstr_t(strCnn), "", "", adConnectUnspecified );
//	}
//	catch( _com_error& e )
//	{
//		AfxMessageBox( _T("Can not open connection:\n") + GetComError( e ) );
//		pCnn.Release();
//		return;
//	}
//
//	// Step 3. Get all the subscriptions
//	strSQL = _T("select FeedLink from FeedSource");
//	try
//	{
//		rs = pCnn->Execute( _bstr_t( strSQL ), NULL, adCmdText );
//		while ( rs != NULL && rs->adoEOF != TRUE )
//		{
//			if ( rs->adoEOF )
//			{
//				break;
//			}
//			strLinkArray.Add( GetFieldValue( rs->Fields, 0 ) );
//			rs->MoveNext();
//		}
//	}
//	catch( _com_error& e )
//	{
//		AfxMessageBox( GetComError( e ) );
//	}
//	rs.Release();
//	rs = NULL;
//
//	// Step 4. Refreah each link
//	for( int nIndex = 0; nIndex < strLinkArray.GetSize(); nIndex++ )
//	{
//		CFeed	feed( strLinkArray.GetAt( nIndex ) );
//		feed.m_source.m_strLink = strLinkArray.GetAt( nIndex );
//		feed.Save();
//	}
//
//	// Step 5. Done!
//	pCnn.Release();
//}
