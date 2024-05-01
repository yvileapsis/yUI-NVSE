#pragma once

class BGSEntryPointFunctionData {
public:
	BGSEntryPointFunctionData();
	virtual			~BGSEntryPointFunctionData();
	virtual	void	Unk_01();
	virtual	void	Unk_02();
	virtual	void	Unk_03();
	virtual	void	Unk_04();
	virtual	void	Unk_05();
	virtual	void	Unk_06();
};
static_assert(sizeof(BGSEntryPointFunctionData) == 0x4);