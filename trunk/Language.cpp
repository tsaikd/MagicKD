#include "StdAfx.h"
#include "Language.h"
#include "MagicKD.h"

SLanguage g_aLanguages[] =
{
//	{LANGID_AR_AE,	FALSE,	_T("ar-AE"),	1256, 	_T("windows-1256"),	L""},					// Arabic (UAE)
//	{LANGID_BG_BG,	FALSE,	_T("bg-BG"),	1251, 	_T("windows-1251"),	L"hun"},				// Bulgarian
//	{LANGID_CA_ES,	FALSE,	_T("ca-ES"),	1252, 	_T("windows-1252"),	L""},					// Catalan
//	{LANGID_CZ_CZ,	FALSE,	_T("cz-CZ"),	1250, 	_T("windows-1250"),	L"czech"},				// Czech
//	{LANGID_DA_DK,	FALSE,	_T("da-DK"),	1252, 	_T("windows-1252"),	L"danish"},				// Danish
//	{LANGID_DE_DE,	FALSE,	_T("de-DE"),	1252, 	_T("windows-1252"),	L"german"},				// German (Germany)
//	{LANGID_EL_GR,	FALSE,	_T("el-GR"),	1253, 	_T("windows-1253"),	L"greek"},				// Greek
	{LANGID_EN_US,	TRUE,	_T("en-US"),	1252, 	_T("windows-1252"),	L"English"},			// English
//	{LANGID_ES_ES_T,FALSE,	_T("es-ES-T"),	1252, 	_T("windows-1252"), L"spanish"},			// Spanish (Castilian)
//	{LANGID_ET_EE,	FALSE,	_T("et-EE"),	1257, 	_T("windows-1257"),	L""},					// Estonian
//	{LANGID_FI_FI,	FALSE,	_T("fi-FI"),	1252, 	_T("windows-1252"),	L"finnish"},			// Finnish
//	{LANGID_FR_FR,	FALSE,	_T("fr-FR"),	1252, 	_T("windows-1252"),	L"french"},				// French (France)
//	{LANGID_GL_ES,	FALSE,	_T("gl-ES"),	1252, 	_T("windows-1252"),	L""},					// Galician
//	{LANGID_HE_IL,	FALSE,	_T("he-IL"),	1255, 	_T("windows-1255"),	L""},					// Hebrew
//	{LANGID_HU_HU,	FALSE,	_T("hu-HU"),	1250, 	_T("windows-1250"),	L"hungarian"},			// Hungarian
//	{LANGID_IT_IT,	FALSE,	_T("it-IT"),	1252, 	_T("windows-1252"),	L"italian"},			// Italian (Italy)
//	{LANGID_JP_JP,	FALSE,	_T("jp-JP"),	 932, 	_T("shift_jis"),	L"日本語"},				// Japanese
//	{LANGID_KO_KR,	FALSE,	_T("ko-KR"),	 949, 	_T("euc-kr"),		L"korean"},				// Korean
//	{LANGID_LT_LT,	FALSE,	_T("lt-LT"),	1257, 	_T("windows-1257"),	L""},					// Lithuanian
//	{LANGID_LV_LV,	FALSE,	_T("lv-LV"),	1257, 	_T("windows-1257"),	L""},					// Latvian
//	{LANGID_NB_NO,	FALSE,	_T("nb-NO"),	1252, 	_T("windows-1252"),	L"norwegian"},			// Norwegian (Bokmal)
//	{LANGID_NL_NL,	FALSE,	_T("nl-NL"),	1252, 	_T("windows-1252"),	L"dutch"},				// Dutch (Netherlands)
//	{LANGID_PL_PL,	FALSE,	_T("pl-PL"),	1250, 	_T("windows-1250"),	L"polish"},				// Polish
//	{LANGID_PT_BR,	FALSE,	_T("pt-BR"),	1252, 	_T("windows-1252"),	L"ptb"},				// Portuguese (Brazil)
//	{LANGID_PT_PT,	FALSE,	_T("pt-PT"),	1252, 	_T("windows-1252"),	L"ptg"},				// Portuguese (Portugal)
//	{LANGID_RO_RO,	FALSE,	_T("ro-RO"),	1250, 	_T("windows-1250"),	L"Română"},				// Romanian
//	{LANGID_RU_RU,	FALSE,	_T("ru-RU"),	1251, 	_T("windows-1251"),	L"russian"},			// Russian
//	{LANGID_SL_SI,	FALSE,	_T("sl-SI"),	1250, 	_T("windows-1250"),	L""},					// Slovenian
//	{LANGID_SV_SE,	FALSE,	_T("sv-SE"),	1252, 	_T("windows-1252"),	L"swedish"},			// Swedish
//	{LANGID_TR_TR,	FALSE,	_T("tr-TR"),	1254, 	_T("windows-1254"),	L"turkish"},			// Turkish
//	{LANGID_ZH_CN,	FALSE,	_T("zh-CN"),	 936, 	_T("gb2312"),		L"简体中文"},			// Chinese (P.R.C.)
	{LANGID_ZH_TW,	FALSE,	_T("zh-TW"),	 950, 	_T("big5"),			L"正體中文"},			// Chinese (Taiwan)
	{0, 0, 0, 0,  NULL, NULL}
};

