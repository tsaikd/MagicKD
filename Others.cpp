#include "StdAfx.h"
#ifdef IDS_SEL_FOLDER
#include "Language.h"
#endif

#include "Others.h"

// return true if user choice a folder
bool ChooseFolder(CString &sFolder, HWND hWnd/*= 0*/)
{
	LPMALLOC pMalloc;
	bool bOK = false;

	if (SHGetMalloc(&pMalloc) == NOERROR) {
#ifdef IDS_SEL_FOLDER
		CResString sTitle(IDS_SEL_FOLDER);
#else
		CString sTitle(_T("Please select a folder"));
#endif
		LPTSTR pFolder = sFolder.GetBuffer(MAX_PATH);
		BROWSEINFO brInfo = {0};
		brInfo.hwndOwner = hWnd;
		brInfo.pszDisplayName = pFolder;
		brInfo.lpszTitle = sTitle;
		brInfo.ulFlags = BIF_USENEWUI;

		CoInitialize(NULL);

		LPITEMIDLIST pidl;
		if ((pidl = SHBrowseForFolder(&brInfo)) != NULL){
			if (SHGetPathFromIDList(pidl, pFolder))
				bOK = true;
			pMalloc->Free(pidl);
		}
		pMalloc->Release();

		CoUninitialize();
	}

	return bOK;
}

bool ChooseFolder(LPTSTR sFolder, HWND hWnd/*= 0*/)
{
	LPMALLOC pMalloc;
	bool bOK = false;

	if (SHGetMalloc(&pMalloc) == NOERROR) {
#ifdef IDS_SEL_FOLDER
		CResString sTitle(IDS_SEL_FOLDER);
#else
		CString sTitle(_T("Please select a folder"));
#endif
		BROWSEINFO brInfo = {0};
		brInfo.hwndOwner = hWnd;
		brInfo.pszDisplayName = sFolder;
		brInfo.lpszTitle = sTitle;
		brInfo.ulFlags = BIF_USENEWUI;

		CoInitialize(NULL);

		LPITEMIDLIST pidl;
		if ((pidl = SHBrowseForFolder(&brInfo)) != NULL){
			if (SHGetPathFromIDList(pidl, sFolder))
				bOK = true;
			pMalloc->Free(pidl);
		}
		pMalloc->Release();

		CoUninitialize();
	}

	return bOK;
}

#include "shellapi.h"
// Open a Dialog to ask user for sure
// if not Undo, then remove files directly (default: true)
bool RemoveFileDlg(HWND hWnd, CString &sFiles, bool bUndo/* = true*/)
{
	TCHAR sFileBuf[MAX_PATH] = {0};
	_tcscpy(sFileBuf, sFiles);

	SHFILEOPSTRUCT shFile = {0};
	shFile.hwnd = hWnd;
	shFile.wFunc = FO_DELETE;
	shFile.pFrom = sFileBuf;
	if (bUndo)
		shFile.fFlags = FOF_ALLOWUNDO;
	return (SHFileOperation(&shFile)) == 0;
}

bool RemoveFileDlg(HWND hWnd, LPCTSTR sFiles, bool bUndo/* = true*/)
{
	SHFILEOPSTRUCT shFile = {0};
	shFile.hwnd = hWnd;
	shFile.wFunc = FO_DELETE;
	shFile.pFrom = sFiles;
	if (bUndo)
		shFile.fFlags = FOF_ALLOWUNDO;
	return (SHFileOperation(&shFile)) == 0;
}

// ouput lpTempFilePath and return sTempFilePath, input lpTempDir and lpPreFix
CString GetTempFilePath(LPSTR lpTempFilePath/* = NULL*/, LPCTSTR lpTempDir/* = NULL*/, LPCTSTR lpPreFix/* = NULL*/)
{
	CString sTempFilePath, sTempDir, sPreFix;
	if (lpTempFilePath)
		sTempFilePath = lpTempFilePath;

	if (lpTempDir)
		sTempDir = lpTempDir;
	else
		GetTempPath(MAX_PATH, sTempDir.GetBuffer(MAX_PATH));

	if (lpPreFix)
		sPreFix = lpPreFix;
	else
		sPreFix = _T("__TEMP_");

	GetTempFileName(sTempDir, sPreFix, 0, sTempFilePath.GetBuffer(MAX_PATH));
	DeleteFile(sTempFilePath);

	return sTempFilePath;
}

#ifdef _UNICODE

//WallPaper Options
//	WPSTYLE_CENTER
//	WPSTYLE_TILE
//	WPSTYLE_STRETCH (default)
//	WPSTYLE_MAX
bool SetWallpaper(LPCTSTR sFilePath, DWORD dwStyle/* = WPSTYLE_STRETCH*/)
{
	if (!sFilePath || !PathFileExists(sFilePath))
		return false;

	CoInitialize(NULL);

	bool bMethod1 = true, bMethod2 = true;
	IActiveDesktop *pActiveDesktop;

	#define TRYEXP(exp) { if ((exp) != S_OK) throw 1; }

	try {
		WALLPAPEROPT wpOpt;
		//Create an instance of the Active Desktop
		TRYEXP(CoCreateInstance(CLSID_ActiveDesktop, NULL, CLSCTX_INPROC_SERVER,
							IID_IActiveDesktop, (void**) &pActiveDesktop));
		TRYEXP(pActiveDesktop->SetWallpaper(sFilePath, 0));

		wpOpt.dwSize = sizeof(WALLPAPEROPT);
		wpOpt.dwStyle = dwStyle;

		TRYEXP(pActiveDesktop->SetWallpaperOptions(&wpOpt, 0));
		TRYEXP(pActiveDesktop->ApplyChanges(AD_APPLY_ALL));
	} catch (...) {
		bMethod1 = false;
	}

	#undef TRYEXP

	if (pActiveDesktop)
		pActiveDesktop->Release();
	else {
		CString cstrTileWallpaper, cstrWallpaperStyle;
		
		
		if (dwStyle == WPSTYLE_TILE)
		{
			// Tiled
			cstrTileWallpaper = _T("1");
			cstrWallpaperStyle = _T("1");
		}
		else if (dwStyle == WPSTYLE_CENTER)
		{
			// Center
			cstrTileWallpaper = _T("0");
			cstrWallpaperStyle = _T("1");
		}
		else//if (dwStyle == WPSTYLE_STRETCH)
		{
			// Stretch
			cstrTileWallpaper = _T("0");
			cstrWallpaperStyle = _T("2");
		}

		CRegKey reg;

		#define TRYREG(exp) {if ((exp) != ERROR_SUCCESS) throw 1;}

		try {
			TRYREG(reg.Create(HKEY_CURRENT_USER, _T("Control Panel\\Desktop")));
			TRYREG(reg.SetStringValue(_T("TileWallpaper"), cstrTileWallpaper));
			TRYREG(reg.SetStringValue(_T("WallpaperStyle"), cstrWallpaperStyle));

			bMethod2 = (SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (void *) sFilePath,
				SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE) != FALSE);
		} catch (...) {
			bMethod2 = false;
		}

		#undef TRYREG
	}

	CoUninitialize();

	return bMethod1 || bMethod2;
}

#endif // _UNICODE
