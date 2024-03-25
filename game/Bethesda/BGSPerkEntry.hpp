#pragma once

// 0x8
class BGSPerkEntry {
public:
	BGSPerkEntry();

	virtual void	CheckConditionFilters();
	virtual void	GetPriority();
	virtual void	GetData();
	virtual			~BGSPerkEntry();
	virtual UInt32	GetType();
	virtual void	Fn_06();
	virtual void	Fn_07();
	virtual void	Fn_08();
	virtual void	Fn_09();
	virtual void	Fn_0A();
	virtual void	Fn_0B();
	virtual void	Fn_0C();
	virtual void	Fn_0D();

	UInt8				ucRank;				// 04 +1 for value shown in GECK
	UInt8				ucPriority;			// 05
	UInt16				usType;				// 06 (Quest: 0xC24, Ability: 0xB27, Entry Point: 0xD16)
};
static_assert(sizeof(BGSPerkEntry) == 0x8);