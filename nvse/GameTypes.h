#pragma once
#include <NiTypes.h>
#include <GameOSDepend.h>

class String
{
public:
	String();
	~String();

	char*		m_data;
	UInt16		m_dataLen;
	UInt16		m_bufLen;
	
	void		Init(UInt32 bufSize);
	bool		Set(const char* src);
	bool		Includes(const char* toFind) const;
	bool		Replace(const char* toReplace, const char* replaceWith); // replaces instance of toReplace with replaceWith
	bool		Append(const char* toAppend);
	double		Compare(const String& compareTo, bool caseSensitive = false);

	const char* CStr(void);

	void		AppendChar(char toAppend);
	void		InsertChar(char toInsert, UInt32 index);
	void		EraseAt(UInt32 index);
	void		EraseAfter(UInt32 index);
	int			EraseWord(UInt32 index);
	void		EraseNextWord(UInt32 index);
	void		RemoveLastChar();
};
STATIC_ASSERT(sizeof(String) == 0x8);

enum {
	eListCount		= -3,
	eListEnd		= -2,
	eListInvalid	= -1,
};

typedef void* (*_FormHeap_Allocate)(UInt32 size);
extern const _FormHeap_Allocate FormHeap_Allocate;

typedef void (*_FormHeap_Free)(void* ptr);
extern const _FormHeap_Free FormHeap_Free;

TESForm* __stdcall LookupFormByID(UInt32 refID);

template <typename Item> struct TListNode
{
	Item* data;
	TListNode* next;

	TListNode() : data(nullptr), next(nullptr) {}
	TListNode(Item* _data) : data(_data), next(nullptr) {}

	Item* Data() const { return data; }
	TListNode* Next() const { return next; }

	TListNode* RemoveMe()
	{
		if (next)
		{
			TListNode* pNext = next;
			data = next->data;
			next = next->next;
			GameHeapFree(pNext);
			return this;
		}
		data = NULL;
		return nullptr;
	}

	TListNode* RemoveNext()
	{
		TListNode* pNext = next;
		next = next->next;
		FormHeap_Free(pNext);
		return next;
	}

	TListNode* Append(Item* _data)
	{
		const auto newNode = static_cast<TListNode*>(FormHeap_Allocate(sizeof(TListNode)));
		newNode->data = _data;
		newNode->next = next;
		next = newNode;
		return newNode;
	}

	TListNode* Insert(Item* _data)
	{
		const auto newNode = static_cast<TListNode*>(FormHeap_Allocate(sizeof(TListNode)));
		newNode->data = data;
		data = _data;
		newNode->next = next;
		next = newNode;
		return newNode;
	}
};