HINSTANCE g_hLangDLL = NULL;
const int g_iLanguageCount = sizeof(g_aLanguages) / sizeof(SLanguage) - 1;
UINT g_uCurrentLang = 0;

CString GetResString(UINT uStringID)
{
	CString resString;
	if (g_hLangDLL != NULL)
		resString.LoadString(g_hLangDLL, uStringID);
	if (resString.IsEmpty())
		resString.LoadString(uStringID);

	return resString;
}

HINSTANCE LoadStringLib(LANGID lid)
{
	CString sDllPath;
	int i;

	for (i = 0;i < g_iLanguageCount;i++)
		if (lid == g_aLanguages[i].lid) {
			// Need to add (CString)m_sAppDir, (CString)m_sAppPath to MainApp class
			// Need to add the following lines to MainApp class InitInstance()
			// or Inherit CKDApp
			/*
	{
		TCHAR sBuffer[MAX_PATH], *ptr;
		GetModuleFileName(NULL, sBuffer, MAX_PATH);
		m_sAppPath = sBuffer;
		if (ptr = _tcsrchr(sBuffer, _T('\\'))) {
			ptr++;
			*ptr = _T('\0');
			SetCurrentDirectory(sBuffer);
			m_sAppDir = sBuffer;
		} else {
			MessageBox(NULL, _T("Can not locate the execution file!"), _T("ERROR"), MB_OK | MB_ICONERROR);
			return FALSE;
		}
	}
			*/
			sDllPath.Format(_T("%s%s.dll"), theApp.GetAppLangDir(), g_aLanguages[i].pszISOLocale);
			g_uCurrentLang = i;
			break;
		}

	FreeLangLib();

	if (!g_aLanguages[i].bSupported && g_aLanguages[i].lid != 0 && PathFileExists(sDllPath))
		g_hLangDLL = LoadLibrary(sDllPath);

	return g_hLangDLL;
}

void FreeLangLib()
{
	if (g_hLangDLL != NULL && g_hLangDLL != GetModuleHandle(NULL)) {
		VERIFY( FreeLibrary(g_hLangDLL) );
		g_hLangDLL = NULL;
	}
}

bool IsLangSupport(LANGID lid)
{
	int i;

	for (i = 0;i < g_iLanguageCount;i++)
		if (lid == g_aLanguages[i].lid)
			return true;

	return false;
}

LANGID FindDefaultLang()
{
	const LANGID sysLID = GetUserDefaultUILanguage();
	if (IsLangSupport(sysLID))
		return sysLID;
	else {
		switch (sysLID & 0x00FF) {
			case 0x04:
				if (sysLID != 0x0804)
					return LANGID_ZH_TW;
				break;
		}
	}
	return LANGID_EN_US;
}
