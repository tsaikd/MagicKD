#pragma once
#include "afxmt.h"
#include "Ini.h"

template <typename _Type>
class TKDConf
{
public:
	TKDConf() : m_bDirty(false) {}

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

class CKDConfBool
{
public:
	CKDConfBool() : m_pIni(NULL) {}
	~CKDConfBool() { Destroy(); }

	void Init(CIni *pIni, LPCTSTR lpSection, LPCTSTR lpKey, bool bDefault)
	{
		m_pIni = pIni;
		m_sSection = lpSection;
		m_sKey = lpKey;
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
	CIni *m_pIni;
	CString m_sSection;
	CString m_sKey;
	bool m_bDefault;
	TKDConf<bool> m_bContext;
};

class CKDConfUINT
{
public:
	CKDConfUINT() : m_pIni(NULL) {}
	~CKDConfUINT() { Destroy(); }

	void Init(CIni *pIni, LPCTSTR lpSection, LPCTSTR lpKey, UINT uDefault)
	{
		m_pIni = pIni;
		m_sSection = lpSection;
		m_sKey = lpKey;
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
	CIni *m_pIni;
	CString m_sSection;
	CString m_sKey;
	UINT m_uDefault;
	TKDConf<UINT> m_uContext;
};

class CKDConfString : public CString
{
public:
	CKDConfString() : m_pIni(NULL), m_bDirty(false) {}
	~CKDConfString() { Destroy(); }

	void Init(CIni *pIni, LPCTSTR lpSection, LPCTSTR lpKey, LPCTSTR sDefault)
	{
		m_pIni = pIni;
		m_sSection = lpSection;
		m_sKey = lpKey;
		m_sDefault = sDefault;
		SetString(m_pIni->GetString(m_sSection, m_sKey, m_sDefault));
	}

	void Destroy()
	{
		if (m_bDirty) {
			if (GetString() == m_sDefault)
				m_pIni->DeleteKey(m_sSection, m_sKey);
			else
				m_pIni->WriteString(m_sSection, m_sKey, GetString());

			m_bDirty = false;
		}
	}

	void SetDirty(bool bDirty = true)
	{
		m_bDirty = bDirty;
	}

private:
	bool m_bDirty;
	CIni *m_pIni;
	CString m_sSection;
	CString m_sKey;
	CString m_sDefault;
};

class CKDConfStringArray : public CStringArray
{
public:
	CKDConfStringArray() : m_pIni(NULL), m_bDirty(false) {}
	~CKDConfStringArray() { Destroy(); }

	void Init(CIni *pIni, LPCTSTR lpSection, LPCTSTR lpKey)
	{
		m_pIni = pIni;
		m_sSection = lpSection;
		m_sKey = lpKey;

		RemoveAll();
		m_pIni->GetArray(m_sSection, m_sKey, this);
	}

	void Destroy()
	{
		if (m_bDirty) {
			if (IsEmpty())
				m_pIni->DeleteKey(m_sSection, m_sKey);
			else
				m_pIni->WriteArray(m_sSection, m_sKey, this);

			m_bDirty = false;
		}
	}

	void SetDirty(bool bDirty = true)
	{
		m_bDirty = bDirty;
	}

private:
	bool m_bDirty;
	CIni *m_pIni;
	CString m_sSection;
	CString m_sKey;
};
