#pragma once

#include "NiMemObject.hpp"

#define NiTDefaultAllocator DFALL

template <class T>
class NiTDefaultAllocator {
protected:
	class AllocNode : public NiMemObject {
	public:
		AllocNode() : m_pkNext(NULL), m_pkData(NULL) {}
		~AllocNode() {}

		AllocNode* m_pkNext;
		void* m_pkData;
		T m_element;
	};

public:
	void* Allocate() {
		AllocNode* pkNode = new AllocNode;
		pkNode->m_element = 0;
		return (void*)pkNode;
	}
	void Deallocate(void* p) {
		delete (AllocNode*)p;
	}
};