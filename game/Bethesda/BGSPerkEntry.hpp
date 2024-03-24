#pragma once

class BGSPerkEntry {
public:
	BGSPerkEntry();

	virtual void	Fn_00();
	virtual void	Fn_01();
	virtual void	Fn_02();
	virtual			~BGSPerkEntry();
	virtual UInt32	GetType();
	virtual void	Fn_05();
	virtual void	Fn_06();
	virtual void	Fn_07();
	virtual void	Fn_08();
	virtual void	Fn_09();
	virtual void	Fn_0A();
	virtual void	Fn_0B();
	virtual void	Fn_0C();
	virtual void	Fn_0D();

	UInt8				rank;				// 04 +1 for value shown in GECK
	UInt8				priority;			// 05
	UInt16				type;				// 06 (Quest: 0xC24, Ability: 0xB27, Entry Point: 0xD16)
};