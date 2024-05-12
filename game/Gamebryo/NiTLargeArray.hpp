#pragma once

#include "NiMemObject.hpp"
#include "NiTMallocInterface.hpp"
#include "NiTNewInterface.hpp"

template <class T_Data, class Allocator>
class NiTLargeArray : public NiMemObject {
public:
	NiTLargeArray(UInt32 uiMaxSize = 0, UInt32 uiGrowBy = 1) {
		m_uiMaxSize = uiMaxSize;
		m_uiGrowBy = uiGrowBy;
		m_uiSize = 0;
		m_uiESize = 0;

		if (m_uiMaxSize > 0)
			m_pBase = Allocator::Allocate(m_uiMaxSize);
		else
			m_pBase = 0;
	}

	virtual ~NiTLargeArray() {
		Allocator::Deallocate(m_pBase);
	};

	T_Data* m_pBase;
	UInt32 m_uiMaxSize;
	UInt32 m_uiSize;
	UInt32 m_uiESize;
	UInt32 m_uiGrowBy;

	T_Data operator[](UInt32 idx) {
		if (idx < m_uiSize)
			return m_pBase[idx];
		return NULL;
	}

	inline T_Data Get(UInt32 idx) { return (*this)[idx]; }

	inline UInt32 GetEffectiveSize() { return m_uiESize; }
	inline UInt32 GetSize() { return m_uiSize; }

	// 0xCA47F0, 0xA64FE0
	void SetSize(UInt32 uiMaxSize) {
		if (uiMaxSize == m_uiMaxSize)
			return;

		UInt32 i;
		if (uiMaxSize < m_uiSize) {
			for (i = uiMaxSize; i < m_uiSize; i++) {
				if (m_pBase[i] != T_Data(0)) {
					m_pBase[i] = T_Data(0);
					m_uiESize--;
				}
			}
			m_uiSize = uiMaxSize;
		}

		T_Data* pSaveBase = m_pBase;
		m_uiMaxSize = uiMaxSize;
		if (uiMaxSize > 0) {
			m_pBase = (T_Data*)Allocator::Allocate(m_uiMaxSize);
			for (i = 0; i < m_uiSize; i++) {
				m_pBase[i] = pSaveBase[i];
			}
			for (i = m_uiSize; i < m_uiMaxSize; i++) {
				m_pBase[i] = T_Data(0);
			}
		}
		else {
			m_pBase = 0;
		}
		Allocator::Deallocate(pSaveBase);
	}

	// 0x61BD20
	void SetAt(UInt32 uiIndex, const T_Data& element) {
		if (uiIndex >= m_uiSize) {
			m_uiSize = uiIndex + 1;
			if (element != T_Data(0))
				m_uiESize++;
		}
		else {
			if (element != T_Data(0)) {
				if (m_pBase[uiIndex] == T_Data(0))
					m_uiESize++;
			}
			else if (m_pBase[uiIndex] != T_Data(0))
				m_uiESize--;
		}

		m_pBase[uiIndex] = element;
	}

	// 0x864240
	UInt32 SetAtGrow(UInt32 uiIndex, const T_Data& element) {
		if (uiIndex >= m_uiMaxSize) {
			SetSize(uiIndex + m_uiGrowBy);
		}

		SetAt(uiIndex, element);
		return uiIndex;
	}

	// 0x863D90
	UInt32 Add(const T_Data& element) {
		return SetAtGrow(m_uiSize, element);
	}

	// 0xA65F80, 0x470140
	UInt32 AddFirstEmpty(const T_Data& element) {
		if (element == T_Data(0))
			return 0xffffffff;

		for (UInt32 i = 0; i < m_uiSize; i++) {
			if (m_pBase[i] == T_Data(0)) {
				// empty slot - add here
				m_pBase[i] = element;
				m_uiESize++;
				return i;
			}
		}

		// no empty slots - add at end
		return SetAtGrow(m_uiSize, element);
	}

	T_Data RemoveAt(UInt32 uiIndex) {
		if (uiIndex >= m_uiSize)
			return T_Data(0);

		T_Data element = m_pBase[uiIndex];
		m_pBase[uiIndex] = T_Data(0);

		if (element != T_Data(0))
			m_uiESize--;

		if (uiIndex == (UInt32)(m_uiSize - 1))
			m_uiSize--;

		return element;
	}

	inline T_Data& GetAt(UInt32 uiIndex) {
		return m_pBase[uiIndex];
	}

	T_Data RemoveAtAndFill(UInt32 uiIndex)
	{
		if (uiIndex >= m_uiSize)
			return T_Data(0);

		m_uiSize--;
		T_Data element = m_pBase[uiIndex];

		m_pBase[uiIndex] = m_pBase[m_uiSize];
		m_pBase[m_uiSize] = T_Data(0);

		if (element != T_Data(0))
			m_uiESize--;

		return element;
	}

	T_Data RemoveEnd() {
		if (m_uiSize == 0)
			return T_Data(0);

		m_uiSize--;
		T_Data element = m_pBase[m_uiSize];
		m_pBase[m_uiSize] = T_Data(0);

		if (element != T_Data(0))
			m_uiESize--;

		return element;
	}

	// 0x863DB0, 0xA64250
	void RemoveAll() {
		for (UInt32 i = 0; i < m_uiSize; i++)
			m_pBase[i] = T_Data(0);

		m_uiSize = 0;
		m_uiESize = 0;
	}

	UInt32 Remove(const T_Data& element) {
		if (element != T_Data(0)) {
			for (UInt32 i = 0; i < m_uiSize; i++) {
				if (m_pBase[i] == element) {
					m_pBase[i] = T_Data(0);

					m_uiESize--;
					if (i == m_uiSize - 1)
						m_uiSize--;

					return i;
				}
			}
		}

		return (UInt32)~0;
	}

	// 0x61BDC0
	void Compact() {
		if (m_uiESize == m_uiSize)
			return;

		if (m_uiESize) {
			for (UInt32 i = 0, j = 0; i < m_uiSize; i++) {
				if (m_pBase[i] != T_Data(0)) {
					if (m_pBase[j] != m_pBase[i])
						m_pBase[j] = m_pBase[i];
					j++;
				}
			}
		}

		T_Data* pSaveBase = m_pBase;
		m_uiSize = m_uiESize;
		m_uiMaxSize = m_uiSize;
		if (m_uiMaxSize > 0) {
			m_pBase = Allocator::Allocator(m_uiMaxSize);

			for (UInt32 i = 0; i < m_uiSize; i++)
				m_pBase[i] = pSaveBase[i];
		}
		else
			m_pBase = 0;

		Allocator::Deallocate(pSaveBase);
	}
};

template <class T_Data>
class NiTLargeObjectArray : public NiTLargeArray<T_Data, NiTNewInterface<T_Data>> {
};

template <class T_Data>
class NiTLargePrimitiveArray : public NiTLargeArray<T_Data, NiTMallocInterface<T_Data>> {
};

ASSERT_SIZE(NiTLargeObjectArray<void*>, 0x18);
ASSERT_SIZE(NiTLargePrimitiveArray<void*>, 0x18);