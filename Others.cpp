#include "Others.h"
#include "StdAfx.h"
#include "MagicKD.h"
#include "Language.h"

bool ChooseFolder(LPTSTR sFolder, HWND hWnd/*= 0*/)
{
	LPMALLOC pMalloc;
	bool bOK = false;

	if (SHGetMalloc(&pMalloc) == NOERROR) {
		CResString sTitle(IDS_SEL_FOLDER);
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

int PosToInt(POSITION pos)
{
	return (int)pos - 1;
}
