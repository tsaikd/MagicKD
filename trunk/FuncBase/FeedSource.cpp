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
#include "Others.h"
#include "FeedSource.h"

#define PUGAPI_VARIANT 0x58475550	//The Pug XML library variant we are using in this implementation.
#define PUGAPI_VERSION_MAJOR 1		//The Pug XML library major version we are using in this implementation.
#define PUGAPI_VERSION_MINOR 2		//The Pug XML library minor version we are using in this implementation.
#include "pugxml.h"

using namespace pug;

class xml_outline : public xml_tree_walker
{
public:
	LPCTSTR map_to_type_name(xml_node_type type) //Convert some entity types to text.
	{
		switch(type)
		{
		case node_pcdata: return _T("PCDATA");
		case node_cdata: return _T("CDATA");
		case node_comment: return _T("COMMENT");
		case node_pi: return _T("PI");
		case node_document: return _T("DOCUMENT");
		default: return _T("UNKNOWN");
		}
	}
public:
	//Traversal begin callback.
	virtual bool begin(xml_node& node)
	{
#ifdef DEBUG
		CString sMsg;
		sMsg.Format(_T("BEGIN(%s)"), map_to_type_name(node.type()));
#endif //DEBUG
		return true;
	}
	//Traversal node callback; cumulatively outputs a simple document outline.
	virtual bool for_each(xml_node& node)
	{
#ifdef DEBUG
		CString sMsg;
		for(long i=0; i<depth(); ++i)
			sMsg.Append(_T("| "));
		if(node.has_child_nodes())
			sMsg.AppendFormat(_T("+ %s\n"), node.name());
		else if(node.type_element()||node.type_dtd_item())
			sMsg.AppendFormat(_T("- %s\n"), node.name());
		else
			sMsg.AppendFormat(_T("- (%s)\n"), map_to_type_name(node.type()));
#endif //DEBUG
		return true; //Keep traversing.
	}
	//Traversal end callback.
	virtual bool end(xml_node& node)
	{
#ifdef DEBUG
		CString sMsg;
		sMsg.Format(_T("END(%s)"), map_to_type_name(node.type()));
#endif //DEBUG
		return true;
	}
};

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
	: m_bAdded(FALSE)
{
}

CFeed::CFeed(LPCTSTR sDBPath)
	: m_bAdded(FALSE)
{
	OpenDB(sDBPath);
}

CFeed::CFeed(LPCTSTR sDBPath, LPCTSTR strXMLURL)
	: m_bAdded(FALSE)
{
	OpenDB(sDBPath);
	BuildFromFile(strXMLURL);
}

CFeed::~CFeed()
{
	CloseDB();
}

bool CFeed::OpenDB(LPCTSTR sDBPath)
{

	bool bRes = CKDSQLiteAPI::OpenDB(sDBPath);
	if (!bRes)
		return bRes;

	CKDSQLiteTable table;
	table.m_sTableName = _T("FeedItem");
	table.AddField(_T("FeedLink"));
	table.AddField(_T("title"));
	table.AddField(_T("link"), _T("VARCHAR UNIQUE"));
	table.AddField(_T("description"));
	table.AddField(_T("pubdate"));
	table.AddField(_T("author"));
	table.AddField(_T("category"));
	table.AddField(_T("subject"));
	table.AddField(_T("readstatus"));
	CheckTableField(table);

	table.Empty();
	table.m_sTableName = _T("FeedSource");
	table.AddField(_T("FeedLink"), _T("VARCHAR PRIMARY KEY"));
	table.AddField(_T("description"));
	table.AddField(_T("title"));
	table.AddField(_T("version"));
	table.AddField(_T("copyright"));
	table.AddField(_T("generator"));
	table.AddField(_T("feedlanguage"));
	table.AddField(_T("lastbuilddate"));
	table.AddField(_T("ttl"));
	table.AddField(_T("webmaster"));
	table.AddField(_T("imagedescription"));
	table.AddField(_T("imageheight"));
	table.AddField(_T("imagewidth"));
	table.AddField(_T("imagelink"));
	table.AddField(_T("imagetitle"));
	table.AddField(_T("imageurl"));
	CheckTableField(table);

	return bRes;
}

