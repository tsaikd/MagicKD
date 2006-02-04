#pragma once

/*
	Inherit and	ReWrite virtual void SaveIni();

	void SaveIni()
	{
		if (!IsIniModify())
			return;

		// Add Your Config

		CKDIni::SaveIni();
	}
*/

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
