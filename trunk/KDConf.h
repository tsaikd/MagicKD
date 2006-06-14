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
		m_mux.Lock();

		m_tContext = tDefault;
		m_bDirty = false;

		m_mux.Unlock();
	}

	void Destroy()
	{
		m_mux.Lock();

		m_bDirty = false;

		m_mux.Unlock();
	}

	_Type Get()
	{
		m_mux.Lock();

		_Type tBuf = m_tContext;

		m_mux.Unlock();
		return tBuf;
	}

	_Type Set(_Type tValue)
	{
		m_mux.Lock();

		m_tContext = tValue;
		m_bDirty = true;

		m_mux.Unlock();
		return m_tContext;
	}

	inline bool IsDirty() { return m_bDirty; }
	inline operator _Type () { return Get(); }
	inline TKDConf& operator = (_Type tValue) { Set(tValue); return (*this); }

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

	void Init(CIni *pIni, LPCTSTR lpSection, LPCTSTR lpKey, bool Default)
	{
		CKDConfType::Init(pIni, lpSection, lpKey);
		m_Default = Default;

		m_Context.Init(m_pIni->GetBool(m_sSection, m_sKey, m_Default) == TRUE);
	}

	void Destroy()
	{
		if (m_Context.IsDirty()) {
			if (m_Context == m_Default)
				m_pIni->DeleteKey(m_sSection, m_sKey);
			else
				m_pIni->WriteBool(m_sSection, m_sKey, m_Context);
		}
		m_Context.Destroy();

		CKDConfType::Destroy();
	}

	inline operator bool () { return (bool)m_Context; }
	inline CKDConfBool& operator = (bool Value) { m_Context = Value; return (*this); }

private:
	bool m_Default;
	TKDConf<bool> m_Context;
};

class CKDConfUINT : public CKDConfType
{
public:
	CKDConfUINT() {}
	virtual ~CKDConfUINT() { Destroy(); }

	void Init(CIni *pIni, LPCTSTR lpSection, LPCTSTR lpKey, UINT Default)
	{
		CKDConfType::Init(pIni, lpSection, lpKey);
		m_Default = Default;

		m_Context.Init(m_pIni->GetUInt(m_sSection, m_sKey, m_Default));
	}

	void Destroy()
	{
		if (m_Context.IsDirty()) {
			if (m_Context == m_Default)
				m_pIni->DeleteKey(m_sSection, m_sKey);
			else
				m_pIni->WriteUInt(m_sSection, m_sKey, m_Context);
		}
		m_Context.Destroy();

		CKDConfType::Destroy();
	}

	inline operator UINT () { return (UINT)m_Context; }
	inline CKDConfUINT& operator = (UINT Value) { m_Context = Value; return (*this); }

private:
	UINT m_Default;
	TKDConf<UINT> m_Context;
};

class CKDConfString : public CString, public CKDConfType
{
public:
	CKDConfString() : m_bDirty(false), m_lpDefault(NULL) {}
	virtual ~CKDConfString() { Destroy(); }

	void Init(CIni *pIni, LPCTSTR lpSection, LPCTSTR lpKey, LPCTSTR lpDefault)
	{
		CKDConfType::Init(pIni, lpSection, lpKey);

		if (m_lpDefault)
			delete [] m_lpDefault;
		m_lpDefault = new TCHAR[_tcslen(lpDefault) + 1];
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

		CKDConfType::Destroy();
	}

	inline void SetDirty(bool bDirty = true) { m_bDirty = bDirty; }
	using CString::operator =;

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
