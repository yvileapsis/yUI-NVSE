#pragma once

#include "NiTArray.hpp"

template<class T> 
class SettingCollection {
public:
	virtual void	Destroy(bool);
	virtual void	Add(T* apSetting);
	virtual void	Remove(T* apSetting);
	virtual UInt32	GetViewerStrings(NiViewerStringsArray* apStrings);
	virtual bool	WriteSetting(T* apSetting);
	virtual bool	ReadSetting(T* apSetting);
	virtual bool	Open(bool abWrite);
	virtual bool	Close();
	virtual bool	WriteSettings();
	virtual bool	ReadSettings();

	char pSettingFile[260];
	void* pHandle;
};

ASSERT_SIZE(SettingCollection<UInt32>, 0x10C);