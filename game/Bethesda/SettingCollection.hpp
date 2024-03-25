#pragma once
#include "NiTArray.hpp"

template<class T> 
class SettingCollection {
public:
	SettingCollection();
	virtual			~SettingCollection();
	virtual void	Add(T* apSetting);
	virtual void	Remove(T* apSetting);
	virtual UInt32	GetViewerStrings(NiViewerStringsArray* apStrings);
	virtual bool	WriteSetting(T* apSetting);
	virtual bool	ReadSetting(T* apSetting);
	virtual bool	Open(bool abWrite);
	virtual bool	Close();
	virtual bool	WriteSettings();
	virtual bool	ReadSettings();

	char pSettingFile[MAX_PATH];
	void* pHandle;
};
static_assert(sizeof(SettingCollection<void*>) == 0x10C);