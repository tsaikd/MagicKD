#pragma once

class CKDStaticPath : public CStatic
{
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CKDStaticPath)
public:
	CKDStaticPath();
	virtual ~CKDStaticPath();

	bool SignToolTipCtrl(CToolTipCtrl *ttc);

private:
	CToolTipCtrl *m_pttc;

protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};


