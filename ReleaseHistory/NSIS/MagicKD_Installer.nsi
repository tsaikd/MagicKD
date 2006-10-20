!define APPNAME "MagicKD"
!define APPVER "1.0.3.9"
!define INSTNAME "${APPNAME}-${APPVER} Installer"
!define SRCDIR "C:\Documents and Settings\tsaikd\桌面\picture\MagicKD"

Name "${INSTNAME}"
OutFile "${INSTNAME}.exe"
InstallDir "$PROGRAMFILES\${APPNAME}"
VIAddVersionKey "LegalCopyright" "KD"
VIAddVersionKey "FileDescription" "${APPNAME} installer"
VIAddVersionKey "FileVersion" ${APPVER}
VIProductVersion ${APPVER}

XPStyle on

!include "MUI.nsh"
!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Header\nsis.bmp" ; optional
!define MUI_ABORTWARNING
!insertmacro MUI_RESERVEFILE_LANGDLL

!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_INSTFILES

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

# first language is the default language
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "TradChinese"

# KDGetVer
#	Input[1]: the file path that you want to get version
#	Output[1]: the file version (X.X.X.X)
Function KDGetVer
	Exch $0
	Push $R0
	Push $R1
	Push $R2
	Push $R3
	Push $R4
	Push $R5

	GetDllVersion $0 $R0 $R1
	IntOp $R2 $R0 / 0x00010000
	IntOp $R3 $R0 & 0x0000FFFF
	IntOp $R4 $R1 / 0x00010000
	IntOp $R5 $R1 & 0x0000FFFF
	StrCpy $0 "$R2.$R3.$R4.$R5"

	Pop $R5
	Pop $R4
	Pop $R3
	Pop $R2
	Pop $R1
	Pop $R0
	Exch $0
FunctionEnd

LangString DESC_InstType1 ${LANG_ENGLISH} "Full"
LangString DESC_InstType1 ${LANG_TRADCHINESE} "完全安裝"

LangString DESC_InstType2 ${LANG_ENGLISH} "Minimal"
LangString DESC_InstType2 ${LANG_TRADCHINESE} "最小安裝"

LangString DESC_SecMagicKD ${LANG_ENGLISH} "The main application of MagicKD"
LangString DESC_SecMagicKD ${LANG_TRADCHINESE} "MagicKD 主程式"

LangString DESC_GrpLang ${LANG_ENGLISH} "Languages"
LangString DESC_GrpLang ${LANG_TRADCHINESE} "語系檔"

LangString DESC_Sec_zh_TW ${LANG_ENGLISH} "Traditional Chinese"
LangString DESC_Sec_zh_TW ${LANG_TRADCHINESE} "正體中文"

LangString DESC_SecShortCut ${LANG_ENGLISH} "Create ShortCut"
LangString DESC_SecShortCut ${LANG_TRADCHINESE} "建立捷徑"

InstType $(DESC_InstType1)
InstType $(DESC_InstType2)

Function .onInit
	!insertmacro MUI_LANGDLL_DISPLAY
FunctionEnd

Section "MagicKD" SecMagicKD
	SectionIn 1 2 RO

	SetOutPath "$INSTDIR"
	File "${SRCDIR}\MagicKD.exe"
	File "${SRCDIR}\KDUpdater.exe"

	SetOutPath "$INSTDIR\lib"
	File "${SRCDIR}\lib\cximage.dll"
	File "${SRCDIR}\lib\Icons.dll"
	File "${SRCDIR}\lib\sqlite.dll"

	WriteUninstaller "$INSTDIR\Uninstall.exe"
SectionEnd

SectionGroup /e $(DESC_GrpLang)

Section $(DESC_Sec_zh_TW) Sec_zh_TW
	SectionIn 1
	SetOutPath "$INSTDIR\lang"
	File "${SRCDIR}\lang\zh-TW.dll"
SectionEnd

SectionGroupEnd

Section $(DESC_SecShortCut) SecShortCut
	SectionIn 1
	CreateDirectory "$SMPROGRAMS\MagicKD"
	CreateShortCut "$SMPROGRAMS\MagicKD\MagicKD.lnk" "$INSTDIR\MagicKD.exe"
	CreateShortCut "$SMPROGRAMS\MagicKD\Uninstall.lnk" "$INSTDIR\Uninstall.exe"
SectionEnd

!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
	!insertmacro MUI_DESCRIPTION_TEXT ${SecMagicKD} $(DESC_SecMagicKD)
	!insertmacro MUI_DESCRIPTION_TEXT ${Sec_zh_TW} $(DESC_Sec_zh_TW)
	!insertmacro MUI_DESCRIPTION_TEXT ${SecShortCut} $(DESC_SecShortCut)
!insertmacro MUI_FUNCTION_DESCRIPTION_END

Section "Uninstall"
	Delete "$INSTDIR\MagicKD.exe"
	Delete "$INSTDIR\KDUpdater.exe"

	Delete "$INSTDIR\lib\cximage.dll"
	Delete "$INSTDIR\lib\Icons.dll"
	Delete "$INSTDIR\lib\sqlite.dll"
	RMDir "$INSTDIR\lib"

	Delete "$INSTDIR\lang\zh-TW.dll"
	RMDir "$INSTDIR\lang"

	Delete "$INSTDIR\Uninstall.exe"
	RMDir "$INSTDIR"

	Delete "$SMPROGRAMS\MagicKD\MagicKD.lnk"
	Delete "$SMPROGRAMS\MagicKD\Uninstall.lnk"
	RMDir "$SMPROGRAMS\MagicKD"
SectionEnd
