#pragma once
#include "BaseFormComponent.hpp"
#include "BSString.hpp"

class BSFileEntry;

// 0x18
class TESModel : public BaseFormComponent
{
public:
	TESModel();

	virtual			~TESModel();
	virtual char*	GetModel();
	virtual void	SetModel(const char* apcPath);	// 06
	virtual bool	Unk_07();

	enum EnumFacegenFlags : UInt8
	{
		kFacegenFlag_Head		= 1 << 0,
		kFacegenFlag_Torso		= 1 << 1,
		kFacegenFlag_RightHand	= 1 << 2,
		kFacegenFlag_LeftHand	= 1 << 3,
	};

	struct TextureData
	{
		UInt8			ucCount;
		BSFileEntry**	pFileEntries;
	};

	BSStringT<char>		kModel;
	TextureData			kTextures;
	EnumFacegenFlags	ucFacegenFlags;
	UInt8				pad15[3];

	void SetPath(const char* newPath) { kModel.Set(newPath); }
};
static_assert(sizeof(TESModel) == 0x18);