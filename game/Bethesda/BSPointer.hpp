#pragma once

template <class T_Data>
class BSPointer {
public:
	BSPointer() : pObject(nullptr), iBits(0) {}
	BSPointer(T_Data* pObject) : pObject(pObject), iBits(0) {}
	BSPointer(const BSPointer& ptr) : pObject(ptr.pObject), iBits(ptr.iBits) {}
	~BSPointer() {}

	T_Data* pObject;
	SInt32 iBits;

	bool operator==(const BSPointer& ptr) const { return pObject == ptr.pObject; }
	bool operator!=(const BSPointer& ptr) const { return pObject != ptr.pObject; }
	bool operator==(const T_Data* ptr) const { return pObject == ptr; }
	bool operator!=(const T_Data* ptr) const { return pObject != ptr; }
	operator bool() const { return pObject != nullptr; }
};