template <class Item> class TList
{
public:
	typedef TListNode<Item> Node;

	Node	first;

	template <class Op>	UInt32 FreeNodes(Node* node, Op&& compareOp) const
	{
		static UInt32 nodeCount = 0, numFreed = 0, lastNumFreed = 0;
		if (node->next)
		{
			nodeCount++;
			FreeNodes(node->next, compareOp);
			nodeCount--;
		}
		if (compareOp.Accept(node->data))
		{
			node->RemoveMe();
			numFreed++;
		}
		if (!nodeCount)
		{
			lastNumFreed = numFreed;
			numFreed = 0;
		}
		return lastNumFreed;
	}

	Node* GetLastNode(SInt32* outIdx = nullptr) const
	{
		SInt32 index = 0;
		Node* node = Head();
		while (node->next)
		{
			node = node->next;
			index++;
		}
		if (outIdx) *outIdx = index;

		return node;
	}

	Node* GetNthNode(SInt32 index) const
	{
		if (index < 0) return nullptr;

		Node* node = Head();
		do
		{
			if (!index) return node;
			index--;
		}
		while ((node = node->next));

		return nullptr;
	}

	void Init(Item* item = nullptr)
	{
		first.data = item;
		first.next = NULL;
	}

	Node* Head() const { return const_cast<Node*>(&first); }
	Node* Tail() const { return const_cast<Node*>(GetLastNode()); }

	bool Empty() const { return !first.data; }

	class Iterator
	{
		Node*	m_curr;

	public:
		Iterator operator++()
		{
			if (m_curr) m_curr = m_curr->next;
			return *this;
		}
		Item* operator->() const { return m_curr->data; }
		Item*& operator*() const { return m_curr->data; }
		Iterator& operator=(const Iterator& rhs)
		{
			m_curr = rhs.m_curr;
			return *this;
		}
		bool operator!=(const Iterator& rhs) { return m_curr != rhs.m_curr; };

		Item* Get() const { return m_curr->data; }
		void Next() { if (m_curr) m_curr = m_curr->next; }
//		bool End() const { return !m_curr || (!m_curr->data && !m_curr->next); }
		void Find(Item* _item)
		{
			while (m_curr)
			{
				if (m_curr->data == _item) break;
				m_curr = m_curr->next;
			}
		}

		Iterator(Node* node = nullptr) : m_curr(node) {}
		Iterator(TList& _list) : m_curr(&_list.first) {}
		Iterator(TList* _list) : m_curr(&_list->first) {}
		Iterator(TList& _list, Item* _item) : m_curr(&_list.first) { Find(_item); }
		Iterator(TList* _list, Item* _item) : m_curr(&_list->first) { Find(_item); }
	};

	Iterator begin() const { return Iterator(Head()); }
	Iterator end() const { return Iterator(Tail()); }

	UInt32 Count() const
	{
		if (!first.data) return 0;
		Node* node = Head();
		UInt32 count = 1;
		while ((node = node->next)) count++;
		return count;
	}

	bool IsInList(Item* item) const
	{
		Node* node = Head();
		do if (node->data == item) return true;
		while ((node = node->next));
		return false;
	}

	Item* GetFirstItem() const { return first.data; }

	Item* GetLastItem() const { return GetLastNode()->data; }

	Item* GetNthItem(SInt32 index) const
	{
		if (eListEnd == index)
			return GetLastNode()->data;
		Node* node = GetNthNode(index);
		return node ? node->data : NULL;
	}

	SInt32 AddAt(Item* item, SInt32 index)
	{
		if (!item) return eListInvalid;
		if (!index)
		{
			if (first.data) first.Insert(item);
			else first.data = item;
		}
		else if (eListEnd == index)
		{
			if (Node* node = GetLastNode(&index); node->data) node->Append(item);
			else node->data = item;
		}
		else
		{
			Node* node = GetNthNode(index);
			if (!node) return eListInvalid;
			node->Insert(item);
		}
		return index;
	}

	SInt32 Append(Item* item)
	{
		SInt32 index = eListInvalid;
		if (item)
		{
			Node* node = GetLastNode(&index);
			if (node->data) node->Append(item);
			else node->data = item;
		}
		return index;
	}

	void Insert(Item* item)
	{
		if (item)
		{
			if (first.data) first.Insert(item);
			else first.data = item;
		}
	}

	void CopyFrom(TList& sourceList)
	{
		Node* target = Head(), * source = sourceList.Head();
		RemoveAll();
		if (!source->data) return;
		target->data = source->data;
		while (source = source->next)
			target = target->Append(source->data);
	}

	template <class Op>
	void Visit(Op&& op, Node* prev = nullptr) const
	{
		Node* curr = prev ? prev->next : Head();
		while (curr)
		{
			if (!curr->data || !op.Accept(curr->data)) break;
			curr = curr->next;
		}
	}

	template <class Op>
	Item* Find(Op&& op) const
	{
		Node* curr = Head();
		do
		{
			Item* pItem = curr->data;
			if (pItem && op.Accept(pItem)) return pItem;
			curr = curr->next;
		} while (curr);
		return nullptr;
	}

	template <class Op>
	Iterator Find(Op&& op, Iterator& prev) const
	{
		Iterator curIt = prev.End() ? begin() : ++prev;
		while (!curIt.End())
		{
			if (*curIt && op.Accept(*curIt)) break;
			++curIt;
		}
		return curIt;
	}

	template <class Op>
	UInt32 CountIf(Op&& op) const
	{
		UInt32 count = 0;
		Node* curr = Head();
		do if (curr->data && op.Accept(curr->data)) count++;
		while ((curr = curr->next));
		return count;
	}

	class AcceptAll
	{
	public:
		bool Accept(Item* item) { return true; }
	};

	void RemoveAll() const
	{
		Node* nextNode = Head(), * currNode = nextNode->next;
		nextNode->data = NULL;
		nextNode->next = NULL;
		while (currNode)
		{
			nextNode = currNode->next;
			GameHeapFree(currNode);
			currNode = nextNode;
		}
	}

	void DeleteAll() const
	{
		Node* nextNode = Head(), * currNode = nextNode->next;
		GameHeapFree(nextNode->data);
		nextNode->data = NULL;
		nextNode->next = NULL;
		while (currNode)
		{
			nextNode = currNode->next;
			currNode->data->~Item();
			GameHeapFree(currNode->data);
			GameHeapFree(currNode);
			currNode = nextNode;
		}
	}

	Item* RemoveNth(SInt32 idx)
	{
		Item* removed = NULL;
		if (idx <= 0)
		{
			removed = first.data;
			first.RemoveMe();
		}
		else
		{
			Node* node = Head();
			while (node->next && --idx)
				node = node->next;
			if (!idx)
			{
				removed = node->next->data;
				node->RemoveNext();
			}
		}
		return removed;
	};

	UInt32 Remove(Item* item)
	{
		UInt32 removed = 0;
		Node* curr = Head(), * prev = nullptr;
		do
		{
			if (curr->data == item)
			{
				curr = prev ? prev->RemoveNext() : curr->RemoveMe();
				removed++;
			}
			else
			{
				prev = curr;
				curr = curr->next;
			}
		} while (curr);
		return removed;
	}

	Item* ReplaceNth(SInt32 index, Item* item)
	{
		Item* replaced = nullptr;
		if (item)
		{
			Node* node;
			if (eListEnd == index)
				node = GetLastNode();
			else
			{
				node = GetNthNode(index);
				if (!node) return nullptr;
			}
			replaced = node->data;
			node->data = item;
		}
		return replaced;
	}

	UInt32 Replace(Item* item, Item* replace)
	{
		UInt32 replaced = 0;
		Node* curr = Head();
		do
		{
			if (curr->data == item)
			{
				curr->data = replace;
				replaced++;
			}
			curr = curr->next;
		} while (curr);
		return replaced;
	}

	template <class Op>
	UInt32 RemoveIf(Op&& op)
	{
		return FreeNodes(Head(), op);
	}

	SInt32 GetIndexOf(Item* item)
	{
		SInt32 idx = 0;
		Node* curr = Head();
		do { if (curr->data == item) return idx;
			idx++;
		} while ((curr = curr->next));
		return -1;
	}

	template <class Op>
	SInt32 GetIndexOf(Op&& op)
	{
		SInt32 idx = 0;
		Node* curr = Head();
		do { if (curr->data && op.Accept(curr->data)) return idx;
			idx++;
		} while ((curr = curr->next));
		return -1;
	}

	template <typename F>
	void ForEach(const F* function)
	{
		auto* node = Head();
		if (!node) return;
		do function(node->data);
		while ((node = node->next));
	}
};
STATIC_ASSERT(sizeof(TList<void*>) == 0x8);

