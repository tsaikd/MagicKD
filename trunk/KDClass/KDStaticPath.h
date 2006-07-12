#pragma once

class CKDStaticPath : public CStatic
{
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CKDStaticPath)
public:
	CKDStaticPath();
	virtual ~CKDStaticPath();

	bool SignToolTipCtrl(CToolTipCtrl *ttc);
	void UnSignToolTipCtrl();

private:
	CToolTipCtrl *m_pttc;

protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};


