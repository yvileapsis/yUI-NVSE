#pragma once
template <class T_Data> class NiTSet {
public:
	NiTSet();
	~NiTSet();

	T_Data* m_pBase;
	UInt32 m_uiAlloced;
	UInt32 m_uiUsed;

	struct Iterator
	{
		friend NiTSet;

		UInt32 index;
		NiTSet* set;

		T_Data& operator*() const { return set->m_pBase[index]; }

		Iterator& operator++()
		{
			index++;
			return *this;
		}

		bool operator!=(const Iterator& other)
		{
			return index != other.index;
		}
	};

	Iterator begin()
	{
		return Iterator{ 0, this };
	}

	Iterator end()
	{
		return Iterator{ m_uiUsed, this };
	}
};

template <class T_Data> class NiTObjectSet : public NiTSet<T_Data> {};

template <class T_Data> class NiTPrimitiveSet : public NiTSet<T_Data> {};

typedef NiTPrimitiveSet<UInt32> NiUnsignedIntSet;
typedef NiTPrimitiveSet<UInt16> NiUnsignedShortSet;
typedef NiTPrimitiveSet<float> NiFloatSet;