template <typename Item> struct DListNode
{
	DListNode*		next;
	DListNode*		prev;
	Item*			data;

	DListNode* Advance(UInt32 times)
	{
		DListNode* result = this;
		while (result && times)
		{
			times--;
			result = result->next;
		}
		return result;
	}

	DListNode* Regress(UInt32 times)
	{
		DListNode* result = this;
		while (result && times)
		{
			times--;
			result = result->prev;
		}
		return result;
	}

	Item* GetAndAdvance()
	{
		Item* item = nullptr;
		if (next)
		{
			item = next->data;
			next = next->next;
		}
		return item;
	}
};

template <class Item> class DList
{
public:
	typedef DListNode<Item> Node;

private:
	Node*		first		= nullptr;
	Node*		last		= nullptr;
	UInt32		count		= 0;

public:

	class Iterator
	{
		Node* m_curr;

	public:
		Iterator operator++()
		{
			if (m_curr) m_curr = m_curr->next;
			return *this;
		}
		Item* operator->() const { return m_curr->data; }
		Item*& operator*() const { return m_curr->data; }
		Iterator& operator=(const Iterator& rhs)
		{
			m_curr = rhs.m_curr;
			return *this;
		}
		bool operator!=(const Iterator& rhs) { return m_curr != rhs.m_curr; }

		Item* Get() const { return m_curr->data; }
		void Next() { if (m_curr) m_curr = m_curr->next; }

		void Find(Item* _item)
		{
			while (m_curr)
			{
				if (m_curr->data == _item) break;
				m_curr = m_curr->next;
			}
		}

		Iterator(Node* node = nullptr) : m_curr(node) {}
		Iterator(DList& _list) : m_curr(&_list.first) {}
		Iterator(DList* _list) : m_curr(&_list->first) {}
		Iterator(DList& _list, Item* _item) : m_curr(&_list.first) { Find(_item); }
		Iterator(DList* _list, Item* _item) : m_curr(&_list->first) { Find(_item); }
	};


	bool Empty() const { return !first; }
	Node* Head() { return first; }
	Node* Tail() { return last; }
	UInt32 Size() const { return count; }

	Iterator begin() const { return Iterator(Head()); }
	Iterator end() const { return Iterator(Tail()); }

	void ExchangeNodeData(Node*, Node*);

	void Append(Item* item) { ThisCall(0x4ED8C0, this, &item); }
	
	template <typename F>
	void ForEach(const F& f)
	{
		auto* node = Head();
		for (int i = 0; i < Size(); ++i)
		{
			f(node->data);
			node = node->next;
		}
	}

	Node* Remove(Item* item)
	{
		// return the item before the removed entry, or first if previous is null
		Node* result = nullptr;

		Node* node = Head();
		if (!node) return result;

		if (node->data == item)
		{
			first = node->next;
			if (first)
				first->prev = nullptr;
			else
				last = nullptr;

			--count;
			StdCall(0x4A49E0, node);
			node = first;
		}
		else
		{
			node = node->next;
		}

		if (node)
		{
			do if (node->data == item)
				{
					result = node->prev;

					if (node->prev) 
						node->prev->next = node->next;
					if (node->next)
						node->next->prev = node->prev;
					else
						last = node->prev;
					--count;
					StdCall(0x4A49E0, node);
				}
			while ((node = node->next));
		}

		return result ? result : first;
	}

	void Sort(int (*compare)(Item* a, Item* b))
	{
		if (!first) return;
		Node* current = first;
		while (current->next)
		{
			Node* index = current->next;
			while (index)
			{
				if (compare(current->data, index->data) > 0)
				{
					Item* temp = current->data;
					current->data = index->data;
					index->data = temp;
				}
				index = index->next;
			}
			current = current->next;
		}
	}
};
STATIC_ASSERT(sizeof(DList<void*>) == 0xC);

