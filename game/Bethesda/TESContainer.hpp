#pragma once
#include "BaseFormComponent.hpp"
#include "BSSimpleList.hpp"

// 0x10
struct ContainerItemExtra {
	UInt32	pkOwner;
	UInt32	uiGlobalOrRequiredRank;
	Float64	dItemCondition;
};
static_assert(sizeof(ContainerItemExtra) == 0x10);

// 0xC
class TESContainer : public BaseFormComponent {
public:
	TESContainer();
	~TESContainer();

	struct FormCount
	{
		SInt32				iCount;			//	00
		TESForm*			pkForm;			//	04
		ContainerItemExtra*	pItemExtra;		//	08
	};

	BSSimpleList<FormCount*> kFormCountList;	// 004

	SInt32 GetCountForForm(TESForm* form);
};
static_assert(sizeof(TESContainer) == 0xC);