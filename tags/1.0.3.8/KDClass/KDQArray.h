#pragma once
#include "afxtempl.h"

template <class T, class PT>
class CKDQArray : public CArray <T, PT>
{
public:
	CKDQArray() : m_uQSortStep(0), m_fQSortGuessStep(0) {}
	virtual ~CKDQArray() {}

	bool QuickSort(bool bAscending = true, bool *pbStop = NULL)
	{
		if (GetCount() < 1)
			return false;

		INT_PTR low = 0;
		INT_PTR high = GetCount() - 1;

		m_uQSortStep = 0;
		if (high > 1000)
			m_fQSortGuessStep = -0.0000006*high*high + 0.9*high + 50;
		else
			m_fQSortGuessStep = high;

		bool bRes = _QuickSortRecursive(GetData(), low, high, bAscending, pbStop);

		m_uQSortStep = UINT_MAX;

		return bRes;
	}

	// Return value: 0 to 1
	double GetQSortProgress()
	{
		if (m_fQSortGuessStep < 1)
			m_fQSortGuessStep = 1;
		double fRes = (double)m_uQSortStep / m_fQSortGuessStep;

		if (fRes < 0)
			return 0;
		if (fRes > 1)
			return 1;
		return fRes;
	}

	void RemoveAll()
	{
		m_uQSortStep = 0;
		m_fQSortGuessStep = 0;
		return CArray::RemoveAll();
	}

protected:
	virtual int QuickSortCmp(T &x, T &y)
	{
		if (x > y)
			return 1;
		if (x < y)
			return -1;
		return 0;
	}

	virtual void QuickSortSwap(T &x, T &y)
	{
		T z;
		z = x;
		x = y;
		y = z;
	}

private:
	bool _QuickSortRecursive(T *pArr, INT_PTR d, INT_PTR h, bool bAscending = true, bool *pbStop = NULL)
	{
		bool bRes = true;
		INT_PTR i, j;
		T str;

		i = h;
		j = d;

		str = pArr[((int) ((d+h) / 2))];

		do {
			if (pbStop && (*pbStop))
				return false;

			if (bAscending) {
				while (QuickSortCmp(pArr[j], str) < 0) j++;
				while (QuickSortCmp(pArr[i], str) > 0) i--;
			} else {
				while (QuickSortCmp(pArr[j], str) > 0) j++;
				while (QuickSortCmp(pArr[i], str) < 0) i--;
			}

			if ( i >= j ) {
				if ( i != j )
					QuickSortSwap(pArr[i], pArr[j]);

				i--;
				j++;
			}
		} while (j <= i);

		if (d < i)
			bRes = bRes && _QuickSortRecursive(pArr, d, i, bAscending, pbStop);
		if (j < h)
			bRes = bRes && _QuickSortRecursive(pArr, j, h, bAscending, pbStop);

		m_uQSortStep++;

		return bRes;
	}

	UINT m_uQSortStep;
	double m_fQSortGuessStep;
};
