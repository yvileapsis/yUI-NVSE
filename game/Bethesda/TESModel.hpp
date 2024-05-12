#pragma once

#include "BaseFormComponent.hpp"
#include "BSString.hpp"

class BSFileEntry;

class TESModel : public BaseFormComponent {
public:
	TESModel();
	virtual				~TESModel();

	virtual const char*	GetModel();
	virtual void		SetModel(const char* apcPath);
	virtual bool		Unk_7();

	enum FacegenFlags {
		HEAD			= 1 << 0,
		TORSO			= 1 << 1,
		RIGHT_HAND		= 1 << 2,
		LEFT_HAND		= 1 << 3,
	};

	struct TextureData {
		UInt8			ucCount;
		BSFileEntry**	pFileEntries;
	};

	BSStringT	strModel;
	TextureData kTextures;
	Bitfield8	ucFaceGenFlags;

	void SetPath(const char* newPath) { strModel.Set(newPath); }
};

ASSERT_SIZE(TESModel, 0x18);