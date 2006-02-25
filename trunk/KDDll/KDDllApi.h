/*
	Link to KDDll.lib

	#include "KDDllApi.h"
*/
#ifdef KDDLL_EXPORTS
#define KDDLL_API __declspec(dllexport)
#else
#define KDDLL_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/*
Usage Example:
	CStringArray saBtn;
	saBtn.SetSize(5);
	c[0] = _T("Btn0");
	c[1] = _T("Btn1");
	c[2] = _T("Btn2");
	c[3] = _T("Btn3");
	c[4] = _T("Btn4");
	int iRes = CKDMessageBox(_T("Title"), _T("This is a KDMessageBox\nPlease Select a Button!"), saBtn
							, this, 1, 2, KDMSG_F_CANCANCEL);
	if (iRes == 1)
		...
		...
*/
#define KDMSG_IDNOCHOICE	0x00

#define KDMSG_F_CANCANCEL	0x01

class KDDLL_API CKDMessageBox
{
public:
	CKDMessageBox(LPCTSTR lpTitle, LPCTSTR lpHelpText, CStringArray &saBtnText, CWnd *pParentWnd = NULL,
		int iOnOkBtn = -1, int iOnCancelBtn = -1, UINT uFlag = 0);
	~CKDMessageBox();

	operator int ();

private:
	void *m_pMsgBox;
};

#ifdef __cplusplus
}
#endif