template <class Item> void DList<Item>::ExchangeNodeData(Node* node1, Node* node2)
{
	const auto tmpdata = node1->data;
	node1->data = node2->data;
	node2->data = tmpdata;
}

enum KeyboardMenuInputCode;

// 010
template <class Item>
class BSSimpleList
{
public:
	BSSimpleList<Item>();
	~BSSimpleList<Item>();

	virtual bool	SetSelectedTile(Tile* tile) { return false; };
	virtual Tile*	GetSelectedTile(void) { return nullptr; };
	virtual Tile*	HandleKeyboardInput(KeyboardMenuInputCode code) { return nullptr; };
	virtual bool	IsMenuEqual(Menu* that) { return false; };
	virtual void	ScrollToHighlight(void) {};
	virtual Tile*	GetTileByIndex(int index, char isNotTileListIndex) { return nullptr; };
	virtual void	Destructor(bool doFree) {};
	virtual void	FreeAllTiles(void) {};
	virtual void	Sort(signed int(__cdecl*)(Item*, Item*)) {};
	
	TList<Item>	list;	// 004
};	// 00C
STATIC_ASSERT(sizeof(BSSimpleList<void*>) == 0xC);

template <typename Item>
struct BSSimpleArray
{
	virtual void	Destroy(bool doFree);
	virtual Item* Allocate(UInt32 size);

	Item*			data;			// 04
	UInt32		size;			// 08
	UInt32		alloc;			// 0C

	Item operator[](UInt32 idx)
	{
		return (idx < size) ? data[idx] : NULL;
	}

	class Iterator
	{
	protected:
		friend BSSimpleArray;

