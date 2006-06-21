#include "StdAfx.h"

#include "PicCHTMLEventHandler.h"

CPicCHTMLEventHandler::CPicCHTMLEventHandler()
{
}

CPicCHTMLEventHandler::~CPicCHTMLEventHandler()
{
}

//void CPicCHTMLEventHandler::BeginParse(DWORD dwAppData, bool &bAbort)
//{
//	UNUSED_ALWAYS(dwAppData);
//	bAbort = false;
//}

void CPicCHTMLEventHandler::StartTag(CLiteHTMLTag *pTag, DWORD dwAppData, bool &bAbort)
{
	UNUSED_ALWAYS(dwAppData);
	CString sTagName = pTag->getTagName();

	if (sTagName == _T("img")) {
		CLiteHTMLAttributes const *pAttr = pTag->getAttributes();
		if (pAttr) {
			CString sSrcValue = pAttr->getValueFromName(_T("src"));
			if (!sSrcValue.IsEmpty())
				m_slParsedPic.AddTail(sSrcValue);
		}
	}

	bAbort = false;
}

//void CPicCHTMLEventHandler::EndTag(CLiteHTMLTag *pTag, DWORD dwAppData, bool &bAbort)
//{
//	UNUSED_ALWAYS(pTag);
//	UNUSED_ALWAYS(dwAppData);
//	bAbort = false;
//}

void CPicCHTMLEventHandler::Characters(const CString &rText, DWORD dwAppData, bool &bAbort)
{
	UNUSED_ALWAYS(rText);
	UNUSED_ALWAYS(dwAppData);
	bAbort = false;
}

//void CPicCHTMLEventHandler::Comment(const CString &rComment, DWORD dwAppData, bool &bAbort)
//{
//	UNUSED_ALWAYS(rComment);
//	UNUSED_ALWAYS(dwAppData);
//	bAbort = false;
//}

//void CPicCHTMLEventHandler::EndParse(DWORD dwAppData, bool bIsAborted)
//{
//	UNUSED_ALWAYS(dwAppData);
//	UNUSED_ALWAYS(bIsAborted);
//}
