#pragma once
#include "BaseFormComponent.hpp"
#include "BSSimpleList.hpp"

// 0x14
class TESModelList : public BaseFormComponent {
public:
	TESModelList();
	~TESModelList();

	BSSimpleList<char>	modelList;	// 04
	UInt32				count;		// 0C
	UInt32				unk10;		// 10

	SInt32	GetIndex(char* path);
	bool	RemoveEntry(char* nifToRemove);
	bool	AddEntry(char* nifToAdd);
	bool	ModelListAction(char* path, SInt8 action);
};
static_assert(sizeof(TESModelList) == 0x14);