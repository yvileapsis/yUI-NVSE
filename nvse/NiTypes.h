#pragma once
#include "Utilities.h"

#define DECL_FLOAT_OP(op) \
	NiPoint3 operator op(const float n) const \
	{ \
		return NiPoint3(x op n, y op n, z op n); \
	} \
	NiPoint3 operator op##=(const float n) \
	{ \
		return *this = NiPoint3(x op n, y op n, z op n); \
	} \

#define DECL_VEC_OP(op) \
	NiPoint3 operator op(const NiPoint3 v) const \
	{ \
		return NiPoint3(x op v.x, y op v.y, z op v.z); \
	} \
	NiPoint3 operator op##=(const NiPoint3 v) \
	{ \
		return *this = NiPoint3(x op v.x, y op v.y, z op v.z); \
	}

#if RUNTIME

const UInt32 _NiTMap_Lookup = 0x00853130;

#endif

// 8
struct NiRTTI {
	const char* name;
	NiRTTI* parent;
};
STATIC_ASSERT(sizeof(NiRTTI) == 0x008);

class NiPoint2
{
public:
	float x;
	float y;

	void Scale(float mult)
	{
		x *= mult;
		y *= mult;
	}
};

// C
class NiPoint3
{
public:
	float x;
	float y;
	float z;

	void Scale(float scale) {
		x *= scale;
		y *= scale;
		z *= scale;
	};

	void Init(NiPoint3* point)
	{
		x = point->x;
		y = point->y;
		z = point->z;
	};

	NiPoint3() : x(0.f), y(0.f), z(0.f)
	{
	};

	NiPoint3(const float x, const float y, const float z) : x(x), y(y), z(z)
	{
	};


	DECL_FLOAT_OP(*);
	DECL_FLOAT_OP(/ );

	DECL_VEC_OP(+);
	DECL_VEC_OP(-);
	DECL_VEC_OP(*);
	DECL_VEC_OP(/ );

	float length() const
	{
		return sqrtf(x * x + y * y + z * z);
	}

	float length_sqr() const
	{
		return x * x + y * y + z * z;
	}

	NiPoint3 normal() const
	{
		const auto len = length();
		return len == 0.F ? NiPoint3() : NiPoint3(x / len, y / len, z / len);
	}

	static float dot(const NiPoint3& v1, const NiPoint3& v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	static NiPoint3 cross(const NiPoint3& v1, const NiPoint3& v2)
	{
		return NiPoint3(
			v1.y * v2.z - v1.z * v2.y,
			v1.z * v2.x - v1.x * v2.z,
			v1.x * v2.y - v1.y * v2.x);
	}

	NiPoint3* Add(NiPoint3* toAdd)
	{
		this->x += toAdd->x;
		this->y += toAdd->y;
		this->z += toAdd->z;
		return this;
	}

	NiPoint3* Subtract(NiPoint3* point)
	{
		this->x -= point->x;
		this->y -= point->y;
		this->z -= point->z;
		return this;
	}

	float CalculateDistSquared(NiPoint3* to)
	{
		float deltaX = (x - to->x);
		float deltaY = (y - to->y);
		float deltaZ = (z - to->z);

		return deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ;
	}

	bool Equals(NiPoint3* compare)
	{
		return x == compare->x && y == compare->y && z == compare->z;
	}

	bool FltEquals(NiPoint3* compare)
	{
		if (abs(x - compare->x) > FLT_EPSILON) return false;
		if (abs(y - compare->y) > FLT_EPSILON) return false;
		return (abs(z - compare->z) <= FLT_EPSILON);
	}
};
STATIC_ASSERT(sizeof(NiPoint3) == 0x00C);

// 10 - always aligned?
class NiPoint4 {
public:
	float x;
	float y;
	float z;
	float w;
};
STATIC_ASSERT(sizeof(NiPoint4) == 0x010);

// 10 - always aligned?
struct NiQuaternion
{
	float	x, y, z, w;
};

// 24
class NiMatrix33 {
public:
	union
	{
		Float32 dataAlt[9]{};
		Float32 data[3][3];
		struct
		{
			NiPoint3 x;
			NiPoint3 y;
			NiPoint3 z;
		};
	};
	NiMatrix33()
	= default;
};
STATIC_ASSERT(sizeof(NiMatrix33) == 0x024);

// 34
class NiTransform {
public:
	NiMatrix33	rot;		// 00
	NiPoint3	pos;		// 24
	float		scale;		// 30
};
STATIC_ASSERT(sizeof(NiTransform) == 0x034);

// 10
struct NiSphere
{
	float	x, y, z, radius;
};

// 1C
struct NiFrustum
{
	float	l;			// 00
	float	r;			// 04
	float	t;			// 08
	float	b;			// 0C
	float	n;			// 10
	float	f;			// 14
	UInt8	o;			// 18
	UInt8	pad19[3];	// 19
};

// 10
struct NiViewport
{
	float	l;
	float	r;
	float	t;
	float	b;
};

// C
struct NiColor
{
	float	r;
	float	g;
	float	b;
};

// 10
struct NiColorAlpha
{
	float	r;
	float	g;
	float	b;
	float	a;
};

// 10
struct NiPlane
{
	NiPoint3	nrm;
	float		offset;
};

// 10
// NiTArrays are slightly weird: they can be sparse
// this implies that they can only be used with types that can be NULL?
// not sure on the above, but some code only works if this is true
// this can obviously lead to fragmentation, but the accessors don't seem to care
// weird stuff
template <typename T_Data>
struct NiTArray
{
	virtual void* Destroy(UInt32 doFree);

