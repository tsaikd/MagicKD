#pragma once

#include <shlwapi.h>
#include <winnls.h>

struct SLanguage {
	LANGID	lid;
	BOOL	bSupported;
	LPCTSTR	pszISOLocale;
	UINT	uCodepage;
	LPCTSTR	pszHtmlCharset;
	LPCWSTR pszLocale;
};

#define LANGID_AR_AE	MAKELANGID(LANG_ARABIC, SUBLANG_ARABIC_UAE)
#define LANGID_BG_BG	MAKELANGID(LANG_BULGARIAN, SUBLANG_DEFAULT)
#define LANGID_CA_ES	MAKELANGID(LANG_CATALAN, SUBLANG_DEFAULT)
#define LANGID_CZ_CZ	MAKELANGID(LANG_CZECH, SUBLANG_DEFAULT)
#define LANGID_DA_DK	MAKELANGID(LANG_DANISH, SUBLANG_DEFAULT)
#define LANGID_DE_DE	MAKELANGID(LANG_GERMAN, SUBLANG_DEFAULT)
#define LANGID_EL_GR	MAKELANGID(LANG_GREEK, SUBLANG_DEFAULT)
#define LANGID_EN_US	MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT)
#define LANGID_ES_ES_T	MAKELANGID(LANG_SPANISH, SUBLANG_SPANISH)
#define LANGID_ET_EE	MAKELANGID(LANG_ESTONIAN, SUBLANG_DEFAULT)
#define LANGID_FI_FI	MAKELANGID(LANG_FINNISH, SUBLANG_DEFAULT)
#define LANGID_FR_FR	MAKELANGID(LANG_FRENCH, SUBLANG_DEFAULT)
#define LANGID_GL_ES	MAKELANGID(LANG_GALICIAN, SUBLANG_DEFAULT)
#define LANGID_HE_IL	MAKELANGID(LANG_HEBREW, SUBLANG_DEFAULT)
#define LANGID_HU_HU	MAKELANGID(LANG_HUNGARIAN, SUBLANG_DEFAULT)
#define LANGID_IT_IT	MAKELANGID(LANG_ITALIAN, SUBLANG_DEFAULT)
#define LANGID_JP_JP	MAKELANGID(LANG_JAPANESE, SUBLANG_DEFAULT)
#define LANGID_KO_KR	MAKELANGID(LANG_KOREAN, SUBLANG_DEFAULT)
#define LANGID_LT_LT	MAKELANGID(LANG_LITHUANIAN, SUBLANG_DEFAULT)
#define LANGID_LV_LV	MAKELANGID(LANG_LATVIAN, SUBLANG_DEFAULT)
#define LANGID_NB_NO	MAKELANGID(LANG_NORWEGIAN, SUBLANG_NORWEGIAN_BOKMAL)
#define LANGID_NL_NL	MAKELANGID(LANG_DUTCH, SUBLANG_DEFAULT)
#define LANGID_PL_PL	MAKELANGID(LANG_POLISH, SUBLANG_DEFAULT)
#define LANGID_PT_BR	MAKELANGID(LANG_PORTUGUESE, SUBLANG_PORTUGUESE_BRAZILIAN)
#define LANGID_PT_PT	MAKELANGID(LANG_PORTUGUESE, SUBLANG_PORTUGUESE)
#define LANGID_RO_RO	MAKELANGID(LANG_ROMANIAN, SUBLANG_DEFAULT)
#define LANGID_RU_RU	MAKELANGID(LANG_RUSSIAN, SUBLANG_DEFAULT)
#define LANGID_SL_SI	MAKELANGID(LANG_SLOVENIAN, SUBLANG_DEFAULT)
#define LANGID_SV_SE	MAKELANGID(LANG_SWEDISH, SUBLANG_DEFAULT)
#define LANGID_TR_TR	MAKELANGID(LANG_TURKISH, SUBLANG_DEFAULT)
#define LANGID_ZH_CN	MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED)
#define LANGID_ZH_TW	MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL)

extern SLanguage g_aLanguages[];
extern const int g_iLanguageCount;
extern UINT g_uCurrentLang;
extern HINSTANCE g_hLangDLL;

CString GetResString(UINT uStringID);
HINSTANCE LoadStringLib(LANGID lid);
void FreeLangLib();
bool IsLangSupport(LANGID lid);
LANGID FindDefaultLang();

class CResString : public CString
{
public:
	__forceinline CResString() {}
	__forceinline CResString(UINT uStringID)
	{
		GetString(uStringID);
	}
	__forceinline LPCTSTR GetString(UINT uStringID)
	{
		if (g_hLangDLL != NULL)
			LoadString(g_hLangDLL, uStringID);
		if (g_hLangDLL == NULL || IsEmpty())
			LoadString(uStringID);

		return CString::GetString();
	}
	using CString::GetString;
};