		Item*		pData;
		UInt32		count;

	public:
		bool End() const { return !count; }
		void operator++()
		{
			count--;
			pData++;
		}

		Item& operator*() const { return *pData; }
		Item& operator->() const { return *pData; }
		Item& Get() const { return *pData; }

		Iterator(): pData(nullptr), count(0) {}

		Iterator(BSSimpleArray& source) : pData(source.data), count(source.size) {}
	};


	void Append(Item* item) { ThisCall(0x7CB2E0, this, item); }
};

template <class Node, class Info>
class Visitor
{
	const Node* m_pHead;

	template <class Op>
	UInt32 FreeNodes(Node* node, Op& compareOp) const
	{
		static UInt32 nodeCount = 0;
		static UInt32 numFreed = 0;
		static Node* lastNode = nullptr;
		static bool bRemovedNext = false;

		if (node->Next())
		{
			nodeCount++;
			FreeNodes(node->Next(), compareOp);
			nodeCount--;
		}

		if (compareOp.Accept(node->Info()))
		{
			if (nodeCount)
				node->Delete();
			else
				node->DeleteHead(lastNode);
			numFreed++;
			bRemovedNext = true;
		}
		else
		{
			if (bRemovedNext)
				node->SetNext(lastNode);
			bRemovedNext = false;
			lastNode = node;
		}

		const UInt32 returnCount = numFreed;

		if (!nodeCount)	//reset vars after recursing back to head
		{
			numFreed = 0;
			lastNode = NULL;
			bRemovedNext = false;
		}

		return returnCount;
	}

	class AcceptAll {
	public:
		bool Accept(Info* info) {
			return true;
		}
	};

	class AcceptEqual {
		const Info* m_toMatch;
	public:
		AcceptEqual(const Info* info) : m_toMatch(info) { }
		bool Accept(const Info* info) {
			return info == m_toMatch;
		}
	};

	class AcceptStriCmp {
		const char* m_toMatch;
	public:
		AcceptStriCmp(const char* info) : m_toMatch(info) { }
		bool Accept(const char* info) {
			if (m_toMatch && info)
				return _stricmp(info, m_toMatch) ? false : true;
			return false;
		}
	};
public:
	Visitor(const Node* pHead) : m_pHead(pHead) { }

	UInt32 Count() const {
		UInt32 count = 0;
		const Node* pCur = m_pHead;
		while (pCur && pCur->Info() != NULL) {
			++count;
			pCur = pCur->Next();
		}
		return count;
	}

	Info* GetNthInfo(UInt32 n) const {
		UInt32 count = 0;
		const Node* pCur = m_pHead;
		while (pCur && count < n && pCur->Info() != NULL) {
			++count;
			pCur = pCur->Next();
		}
		return (count == n && pCur) ? pCur->Info() : NULL;
	}

	template <class Op>
	void Visit(Op&& op) const {
		const Node* pCur = m_pHead;
		bool bContinue = true;
		while (pCur && pCur->Info() && bContinue) {
			bContinue = op.Accept(pCur->Info());
			if (bContinue) {
				pCur = pCur->Next();
			}
		}
	}

	template <class Op>
	const Node* Find(Op&& op, const Node* prev = NULL) const
	{
		const Node* pCur;
		if (!prev)
			pCur = m_pHead;
		else
			pCur = prev->next;
		bool bFound = false;
		while (pCur && !bFound)
		{
			if (!pCur->Info())
				pCur = pCur->Next();
			else
			{
				bFound = op.Accept(pCur->Info());
				if (!bFound)
					pCur = pCur->Next();
			}
		}

		return pCur;
	}

	Node* FindInfo(const Info* toMatch) {
		return Find(AcceptEqual(toMatch));
	}

	template <class Op>
	UInt32 CountIf(Op&& op) const
	{
		UInt32 count = 0;
		const Node* pCur = m_pHead;
		while (pCur)
		{
			if (pCur->Info() && op.Accept(pCur->Info()))
				count++;
			pCur = pCur->Next();
		}
		return count;
	}

	const Node* GetLastNode() const
	{
		const Node* pCur = m_pHead;
		while (pCur && pCur->Next()) pCur = pCur->Next();
		return pCur;
	}

	void RemoveAll() const { FreeNodes(const_cast<Node*>(m_pHead), AcceptAll()); }

