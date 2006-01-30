
#ifndef _CKDStringList_
#define _CKDStringList_

class CKDStringList : public CStringList
{
public:
	CKDStringList() {}
	virtual ~CKDStringList() {}

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
		for (int i=0 ; i<iCount ; i++) {
			if (pos = Find(saRemove[i])) {
				int jCount = m_aPos.GetCount();
				for (int j=0 ; j<jCount ; j++) {
					if (m_aPos[j] == pos) {
						m_aPos.RemoveAt(j);
						break;
					}
				}
				RemoveAt(pos);
			}
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

extern CKDStringList slWallChangerEnablePicPath;
