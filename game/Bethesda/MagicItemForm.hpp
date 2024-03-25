#pragma once
#include "TESForm.hpp"
#include "MagicItem.hpp"

// 034
class MagicItemForm : public TESForm
{
public:
	MagicItemForm();

	~MagicItemForm() override;
	virtual void	ByteSwap(); // pure virtual

	MagicItem kMagicItem;
};
static_assert(sizeof(MagicItemForm) == 0x34);