	template <class Op>
	UInt32 RemoveIf(Op&& op) { return FreeNodes(const_cast<Node*>(m_pHead), op); }

	bool Remove(const Info* toRemove) { return RemoveIf(AcceptEqual(toRemove)) ? true : false; }

	bool RemoveString(const char* toRemove) { return RemoveIf(AcceptStriCmp(toRemove)) ? true : false; }

	void Append(Node* newNode)
	{
		Node* lastNode = const_cast<Node*>(GetLastNode());
		if (lastNode == m_pHead && !m_pHead->Info())
			lastNode->DeleteHead(newNode);
		else
			lastNode->SetNext(newNode);
	}

	template <class Op>
	UInt32 GetIndexOf(Op&& op)
	{
		UInt32 idx = 0;
		const Node* pCur = m_pHead;
		while (pCur && pCur->Info() && !op.Accept(pCur->Info()))
		{
			idx++;
			pCur = pCur->Next();
		}

		if (pCur && pCur->Info()) return idx;
		return -1;
	}
};


struct CoordXY
{
	float	x;
	float	y;

	CoordXY(): x(0), y(0)	{}

	CoordXY(float _x, float _y) : x(_x), y(_y) {}

	inline CoordXY& operator =(const CoordXY& rhs)
	{
		*(double*)this = *(double*)&rhs;
		return *this;
	}
};

struct Semaphore
{
	UInt32 count;
	HANDLE semaphore;
	UInt32 max;
};

struct MouseFilter
{
	float targetValue = 0;
	float remainingValue = 0;
	float lastAmount = 0;

	float smooth(float delta, float scale)
	{
		targetValue += delta;
		delta = (targetValue - remainingValue) * scale;
		lastAmount += (delta - lastAmount) * 0.5F;

		if (delta > 0.0F && delta > lastAmount || delta < 0.0F && delta < lastAmount)
		{
			delta = lastAmount;
		}

		remainingValue += delta;
		return delta;
	}

	void reset()
	{
		targetValue = 0;
		remainingValue = 0;
		lastAmount = 0;
	}
};


enum class IsDXKeyState
{
	IsHeld		= 0x0,
	IsPressed	= 0x1,
	IsDepressed	= 0x2,
	IsChanged	= 0x3,
};

class  TESObjectREFR;

struct PackageInfo
{
	TESPackage*			package;		// 00
	union								// 04
	{
		TESPackageData* packageData;
		void*			actorPkgData;
	};
	TESObjectREFR*		targetRef;		// 08
	UInt32				unk0C;			// 0C	Initialized to 0FFFFFFFFh, set to 0 on start
	float				unk10;			// 10	Initialized to -1.0	. Set to GameHour on start so some time
	UInt32				flags;			// 14	Flags, bit0 would be not created and initialized
};

enum ControlType
{
	kControlType_Keyboard,
	kControlType_Mouse,
	kControlType_Joystick
};


enum AnimState
{
	kAnimState_Inactive		= 0x0,
	kAnimState_Animating	= 0x1,
	kAnimState_EaseIn		= 0x2,
	kAnimState_EaseOut		= 0x3,
	kAnimState_TransSource	= 0x4,
	kAnimState_TransDest	= 0x5,
	kAnimState_MorphSource	= 0x6,
};

enum class ControlCode
{
	Forward = 0x0,
	Backward = 0x1,
	Left = 0x2,
	Right = 0x3,
	Attack = 0x4,
	Activate = 0x5,
	Aim = 0x6,
	ReadyItem = 0x7,
	Crouch = 0x8,
	Run = 0x9,
	AlwaysRun = 0xA,
	AutoMove = 0xB,
	Jump = 0xC,
	TogglePOV = 0xD,
	MenuMode = 0xE,
	Rest = 0xF,
	VATS_ = 0x10,
	Hotkey1 = 0x11,
	AmmoSwap = 0x12,
	Hotkey3 = 0x13,
	Hotkey4 = 0x14,
	Hotkey5 = 0x15,
	Hotkey6 = 0x16,
	Hotkey7 = 0x17,
	Hotkey8 = 0x18,
	QuickSave = 0x19,
	QuickLoad = 0x1A,
	Grab = 0x1B,
	Escape = 0x1C,
	Console = 0x1D,
	Screenshot = 0x1E,
};

