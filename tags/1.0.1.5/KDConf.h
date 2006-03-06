#pragma once
#include "afxmt.h"
#include "KDIni.h"

template <typename _Type>
class TKDConf
{
public:
	TKDConf() : m_bDirty(false) {}
	virtual ~TKDConf() { Destroy(); }

	void Init(_Type tDefault)
	{
		if (m_mux.Lock()) {
			m_tContext = tDefault;

			m_mux.Unlock();
		}
	}

	void Destroy()
	{
		if (m_mux.Lock()) {
			m_bDirty = false;

			m_mux.Unlock();
		}
	}

	_Type Get()
	{
		_Type tBuf = 0;
		if (m_mux.Lock()) {
			tBuf = m_tContext;

			m_mux.Unlock();
		}
		return tBuf;
	}

	_Type Set(_Type tValue)
	{
		if (m_mux.Lock()) {
			m_tContext = tValue;
			m_bDirty = true;

			m_mux.Unlock();
		}
		return m_tContext;
	}

	__inline bool IsDirty() { return m_bDirty; }
	__inline operator _Type () { return Get(); }
	__inline TKDConf& operator = (_Type tValue) { Set(tValue); return (*this); }

private:
	bool m_bDirty;
	CMutex m_mux;
	_Type m_tContext;
};

class CKDConfType
{
public:
	CKDConfType() : m_pIni(NULL) {}
	virtual ~CKDConfType() { Destroy(); }

	virtual void Init(CIni *pIni, LPCTSTR lpSection, LPCTSTR lpKey)
	{
		m_pIni = pIni;
		m_sSection = lpSection;
		m_sKey = lpKey;
	}

	virtual void Destroy() {}

protected:
	CIni *m_pIni;
	CString m_sSection;
	CString m_sKey;
};

class CKDConfBool : public CKDConfType
{
public:
	CKDConfBool() {}
	virtual ~CKDConfBool() { Destroy(); }

	void Init(CIni *pIni, LPCTSTR lpSection, LPCTSTR lpKey, bool bDefault)
	{
		CKDConfType::Init(pIni, lpSection, lpKey);
		m_bDefault = bDefault;

		m_bContext.Init(m_pIni->GetBool(m_sSection, m_sKey, m_bDefault) == TRUE);
	}

	void Destroy()
	{
		if (m_bContext.IsDirty()) {
			if (m_bContext == m_bDefault)
				m_pIni->DeleteKey(m_sSection, m_sKey);
			else
				m_pIni->WriteBool(m_sSection, m_sKey, m_bContext);
		}
		m_bContext.Destroy();
	}

	__inline operator bool () { return (bool)m_bContext; }
	__inline CKDConfBool& operator = (bool bValue) { m_bContext = bValue; return (*this); }

private:
	bool m_bDefault;
	TKDConf<bool> m_bContext;
};

class CKDConfUINT : public CKDConfType
{
public:
	CKDConfUINT() {}
	virtual ~CKDConfUINT() { Destroy(); }

	void Init(CIni *pIni, LPCTSTR lpSection, LPCTSTR lpKey, UINT uDefault)
	{
		CKDConfType::Init(pIni, lpSection, lpKey);
		m_uDefault = uDefault;

		m_uContext.Init(m_pIni->GetUInt(m_sSection, m_sKey, m_uDefault));
	}

	void Destroy()
	{
		if (m_uContext.IsDirty()) {
			if (m_uContext == m_uDefault)
				m_pIni->DeleteKey(m_sSection, m_sKey);
			else
				m_pIni->WriteUInt(m_sSection, m_sKey, m_uContext);
		}
		m_uContext.Destroy();
	}

	__inline operator UINT () { return (UINT)m_uContext; }
	__inline CKDConfUINT& operator = (UINT uValue) { m_uContext = uValue; return (*this); }

private:
	UINT m_uDefault;
	TKDConf<UINT> m_uContext;
};

class CKDConfString : public CString, public CKDConfType
{
public:
	CKDConfString() : m_bDirty(false), m_lpDefault(NULL) {}
	virtual ~CKDConfString() { Destroy(); }

	void Init(CIni *pIni, LPCTSTR lpSection, LPCTSTR lpKey, LPCTSTR lpDefault)
	{
		size_t u64Len;

		CKDConfType::Init(pIni, lpSection, lpKey);

		if (m_lpDefault)
			delete [] m_lpDefault;
		u64Len = _tcslen(lpDefault);
		m_lpDefault = new TCHAR[u64Len + 1];
		_tcscpy((LPTSTR)m_lpDefault, lpDefault);

		SetString(m_pIni->GetString(m_sSection, m_sKey, m_lpDefault));
	}

	void Destroy()
	{
		if (m_bDirty) {
			if (GetString() == CString(m_lpDefault))
				m_pIni->DeleteKey(m_sSection, m_sKey);
			else
				m_pIni->WriteString(m_sSection, m_sKey, GetString());

			m_bDirty = false;
		}

		if (m_lpDefault) {
			delete [] m_lpDefault;
			m_lpDefault = NULL;
		}
	}

	_inline void SetDirty(bool bDirty = true) { m_bDirty = bDirty; }

private:
	bool m_bDirty;
	LPCTSTR m_lpDefault;
};

class CKDConf
{
public:
	CKDConf();
	virtual ~CKDConf();

	virtual void Init(CIni *pIni);
	virtual void SaveConf();

	void AddSaveConf(CKDIni *pSaveConf);
	void RemoveSaveConf(CKDIni *pSaveConf);

protected:
	CIni *m_pIni;
	CList<CKDIni *, CKDIni *> m_listSaveConf;

private:
	bool m_bInit;
};
