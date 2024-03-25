#pragma once

// 0x4
class CachedValuesOwner {
public:
	CachedValuesOwner();
	~CachedValuesOwner();

	virtual float	Fn_00();
	virtual float	Fn_01();
	virtual float	Fn_02();
	virtual float	Fn_03();
	virtual float	Fn_04();
	virtual float	Fn_05();
	virtual float	Fn_06();
	virtual float	Fn_07();
	virtual float	Fn_08();
	virtual float	Fn_09();
	virtual float	Fn_0A();
	virtual UInt32	Fn_0B();
	virtual UInt32	Fn_0C();
	virtual float	Fn_0D();
	virtual float	Fn_0E();
	virtual bool	Fn_0F();
};
static_assert(sizeof(CachedValuesOwner) == 0x4);