extern OSInputGlobals** g_inputGlobals;

//TESAnimGroup::AnimGroupInfo* GetGroupInfo(UInt8 groupId);
UInt32 GetSequenceType(UInt8 groupId);

NiAVObject* __fastcall GetNifBlock(TESObjectREFR* thisObj, UInt32 pcNode, const char* blockName);

#define GetExtraType(xDataList, Type) (Extra ## Type*)xDataList.GetByType(kExtraData_ ## Type)

TESForm* __stdcall LookupFormByRefID(UInt32 refID);
void FormatScriptText(std::string& str);

UInt16 GetActorRealAnimGroup(Actor* actor, UInt8 groupID);

// 64
struct ActorHitData
{
	enum HitFlags
	{
		kFlag_TargetIsBlocking			= 1 << 0,
		kFlag_TargetWeaponOut			= 1 << 1,
		kFlag_IsCritical				= 1 << 2,
		kFlag_OnDeathCritEffect			= 1 << 3,
		kFlag_IsFatal					= 1 << 4,
		kFlag_DismemberLimb				= 1 << 5,
		kFlag_ExplodeLimb				= 1 << 6,
		kFlag_CrippleLimb				= 1 << 7,
		kFlag_BreakWeaponNonEmbedded	= 1 << 8,
		kFlag_BreakWeaponEmbedded		= 1 << 9,
		kFlag_IsSneakAttack				= 1 << 10,
		kFlag_ArmorPenetrated			= 0x80000000	// JIP only
	};

	Actor*				source;			// 00
	Actor*				target;			// 04
	union								// 08
	{
		Projectile*		projectile;
		Explosion*		explosion;
	};
	UInt32				weaponAV;		// 0C
	SInt32				hitLocation;	// 10
	float				healthDmg;		// 14
	float				wpnBaseDmg;		// 18	Skill and weapon condition modifiers included
	float				fatigueDmg;		// 1C
	float				limbDmg;		// 20
	float				blockDTMod;		// 24
	float				armorDmg;		// 28
	float				weaponDmg;		// 2C
	TESObjectWEAP*		weapon;			// 30
	Float32				healthPerc;		// 34
	NiPoint3			impactPos;		// 38
	NiPoint3			impactAngle;	// 44
	UInt32				unk50;			// 50
	void*				ptr54;			// 54
	UInt32				flags;			// 58
	float				dmgMult;		// 5C
	SInt32				unk60;			// 60	Unused; rigged by CopyHitDataHook to store hitLocation

	TESAmmo*			GetAmmo() const;
	TESObjectWEAP*		GetWeapon() const;
	Script*				GetAmmoScript() const;
};


enum APCostActions
{
	kActionPointsAttackUnarmed = 0x0,
	kActionPointsAttackOneHandMelee = 0x1,
	kActionPointsAttackTwoHandMelee = 0x2,
	kActionPointsAttackPistol = 0x3,
	kActionPointsAttackRifle = 0x4,
	kActionPointsAttackHandle = 0x5,
	kActionPointsAttackLauncher = 0x6,
	kActionPointsAttackGrenade = 0x7,
	kActionPointsAttackMine = 0x8,
	kActionPointsReload = 0x9,
	kActionPointsCrouch = 0xA,
	kActionPointsStand = 0xB,
	kActionPointsSwitchWeapon = 0xC,
	kActionPointsToggleWeaponDrawn = 0xD,
	kActionPointsHeal = 0xE,
	kActionPointsVATSUnarmedAttack1 = 0x11,
	kActionPointsOneHandThrown = 0x13,
	kActionPointsAttackThrown = 0x14,
	kActionPointsVATSUnarmedAttackGround = 0x15,
};

