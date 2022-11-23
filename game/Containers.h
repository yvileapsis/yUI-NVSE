#pragma once
#include <Utilities.h>

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
static_assert(sizeof(String) == 0x8);

enum {
	eListCount = -3,
	eListEnd = -2,
	eListInvalid = -1,
};


template <typename Item> struct TListNode
{
	Item* data;
	TListNode* next;

	TListNode() : data(nullptr), next(nullptr) {}
	TListNode(Item* _data) : data(_data), next(nullptr) {}

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
		FormHeapFree(pNext);
		return next;
	}

	TListNode<Item>* Append(Item* _data)
	{
		const auto newNode = static_cast<TListNode*>(FormHeapAlloc(sizeof(TListNode)));
		newNode->data = _data;
		newNode->next = next;
		next = newNode;
		return newNode;
	}

	TListNode<Item>* Insert(Item* _data)
	{
		const auto newNode = static_cast<TListNode*>(FormHeapAlloc(sizeof(TListNode)));
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
		Node* node = Head();
		if (!node) return nullptr;
		if (outIdx)
		{
			SInt32 index = 0;
			while (node->next)
			{
				node = node->next;
				index++;
			}
			*outIdx = index;
		}
		else while (node->next) node = node->next;
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
		} while ((node = node->next));

		return nullptr;
	}

	void Init(Item* item = nullptr)
	{
		first.data = item;
		first.next = NULL;
	}

	Node* Head() const { return const_cast<Node*>(&first); }
	Node* Tail() const { return GetLastNode(); }

	bool Empty() const { return !first.data; }

	class Iterator
	{
		Node* m_curr;

	public:
		Iterator	operator++()
		{
			m_curr = m_curr->next;
			return *this;
		}
		Item* operator->() const { return m_curr->data; }
		Item*& operator*() const { return m_curr->data; }
		Iterator& operator=(const Iterator& rhs)
		{
			m_curr = rhs.m_curr;
			return *this;
		}
		bool		operator!=(const Iterator& rhs) { return m_curr && m_curr->data; }

		Iterator(Node* node = nullptr) : m_curr(node) {}
		Iterator(TList& _list) : m_curr(&_list.first) {}
		Iterator(TList* _list) : m_curr(&_list->first) {}
		Iterator(TList& _list, Item* _item) : m_curr(&_list.first) { Find(_item); }
		Iterator(TList* _list, Item* _item) : m_curr(&_list->first) { Find(_item); }
	};

	Iterator begin() const { return Iterator(Head()); }
	Iterator end() const { return Iterator(); }

	UInt32 Count() const
	{
		if (!first.data) return 0;
		UInt32 count = 1;
		Node* node = Head();
		while ((node = node->next)) count++;
		return count;
	}

	bool Contains(Item* item) const
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

	template <class Op>	UInt32 RemoveIf(Op&& op)
	{
		return FreeNodes(Head(), op);
	}

	SInt32 GetIndexOf(Item* item)
	{
		SInt32 idx = 0;
		Node* curr = Head();
		do {
			if (curr->data == item) return idx;
			idx++;
		} while ((curr = curr->next));
		return -1;
	}

	template <class Op>	SInt32 GetIndexOf(Op&& op)
	{
		SInt32 idx = 0;
		Node* curr = Head();
		do {
			if (curr->data && op.Accept(curr->data)) return idx;
			idx++;
		} while ((curr = curr->next));
		return -1;
	}

	template <typename F> void ForEach(const F* function)
	{
		auto* node = Head();
		if (!node) return;
		do function(node->data);
		while ((node = node->next));
	}

	template <typename F> void ForEach(const F& f)
	{
		for (const auto iter : this)
			f(iter);
	}
};
static_assert(sizeof(TList<void*>) == 0x8);

template <typename Item> struct DListNode
{
	DListNode* next;
	DListNode* prev;
	Item* data;

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

};

