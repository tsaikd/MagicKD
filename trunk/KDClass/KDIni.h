#pragma once
#include "Ini.h"

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
	virtual void SaveIni();

	void Init(CIni *pIni);
	void Init(LPCTSTR lpIniPathName);

	bool IsIniModify();
	void SetIniModify(bool bModify = true);

	CIni *m_pIni;

private:
	bool m_bModify;
	bool m_bNeedFreeMem;
};
