#include "stdafx.h"
#include "MagicKD.h"
#include "PicCollector/HTMLReader/LiteHTMLReader.h"
#include "PicCHTMLEventHandler.h"

#include "PicCollectorDlg.h"

// GLobal variable
CPicCollectorDlg *g_pPicCollectorDlg = NULL;
#include "PicCConf.h"
CPicCConf *g_pPicCConf = NULL;

IMPLEMENT_DYNAMIC(CPicCollectorDlg, CDialog)
CPicCollectorDlg::CPicCollectorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPicCollectorDlg::IDD, pParent)
{
	g_pPicCollectorDlg = this;
}

CPicCollectorDlg::~CPicCollectorDlg()
{
	g_pPicCollectorDlg = NULL;
}

BOOL CPicCollectorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CoInitialize(NULL);

	CString sIniPath;
	sIniPath.Format(_T("%sPicCollector.ini"), theApp.GetAppConfDir());
	m_Ini.SetPathName(sIniPath);
	g_pPicCConf = new CPicCConf;
	g_pPicCConf->Init(&m_Ini);
	m_Feed.SetDBPath(_T(".\\PicCollector.db"));
	m_list_Feed.Init();

	if (PathIsDirectory(g_pPicCConf->m_sDlDir))
		GetDlgItem(IDC_PICC_STATIC_DLDIR)->SetWindowText(g_pPicCConf->m_sDlDir);

	m_HTMLReader.setEventHandler(&m_HTMLEventHandler);

	AddNewFeed(_T("http://forum.p2pzone.org/rss.php?fid=13&limit=10&auth=AFcMVFwHMAwJUFAFUVsD"), _T("test1"));
	RefreshAllFeed();
//	m_Feed.LoadLocal(_T("http://forum.p2pzone.org/rss.php?fid=13&limit=10&auth=AFcMVFwHMAwJUFAFUVsD"));
//	rd.Read(m_Feed.m_item[0].m_strDescription);
	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPicCollectorDlg::OnDestroy()
{
	CDialog::OnDestroy();
	CoUninitialize();

	DEL(g_pPicCConf);
}

void CPicCollectorDlg::Localize()
{
}

void CPicCollectorDlg::AddNewFeed(LPCTSTR lpURL, LPCTSTR lpLocalName)
{
	CString strSQL;
	strSQL.Format(_T("INSERT INTO PicFeed (FeedLink, name) values ('%s', '%s');"),
		m_Feed.EscapeQuote(lpURL),
		m_Feed.EscapeQuote(lpLocalName));
	m_Feed.ExecSQL(strSQL);
	RefreshFeed(lpURL);
}

void CPicCollectorDlg::RefreshFeed(LPCTSTR lpURL)
{
	m_Feed.BuildFromFile(lpURL);
	m_Feed.Save();

	CTime curTime(time(0));
	CString strLink, strName, strLocalPath;
	UINT uPicNum = 1;
	int i;
	for (i=0 ; i<m_Feed.m_item.GetCount() ; i++) {
		strLink = m_Feed.m_item[i].m_strLink;
		if (!m_Feed.IsItemRead(strLink)) {
			strName = m_Feed.GetFeedName(lpURL);
			if (strName.IsEmpty())
				return;

			// Parse HTML statement
			m_HTMLReader.Read(m_Feed.m_item[i].m_strDescription);

			// check download list for black list
			// Create serial number for this link
			uPicNum = m_Ini.GetUInt(_T("PicNum"), strName, 1);

			// check necessary dir
			strLocalPath.Format(_T("%s\\%s\\%s"), CString(g_pPicCConf->m_sDlDir), strName, curTime.Format(_T("%Y_%m\\%d")));
			// download to local own dir and clear old list
			strLocalPath.AppendFormat(_T("\\%05d.jpg"), uPicNum);

			m_Ini.WriteUInt(_T("PicNum"), strName, uPicNum+1);
			m_Feed.MarkItemRead(strLink);
		}
	}
}

void CPicCollectorDlg::RefreshAllFeed()
{
	int i;
	CStringArray strTitleArray, strLinkArray;

	m_Feed.GetFeedSourceList(strTitleArray, strLinkArray);
	for (i=0 ; i<strLinkArray.GetCount() ; i++)
		RefreshFeed(strLinkArray[i]);
}

BEGIN_MESSAGE_MAP(CPicCollectorDlg, CDialog)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_PICC_BTN_CHANGEDLDIR, &CPicCollectorDlg::OnBnClickedPiccBtnChangedldir)
END_MESSAGE_MAP()

void CPicCollectorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICC_LIST_FEED, m_list_Feed);
}

void CPicCollectorDlg::OnOK()
{
//	CDialog::OnOK();
}

void CPicCollectorDlg::OnCancel()
{
	((CDialog *)GetParent())->EndDialog(IDCANCEL);

//	CDialog::OnCancel();
}

void CPicCollectorDlg::OnBnClickedPiccBtnChangedldir()
{
	CString sDlDir;
	if (ChooseFolder(sDlDir, GetSafeHwnd())) {
		if (PathIsDirectory(sDlDir)) {
			GetDlgItem(IDC_PICC_STATIC_DLDIR)->SetWindowText(sDlDir);
			g_pPicCConf->m_sDlDir = sDlDir;
			g_pPicCConf->m_sDlDir.SetDirty();
		}
	}
}