	T_Data*     data;			// 04
	UInt16		capacity;		// 08 - init'd to size of preallocation
	UInt16		firstFreeEntry;	// 0A - index of the first free entry in the block of free entries at the end of the array (or numObjs if full)
	UInt16		numObjs;		// 0C - init'd to 0
	UInt16		growSize;		// 0E - init'd to size of preallocation

	T_Data operator[](UInt32 idx)
	{
		if (idx < firstFreeEntry)
			return data[idx];
		return NULL;
	}

	T_Data Get(UInt32 idx) { return data[idx]; }

	UInt16 Length() { return firstFreeEntry; }
	void AddAtIndex(UInt32 index, T_Data* item);	// no bounds checking
	void SetCapacity(UInt16 newCapacity);	// grow and copy data if needed

	class Iterator
	{
		friend NiTArray;

		T_Data* pData;
		UInt32		count;

	public:
		explicit operator bool() const { return count != 0; }
		void operator++()
		{
			pData++;
			count--;
		}

		T_Data& operator*() const { return *pData; }
		T_Data& operator->() const { return *pData; }
		T_Data& Get() const { return *pData; }

		Iterator(NiTArray& source) : pData(source.data), count(source.firstFreeEntry) {}
	};

	Iterator Begin() { return Iterator(*this); }
};

template <typename T> void NiTArray<T>::AddAtIndex(UInt32 index, T* item)
{
	ThisStdCall(0x00869640, this, index, item);
}

template <typename T> void NiTArray<T>::SetCapacity(UInt16 newCapacity)
{
	ThisStdCall(0x008696E0, this, newCapacity);
}

// 18
// an NiTArray that can go above 0xFFFF, probably with all the same weirdness
// this implies that they make fragmentable arrays with 0x10000 elements, wtf
template <typename T> class NiTLargeArray
{
public:
	NiTLargeArray();
	~NiTLargeArray();

	void	** _vtbl;		// 00
	T		* data;			// 04
	UInt32	capacity;		// 08 - init'd to size of preallocation
	UInt32	firstFreeEntry;	// 0C - index of the first free entry in the block of free entries at the end of the array (or numObjs if full)
	UInt32	numObjs;		// 10 - init'd to 0
	UInt32	growSize;		// 14 - init'd to size of preallocation

	T operator[](UInt32 idx) {
		if (idx < firstFreeEntry)
			return data[idx];
		return NULL;
	}

	T Get(UInt32 idx) { return (*this)[idx]; }

	UInt32 Length(void) { return firstFreeEntry; }
};

// 8
template <typename T> struct NiTSet
{
	T*		data;		// 00
	UInt16	capacity;	// 04
	UInt16	length;		// 06
};

// 10
template <typename T_Data> class NiTPointerMap
{
public:
	NiTPointerMap();
	virtual ~NiTPointerMap();

	struct Entry
	{
		Entry*	next;
		UInt32	key;
		T_Data*	data;
	};

	virtual void	Destroy(bool doFree);
	virtual UInt32	CalculateBucket(UInt32 key);
	virtual bool	CompareKey(UInt32 lhs, UInt32 rhs);
	virtual void	FillEntry(Entry* entry, UInt32 key, T_Data data);
	virtual void	FreeKey(Entry* entry);
	virtual Entry*	AllocNewEntry();
	virtual void	FreeEntry(Entry* entry);

	T_Data *		Lookup(UInt32 key);
	bool			Insert(Entry* nuEntry);

//	void**			_vtbl;			// 0
	UInt32			m_numBuckets;	// 4
	Entry**			m_buckets;		// 8
	UInt32			m_numItems;		// C

	// note: traverses in non-numerical order
	class Iterator
	{
		friend NiTPointerMap;

		NiTPointerMap* m_table;
		Entry* m_entry;
		Entry** m_bucket;
		
		void FindNonEmpty();

	public:

		Iterator() : m_table(nullptr), m_entry(nullptr), m_bucket(nullptr) {}
		Iterator(NiTPointerMap* table, Entry* entry = nullptr)
			: m_table(table), m_entry(entry), m_bucket(table->m_buckets) { FindNonEmpty(); }

		~Iterator() { }

		T_Data*		Get(void);
		UInt32		GetKey(void);
		bool		Next(void);
		bool		Done(void);

		//		T_Key Key() const { return m_entry->key; }

		explicit operator bool() const { return m_entry != nullptr; }
		void operator++()
		{
			m_entry = m_entry->next;
			if (!m_entry)
			{
				m_bucket++;
				FindNonEmpty();
			}
		}

		T_Data* operator->() const { return m_entry->data; }
		T_Data*& operator*() const { return m_entry->data; }
		Iterator& operator=(const Iterator& rhs)
		{
			m_entry = rhs.m_entry;
			return *this;
		}
		bool operator!=(const Iterator& rhs) { return m_entry != rhs.m_entry; }
	};

	Iterator begin() { return Iterator(this); }
	Iterator end() { return Iterator(); }

};

template <typename T_Data> T_Data* NiTPointerMap <T_Data>::Lookup(UInt32 key)
{
	for (Entry * traverse = m_buckets[key % m_numBuckets]; traverse; traverse = traverse->next)
		if (traverse->key == key)
			return traverse->data;
	return nullptr;
}

template <typename T_Data> bool NiTPointerMap<T_Data>::Insert(Entry* nuEntry)
{
	// game code does not appear to care about ordering of entries in buckets
	UInt32 bucket = nuEntry->key % m_numBuckets;
	Entry* prev = nullptr;
	for (Entry* cur = m_buckets[bucket]; cur; cur = cur->next) {
		if (cur->key == nuEntry->key) return false;
		if (!cur->next) {
			prev = cur;
			break;
		}
	}

	if (prev) prev->next = nuEntry;
	else m_buckets[bucket] = nuEntry;

	m_numBuckets++;
	return true;
}

template <typename T_Data> void NiTPointerMap<T_Data>::Iterator::FindNonEmpty()
{
	for (Entry** end = &m_table->m_buckets[m_table->m_numBuckets]; m_bucket != end; ++m_bucket)
		if ((m_entry = *m_bucket)) return;
}

template <typename T_Data> T_Data * NiTPointerMap <T_Data>::Iterator::Get(void)
{
	return m_entry ? m_entry->data : nullptr;
}

template <typename T_Data> UInt32 NiTPointerMap <T_Data>::Iterator::GetKey(void)
{
	return m_entry ? m_entry->key : 0;
}

template <typename T_Data> bool NiTPointerMap <T_Data>::Iterator::Next(void)
{
	if (m_entry) m_entry = m_entry->next;

	while (!m_entry && (m_bucket < (m_table->m_numBuckets - 1)))
	{
		++m_bucket;
		m_entry = m_table->m_buckets[m_bucket];
	}

	return m_entry != nullptr;
}

template <typename T_Data> bool NiTPointerMap <T_Data>::Iterator::Done(void)
{
	return m_entry == nullptr;
}

/*
template <typename T_Data> void NiTPointerMap <T_Data>::Iterator::FindValid(void)
{
	// validate bucket
	if(m_bucket >= m_table->m_numBuckets) return;

	// get bucket
	m_entry = m_table->m_buckets[m_bucket];

	// find non-empty bucket
	while(!m_entry && (m_bucket < (m_table->m_numBuckets - 1)))
	{
		m_bucket++;

		m_entry = m_table->m_buckets[m_bucket];
	}
}
*/

// 10
// todo: NiTPointerMap should derive from this
// cleaning that up now could cause problems, so it will wait
template <typename T_Key, typename T_Data>
class NiTMapBase
{
public:
	NiTMapBase();
	~NiTMapBase();

