#pragma once

template <typename T_Data>
struct DListNode {
	DListNode* m_pkNext;
	DListNode* m_pkPrev;
	T_Data* m_pkHead;

	DListNode* Advance(UInt32 times)
	{
		DListNode* result = this;
		while (result && times)
		{
			times--;
			result = result->m_pkNext;
		}
		return result;
	}

	DListNode* Regress(UInt32 times)
	{
		DListNode* result = this;
		while (result && times)
		{
			times--;
			result = result->m_pkPrev;
		}
		return result;
	}

	DListNode* Next() { return m_pkNext ? m_pkNext : 0; }
};

template <class Item>
class DList {
public:
	typedef DListNode<Item> Node;

private:
	Node* first;
	Node* last;
	UInt32 count;

public:
	bool Empty() const { return !first; }
	Node* Head() { return first; }
	Node* Tail() { return last; }
	UInt32 Size() const { return count; }
};