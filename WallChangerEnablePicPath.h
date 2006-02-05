#include "xImageList.h"

#ifndef _CKDStringList_
#define _CKDStringList_

class CKDStringList
	: private CStringList
{
public:
	CKDStringList() {}
	virtual ~CKDStringList() { RemoveAll(); }

	using CStringList::GetCount;
	using CStringList::Find;

	void AppendArray(CStringArray const &saAppend) {
		int iCount = saAppend.GetCount();
		for (int i=0 ; i<iCount ; i++) {
			AddTail(saAppend[i]);
			m_aPos.Add(GetTailPosition());
		}
	}

	void RemoveArray(CStringArray const &saRemove) {
		POSITION pos;
		int iCount = saRemove.GetCount();
		for (int i=0 ; i<iCount ; i++)
			RemoveFirstFind(saRemove[i]);
	}

	void RemoveAt(POSITION position) {
		int iCount = m_aPos.GetCount();
		for (int i=0 ; i<iCount ; i++) {
			if (m_aPos[i] == position) {
				m_aPos.RemoveAt(i);
				break;
			}
		}
		CStringList::RemoveAt(position);
	}

	bool RemoveFirstFind(LPCTSTR searchValue, POSITION startAfter = 0) {
		POSITION pos = Find(searchValue, startAfter);
		if (pos) {
			RemoveAt(pos);
			return true;
		} else {
			return false;
		}
	}

	CString const GetRandPos() {
		int iCount = m_aPos.GetCount();
		if (iCount) {
			int i = rand() % iCount;
			return GetAt(m_aPos[i]);
		}
		return CString();
	}

	CArray<POSITION> m_aPos;
};

#endif // _CKDStringList_

extern CKDStringList g_slWallChangerEnablePicPath;
extern CxImageList* g_pimglCachePic;