	struct Entry
	{
		Entry*	next;	// 000
		T_Key	key;	// 004
		T_Data	data;	// 008
	};

	virtual NiTMapBase<T_Key, T_Data>*	Destructor(bool doFree);						// 000
	virtual UInt32						Hash(T_Key key);								// 001
	virtual void						Equal(T_Key key1, T_Key key2);					// 002
	virtual void						FillEntry(Entry entry, T_Key key, T_Data data);	// 003
	virtual	void						Unk_004(void * arg0);							// 004
	virtual	void						Unk_005(void);									// 005
	virtual	void						Unk_006();										// 006

	//void**	_vtbl;	// 0
	UInt32		numBuckets;	// 4
	Entry**		buckets;	// 8
	UInt32		numItems;	// C

	DEFINE_MEMBER_FN_LONG(NiTMapBase, Lookup, bool, _NiTMap_Lookup, T_Key key, T_Data * dataOut);
};

// 14
template <typename T_Data> class NiTStringPointerMap : public NiTPointerMap <T_Data>
{
public:
	NiTStringPointerMap();
	~NiTStringPointerMap();

	UInt32	unk010;
};

// not sure how much of this is in NiTListBase and how much is in NiTPointerListBase
// 10
template <typename T> class NiTListBase
{
public:
	NiTListBase();
	~NiTListBase();

	struct Node
	{
		Node*	next;
		Node*	prev;
		T*		data;
	};

	virtual void	Destructor(void);
	virtual Node*	AllocateNode(void);
	virtual void	FreeNode(Node * node);

//	void**		_vtbl;		// 000
	Node*		start;		// 004
	Node*		end;		// 008
	UInt32		numItems;	// 00C
};

// 10
template <typename T> class NiTPointerListBase : public NiTListBase <T>
{
public:
	NiTPointerListBase();
	~NiTPointerListBase();
};

// 10
template <typename T> class NiTPointerList : public NiTPointerListBase <T>
{
public:
	NiTPointerList();
	~NiTPointerList();
};

// 4
template <typename T> class NiPointer
{
public:
	NiPointer(T *init) : data(init)		{	}

	T*		data;

	const T&	operator *() const { return *data; }
	T&			operator *() { return *data; }

	operator const T*() const { return data; }
	operator T*() { return data; }
};

// 14
template <typename T> class BSTPersistentList
{
public:
	BSTPersistentList();
	~BSTPersistentList();

	virtual void	Destroy(bool destroy);

//	void	** _vtbl;	// 00
	UInt32	unk04;		// 04
	UInt32	unk08;		// 08
	UInt32	unk0C;		// 0C
	UInt32	unk10;		// 10
};