/////////////////////////////////////////////////////////////////////////////
// Build XML Feed Information from an XML File
//
// strXMLURL:		Parsed in XML File Name
// This function will build Feed Object from scratch by parsing XML Feed Information
// Result is stored in m_source and m_item objects
//
#define RETURN { CoUninitialize(); m_muxDLInet.Unlock(); return; }
void CFeed::BuildFromFile(LPCTSTR strXMLURL)
{
	if (!IsDBLoaded())
		return;

	m_muxDLInet.Lock();
	ClearLoadedItems();
	CString strTmpFile = GetModuleFileDir() + _T("\\FeedSource_tmp.xml");
	CoInitialize(NULL);

	// Step 0. Download XML File
	if (( URLDownloadToFile( NULL, strXMLURL, strTmpFile,0, NULL ) != S_OK )
		|| (!PathFileExists(strTmpFile)))
	{
		if (0 == GetOnInternet())
			AfxMessageBox(_T("Failed to download ") + CString(strXMLURL));
		RETURN;
	}
#ifdef DEBUG
	//while (!PathFileExists(strTmpFile))
	//	Sleep(2000);
#endif //DEBUG

	// Step 1. Open XML Document, if open fails, then return
	xml_parser xmlParser;
	if (!xmlParser.parse_file(strTmpFile,
		//parse_dtd_only		| //Parse only '<!DOCTYPE [*]>'
		parse_doctype			| //Parse '<!DOCTYPE ...>' section, with '[...]' as data member.
#ifndef PUGOPT_MEMFIL				  //I get all kinds of problems parsing the DTD in memory-mapped mode. KW.
		parse_dtd				| //Parse whatever is in DOCTYPE data member ('[...]').
#endif  //PUGOPT_MEMFIL
		parse_pi				| //Parse '<?...?>'
		parse_cdata				| //Parse '<![CDATA[...]]>', and '<![INCLUDE[...]]>'
		parse_comments			| //Parse <!--...-->'
#ifndef PUGOPT_MEMFIL				
		parse_wnorm				| //Normalize all entities that are flagged to be trimmed.
#endif  //PUGOPT_MEMFIL
		parse_trim_entity		| //Trim DTD entities.
		parse_trim_attribute	| //Trim 'foo="..."'.
		parse_trim_pcdata		| //Trim '>...<'
		parse_trim_cdata		| //Trim '<![CDATA[...]]>'
		parse_trim_comment		  //Trim <!--...-->'
		)) {
#ifdef DEBUG
		AfxMessageBox(_T("Parse XML File Failed!"));
#endif //DEBUG
		RETURN;
	}
	xml_outline outline;
	xml_node doc = xmlParser.document();
	doc.traverse(outline);

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
	LPCTSTR lpName = NULL;
	CString sItemName(_T("channel"));
	xml_node itelem = doc.first_element_by_name(sItemName);
	if (!itelem.empty()) {
		xml_node::child_iterator i = itelem.children_begin(); //For each child.
		xml_node::child_iterator m = itelem.children_end();
		CString sBuf;
		for(; i < m; ++i) {
			lpName = i->name();
			if (!lpName)
				continue;

			if (_tcscmp(_T("title"), lpName) == 0) {
				i->child_value(m_source.m_strTitle);
			} else if (_tcscmp(_T("link"), lpName) == 0) {
				i->child_value(m_source.m_strLink);
			} else if (_tcscmp(_T("description"), lpName) == 0) {
				i->child_value(m_source.m_strDescription);
			} else if (_tcscmp(_T("language"), lpName) == 0) {
				i->child_value(m_source.m_strLanguage);
			} else if (_tcscmp(_T("copyright"), lpName) == 0) {
				i->child_value(m_source.m_strCopyright);
			} else if (_tcscmp(_T("webMaster"), lpName) == 0) {
				i->child_value(m_source.m_strWebMaster);
			} else if (_tcscmp(_T("lastBuildDate"), lpName) == 0) {
				i->child_value(m_source.m_strLastBuildDate);
			} else if (_tcscmp(_T("ttl"), lpName) == 0) {
				i->child_value(m_source.m_strTtl);
			} else if (_tcscmp(_T("generator"), lpName) == 0) {
				i->child_value(m_source.m_strGenerator);
			} else if (_tcscmp(_T("image"), lpName) == 0) {
				xml_node::child_iterator j = i->children_begin(); //For each child.
				xml_node::child_iterator n = i->children_end();

				for(; j < n; ++j) {
					lpName = j->name();
					if (!lpName)
						continue;

					if (_tcscmp(_T("title"), lpName) == 0) {
						j->child_value(m_source.m_strImageTitle);
					} else if (_tcscmp(_T("url"), lpName) == 0) {
						j->child_value(m_source.m_strImageUrl);
					} else if (_tcscmp(_T("link"), lpName) == 0) {
						j->child_value(m_source.m_strImageLink);
					} else if (_tcscmp(_T("width"), lpName) == 0) {
						j->child_value(m_source.m_strImageWidth);
					} else if (_tcscmp(_T("height"), lpName) == 0) {
						j->child_value(m_source.m_strImageHeight);
					} else if (_tcscmp(_T("description"), lpName) == 0) {
						j->child_value(m_source.m_strImageDescription);
					}
				}
			} else if (_tcscmp(_T("item"), lpName) == 0) {
				CFeedItem item;
				xml_node::child_iterator j = i->children_begin(); //For each child.
				xml_node::child_iterator n = i->children_end();

				for(; j < n; ++j) {
					lpName = j->name();
					if (!lpName)
						continue;

					if (_tcscmp(_T("title"), lpName) == 0) {
						j->child_value(item.m_strTitle);
					} else if (_tcscmp(_T("description"), lpName) == 0) {
						j->child_value(item.m_strDescription);
					} else if (_tcscmp(_T("link"), lpName) == 0) {
						j->child_value(item.m_strLink);
					} else if (_tcscmp(_T("author"), lpName) == 0) {
						j->child_value(item.m_strAuthor);
					} else if (_tcscmp(_T("category"), lpName) == 0) {
						j->child_value(item.m_strCategory);
					} else if (_tcscmp(_T("pubDate"), lpName) == 0) {
						j->child_value(item.m_strPubDate);
					} else if (_tcscmp(_T("subject"), lpName) == 0) {
						j->child_value(item.m_strSubject);
					}
				}

				m_item.Add( item );
			}
		}
	}

	DeleteFile(strTmpFile);
	m_source.m_strLink = strXMLURL;

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

	if (!IsDBLoaded())
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

	if (!IsDBLoaded())
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

	if (!IsDBLoaded())
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
	if (!IsDBLoaded())
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

void CFeed::ClearLoadedItems()
{
	m_item.RemoveAll();
}
