#pragma once

#include "BaseFormComponent.hpp"

class TESValueForm : public BaseFormComponent {
public:
	enum
	{
		kModified_GoldValue = 0x00000008,
		// UInt32	value
	};

	TESValueForm();
	~TESValueForm();

	virtual UInt32	GetSaveSize(UInt32 changedFlags);
	virtual void	Save(UInt32 changedFlags);
	virtual void	Load(UInt32 changedFlags);

	//	DEFINE_MEMBER_FN_LONG(TESValueForm, SetValue, void, _TESValueForm_SetValue, UInt32 newVal);

	SInt32	iValue;
	// 008
};

ASSERT_SIZE(TESValueForm, 0x8);