enum ActorValueCode
{
	kAVCode_Aggression = 0x0,
	kAVCode_Confidence = 0x1,
	kAVCode_Energy = 0x2,
	kAVCode_Responsibility = 0x3,
	kAVCode_Mood = 0x4,
	kAVCode_Strength = 0x5,
	kAVCode_Perception = 0x6,
	kAVCode_Endurance = 0x7,
	kAVCode_Charisma = 0x8,
	kAVCode_Intelligence = 0x9,
	kAVCode_Agility = 0xA,
	kAVCode_Luck = 0xB,
	kAVCode_ActionPoints = 0xC,
	kAVCode_CarryWeight = 0xD,
	kAVCode_CritChance = 0xE,
	kAVCode_HealRate = 0xF,
	kAVCode_Health = 0x10,
	kAVCode_MeleeDamage = 0x11,
	kAVCode_DamageResistance = 0x12,
	kAVCode_PoisonResist = 0x13,
	kAVCode_RadResist = 0x14,
	kAVCode_SpeedMult = 0x15,
	kAVCode_Fatigue = 0x16,
	kAVCode_Karma = 0x17,
	kAVCode_XP = 0x18,
	kAVCode_PerceptionCondition = 0x19,
	kAVCode_EnduranceCondition = 0x1A,
	kAVCode_LeftAttackCondition = 0x1B,
	kAVCode_RightAttackCondition = 0x1C,
	kAVCode_LeftMobilityCondition = 0x1D,
	kAVCode_RightMobilityCondition = 0x1E,
	kAVCode_BrainCondition = 0x1F,
	kAVCode_Barter = 0x20,
	kAVCode_BigGuns = 0x21,
	kAVCode_EnergyWeapons = 0x22,
	kAVCode_Explosives = 0x23,
	kAVCode_Lockpick = 0x24,
	kAVCode_Medicine = 0x25,
	kAVCode_MeleeWeapons = 0x26,
	kAVCode_Repair = 0x27,
	kAVCode_Science = 0x28,
	kAVCode_Guns = 0x29,
	kAVCode_Sneak = 0x2A,
	kAVCode_Speech = 0x2B,
	kAVCode_Survival = 0x2C,
	kAVCode_Unarmed = 0x2D,
	kAVCode_InventoryWeight = 0x2E,
	kAVCode_Paralysis = 0x2F,
	kAVCode_Invisibility = 0x30,
	kAVCode_Chameleon = 0x31,
	kAVCode_NightEye = 0x32,
	kAVCode_Turbo = 0x33,
	kAVCode_FireResist = 0x34,
	kAVCode_WaterBreathing = 0x35,
	kAVCode_RadiationRads = 0x36,
	kAVCode_BloodyMess = 0x37,
	kAVCode_UnarmedDamage = 0x38,
	kAVCode_Assistance = 0x39,
	kAVCode_ElectricResist = 0x3A,
	kAVCode_FrostResist = 0x3B,
	kAVCode_EnergyResist = 0x3C,
	kAVCode_EmpResist = 0x3D,
	kAVCode_Variable01 = 0x3E,
	kAVCode_Variable02 = 0x3F,
	kAVCode_Variable03 = 0x40,
	kAVCode_Variable04 = 0x41,
	kAVCode_Variable05 = 0x42,
	kAVCode_Variable06 = 0x43,
	kAVCode_Variable07 = 0x44,
	kAVCode_Variable08 = 0x45,
	kAVCode_Variable09 = 0x46,
	kAVCode_Variable10 = 0x47,
	kAVCode_IgnoreCrippledLimbs = 0x48,
	kAVCode_Dehydration = 0x49,
	kAVCode_Hunger = 0x4A,
	kAVCode_SleepDeprivation = 0x4B,
	kAVCode_DamageThreshold = 0x4C,
	kAVCode_Max = 0x4D,
};

struct __declspec(align(4)) VATSQueuedAction
{
	APCostActions actionType;
	UInt8 isSuccess;
	UInt8 byte05;
	UInt8 isMysteriousStrangerVisit;
	UInt8 byte07;
	UInt8 remainingShotsToFire_Burst;
	UInt8 count09;
	UInt8 gap0A[2];
	TESObjectREFR* unkref;
	ActorValueCode bodyPart;
	ActorHitData* hitData;
	float unk18;
	float unk1C;
	float apCost;
	UInt8 isMissFortuneVisit;
	UInt8 gap25[3];
};

enum AnimMoveTypes
{
	kAnimMoveType_Walking	= 0x0,
	kAnimMoveType_Sneaking	= 0x1,
	kAnimMoveType_Swimming	= 0x2,
	kAnimMoveType_Flying	= 0x3,
};