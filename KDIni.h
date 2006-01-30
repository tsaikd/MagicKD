#pragma once

class CKDIni
{
public:
	CKDIni();
	virtual ~CKDIni();

	bool IsIniModify();
	void SetIniModify(bool bModify = true);
	virtual void SaveIni();

private:
	bool m_bModify;
};
