#pragma once
#include "PicCollector/HTMLReader/LiteHTMLReader.h"

class CPicCHTMLEventHandler : public ILiteHTMLReaderEvents
{
public:
	CPicCHTMLEventHandler();
	virtual ~CPicCHTMLEventHandler();

	CStringList m_slParsedPic;

protected:
	//virtual void BeginParse(DWORD dwAppData, bool &bAbort);
	virtual void StartTag(CLiteHTMLTag *pTag, DWORD dwAppData, bool &bAbort);
	//virtual void EndTag(CLiteHTMLTag *pTag, DWORD dwAppData, bool &bAbort);
	virtual void Characters(const CString &rText, DWORD dwAppData, bool &bAbort);
	//virtual void Comment(const CString &rComment, DWORD dwAppData, bool &bAbort);
	//virtual void EndParse(DWORD dwAppData, bool bIsAborted);
};
