#pragma once
#include "NiTypes.h"
#include "Containers.h"

// 0C
class Setting
{
public:
	Setting() {};
	virtual ~Setting() {};
	virtual void Unk_01();

	enum EType	{
		kSetting_Bool = 0,
		kSetting_Char,
		kSetting_h,
		kSetting_Integer,
		kSetting_Unsigned,
		kSetting_Float,
		kSetting_String,
		kSetting_r,
		kSetting_a,
		kSetting_Other
	};
	union Info {
		UInt32		uint;
		int			i;
		float		f;
		char*		str;
	};

	// void		** vtbl				// 00
	Info		data;				// 04
	char		* name;				// 08

	UInt32 GetType();
	std::string GetAsString();
	Float32 GetAsFloat();
	bool Set(double newVal);
	bool Set(const char* str);
};
static_assert(sizeof(Setting) == 0xC);

template<class T> class SettingCollection
{
	SettingCollection() {};
	virtual ~SettingCollection() {};
	virtual void	Unk_01();
	virtual void	Unk_02();
	virtual void	Unk_03();
	virtual void	Unk_04();
	virtual void	Unk_05();
	virtual void	Unk_06();
	virtual void	Unk_07();
	virtual void	Unk_08();
	virtual void	Unk_09();

	char				iniPath[0x100];		// 004
	UInt32				unk104;				// 104
	UInt32				unk108;				// 108
};

template<class T> class SettingCollectionList : public SettingCollection<T>
{
public:
	SettingCollectionList();
	~SettingCollectionList() override;
	TList<T> settingList;
};

class RegSettingCollection : public SettingCollectionList<Setting>
{
};

class INISettingCollection : public SettingCollectionList<Setting>
{
public:
	INISettingCollection();
	virtual ~INISettingCollection();

	static INISettingCollection* GetSingleton();
};
static_assert(sizeof(INISettingCollection) == 0x114);

class INIPrefSettingCollection : public INISettingCollection
{
public:
	static INIPrefSettingCollection* GetSingleton();
};

class BlendSettingCollection : public INISettingCollection
{
};

class RendererSettingCollection : public INISettingCollection
{
};

// 0014
template<class M, class T> class NiTStringTemplateMap : public M
{
public:
	UInt8	byt0010;	// 0010
	UInt8	fil0011[3];	// 0011
};

template<class T> class NiTStringMap : public NiTStringTemplateMap<NiTMap<char const*, T>, T>
{
};

template<class T> class BSTCaseInsensitiveStringMap : public NiTStringMap<T>
{
};

template<class T> class SettingCollectionMap : public SettingCollection<T>
{
public:
	SettingCollectionMap();
	virtual ~SettingCollectionMap();

	BSTCaseInsensitiveStringMap<T*>	settingMap;
};
static_assert(sizeof(SettingCollectionList<void>) == 0x114);

class GameSettingCollection : public SettingCollectionMap<Setting>
{
public:
	GameSettingCollection();
	virtual ~GameSettingCollection();

	static GameSettingCollection * GetSingleton();
};
static_assert(sizeof(GameSettingCollection) == 0x120);

Setting* GetINISetting(const std::string& settingName);
Setting* GetGameSetting(const std::string& settingName);