template <class Item> class DList
{
public:
	typedef DListNode<Item> Node;

private:
	Node* first = nullptr;
	Node* last = nullptr;
	UInt32		count = 0;

public:

	class Iterator
	{
		Node* m_curr;

	public:
		Iterator	operator++()
		{
			m_curr = m_curr->next;
			return *this;
		}
		Item* operator->() const { return m_curr->data; }
		Item*& operator*() const { return m_curr->data; }
		Iterator& operator=(const Iterator& rhs)
		{
			m_curr = rhs.m_curr;
			return *this;
		}
		bool		operator!=(const Iterator& rhs) { return m_curr && m_curr->data; }

		Iterator(Node* node = nullptr) : m_curr(node) {}
		Iterator(DList& _list) : m_curr(&_list.first) {}
		Iterator(DList* _list) : m_curr(&_list->first) {}
		Iterator(DList& _list, Item* _item) : m_curr(&_list.first) { Find(_item); }
		Iterator(DList* _list, Item* _item) : m_curr(&_list->first) { Find(_item); }
	};

	bool Empty() const { return !first; }
	Node* Head() { return first; }
	Node* Tail() { return last; }
	UInt32 Count() const { return count; }

	Iterator begin() const { return Iterator(first); }
	Iterator end() const { return Iterator(); }

	void ExchangeNodeData(Node* node1, Node* node2)
	{
		const auto tmpdata = node1->data;
		node1->data = node2->data;
		node2->data = tmpdata;
	}

	void Append(Item* item) { ThisCall(0x4ED8C0, this, &item); }

	template <typename F> void ForEach(const F& f)
	{
		for (const auto iter : this)
			f(iter);
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
			if (first) first->prev = nullptr;
			else last = nullptr;

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
				if (node->prev) node->prev->next = node->next;
				if (node->next)	node->next->prev = node->prev;
				else last = node->prev;
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

	Node* GetNthChild(UInt32 n)
	{
		auto node = first;
		while (n--) node = node->next;
		return node;
	}
};
static_assert(sizeof(DList<void*>) == 0xC);

enum KeyboardMenuInputCode;

// 010
template <class Item>
class BSSimpleList
{
public:
	BSSimpleList<Item>();
	~BSSimpleList<Item>();

	virtual bool	SetSelectedTile(Tile* tile) { return false; };
	virtual Tile* GetSelectedTile(void) { return nullptr; };
	virtual Tile* HandleKeyboardInput(KeyboardMenuInputCode code) { return nullptr; };
	virtual bool	IsMenuEqual(Menu* that) { return false; };
	virtual void	ScrollToHighlight(void) {};
	virtual Tile* GetTileByIndex(int index, char isNotTileListIndex) { return nullptr; };
	virtual void	Destructor(bool doFree) {};
	virtual void	FreeAllTiles(void) {};
	virtual void	Sort(signed int(__cdecl*)(Item*, Item*)) {};

	TList<Item>		list;	// 004
};	// 00C
static_assert(sizeof(BSSimpleList<void*>) == 0xC);

template <typename Item> struct BSSimpleArray
{
	virtual void	Destroy(bool doFree);
	virtual Item* Allocate(UInt32 size);

	Item* data;			// 04
	UInt32			size;			// 08
	UInt32			alloc;			// 0C

	Item operator[](UInt32 idx)
	{
		return idx < size ? data[idx] : NULL;
	}

	class Iterator
	{
	protected:
		friend BSSimpleArray;

		Item* pData;
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
		bool operator!=(const Iterator& rhs) { return pData != rhs.pData; }

		Iterator() : pData(nullptr), count(0) {}
		Iterator(const BSSimpleArray& source) : pData(source.data), count(source.size) {}
		Iterator(const BSSimpleArray* source) : pData(source->data), count(source->size) {}

	};

	Iterator begin() const { return Iterator(); }
	Iterator end() const { return Iterator(this); }

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
			if (nodeCount) node->Delete();
			else node->DeleteHead(lastNode);
			numFreed++;
			bRemovedNext = true;
		}
		else
		{
			if (bRemovedNext) node->SetNext(lastNode);
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
		bool Accept(const Info* info) { return info == m_toMatch; }
	};

	class AcceptStriCmp {
		const char* m_toMatch;
	public:
		AcceptStriCmp(const char* info) : m_toMatch(info) { }
		bool Accept(const char* info) { return m_toMatch && info ? !_stricmp(info, m_toMatch) ? true : false : false; }
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
			if (bContinue) pCur = pCur->Next();
		}
	}

	template <class Op>
	const Node* Find(Op&& op, const Node* prev = NULL) const
	{
		const Node* pCur;
		if (!prev) pCur = m_pHead;
		else pCur = prev->next;
		bool bFound = false;
		while (pCur && !bFound)
		{
			if (pCur->Info())
			{
				bFound = op.Accept(pCur->Info());
				if (!bFound) pCur = pCur->Next();
			}
			else pCur = pCur->Next();
		}
		return pCur;
	}

	Node* FindInfo(const Info* toMatch) { return Find(AcceptEqual(toMatch)); }

	template <class Op>
	UInt32 CountIf(Op&& op) const
	{
		UInt32 count = 0;
		const Node* pCur = m_pHead;
		while (pCur)
		{
			if (pCur->Info() && op.Accept(pCur->Info())) count++;
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
		if (lastNode == m_pHead && !m_pHead->Info()) lastNode->DeleteHead(newNode);
		else lastNode->SetNext(newNode);
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
	Float32	x;
	Float32	y;

	CoordXY() : x(0), y(0) {}

	CoordXY(float _x, float _y) : x(_x), y(_y) {}

	inline CoordXY& operator =(const CoordXY& rhs)
	{
		*(Float64*)this = *(Float64*)&rhs;
		return *this;
	}
};
