#pragma once

#include "NiTMap.hpp"
#include "NiTPointerMap.hpp"

template <class T_Parent, class T_Data>
class NiTStringTemplateMap : public T_Parent {
public:
	NiTStringTemplateMap();
	~NiTStringTemplateMap();

	virtual void* Destructor(bool doFree);								// 000
	virtual UInt32 KeyToHashIndex(const char* pcKey);				// 004
	virtual bool IsKeysEqual(const char* pcKey1, const char* pcKey2);	// 008
	virtual void SetValue(NiTMapItem<const char*, T_Data>* pkItem, const char* pcKey, T_Data val);	// 00C
	virtual void ClearValue(NiTMapItem<const char*, T_Data>* pkItem);		// 010
	virtual	void NewItems(void);												// 005
	virtual	void DeleteItem(NiTMapItem<const char*, T_Data>* pkItem);							// 006

	bool m_bCopy;
};

template <class T_Data>
class NiTStringMap : public NiTStringTemplateMap<NiTMap<const char*, T_Data>, T_Data> {
};
STATIC_ASSERT(sizeof(NiTStringMap<UInt32>) == 0x14);

template <class T_Data>
class NiTStringPointerMap : public NiTStringTemplateMap<NiTPointerMap<const char*, T_Data>, T_Data> {
};
STATIC_ASSERT(sizeof(NiTStringPointerMap<UInt32>) == 0x14);