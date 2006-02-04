// MagicKD.cpp : 定義應用程式的類別行為。
//

#include "stdafx.h"
#include "MagicKD.h"
#include "MagicKDDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMagicKDApp

BEGIN_MESSAGE_MAP(CMagicKDApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CMagicKDApp 建構

CMagicKDApp::CMagicKDApp()
{
	// TODO: 在此加入建構程式碼，
	// 將所有重要的初始設定加入 InitInstance 中
}


// 僅有的一個 CMagicKDApp 物件

CMagicKDApp theApp;


// CMagicKDApp 初始設定

BOOL CMagicKDApp::InitInstance()
{
	CWinApp::InitInstance();

	if (IsDialogRunning())
		return FALSE;

	{
		TCHAR sBuffer[MAX_PATH], *ptr;
		GetModuleFileName(NULL, sBuffer, MAX_PATH);
		m_sAppPath = sBuffer;
		if (ptr = _tcsrchr(sBuffer, _T('\\'))) {
			ptr++;
			*ptr = _T('\0');
			SetCurrentDirectory(sBuffer);
			m_sAppDir = sBuffer;
			m_cIni.SetPathName(m_sAppDir + _T("MagicKD.ini"));
		} else {
			MessageBox(NULL, _T("Can not locate the execution file!"), _T("ERROR"), MB_OK | MB_ICONERROR);
			return FALSE;
		}
	}

	// 標準初始設定
	// 如果您不使用這些功能並且想減少最後完成的可執行檔大小，您可以從下列
	// 程式碼移除不需要的初始化常式，變更儲存設定值的登錄機碼
	// TODO: 您應該適度修改此字串 (例如，公司名稱或組織名稱)
//	SetRegistryKey(_T("本機 AppWizard 所產生的應用程式"));

	CMagicKDDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置於使用 [確定] 來停止使用對話方塊時
		// 處理的程式碼
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置於使用 [取消] 來停止使用對話方塊時
		// 處理的程式碼
	}

	// 因為已經關閉對話方塊，傳回 FALSE，所以我們會結束應用程式，
	// 而非提示開始應用程式的訊息。
	return FALSE;
}
