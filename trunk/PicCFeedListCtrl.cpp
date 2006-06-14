#include "StdAfx.h"
#include "Resource.h"
#include "FeedSource.h"

#include "PicCFeedListCtrl.h"

CPicCFeedListCtrl::CPicCFeedListCtrl()
	: m_bInit(false)
{
}

CPicCFeedListCtrl::~CPicCFeedListCtrl()
{
}

void CPicCFeedListCtrl::Init()
{
	CKDListCtrl::Init();

	CRect rcWin;
	GetClientRect(rcWin);
//	InsertColumn(0, CResString(IDS_FIND_COLUMN_FINDLIST), LVCFMT_LEFT, rcWin.right);
	InsertColumn(0, _T("Feed Name"), LVCFMT_LEFT, 100);
	InsertColumn(1, _T("Feed URL"), LVCFMT_LEFT, rcWin.right-100);

	SetExtendedStyle(GetExtendedStyle() | LVS_EX_FULLROWSELECT);

	CFeed feed(_T(".\\FreeSource.mdb"));
	CString sLink = _T("http://forum.p2pzone.org/rss.php?fid=13&limit=10&auth=AFcMVFwHMAwJUFAFUVsD");
	feed.LoadLocal(sLink);

	m_bInit = true;
}
