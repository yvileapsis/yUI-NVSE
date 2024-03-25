#pragma once
#include "NiObject.hpp"

class TESObjectCELL;
class NiNode;

NiSmartPointer(BSTempEffect);

// 0x18
class BSTempEffect : public NiObject 
{
public:
	enum Type : UInt32 {
		GEO_DECAL = 1,
		PARTICLE = 2,
		SIMPLE_DECAL = 3,
		MAGIC_HIT = 4,
		MAGIC_MODEL_HIT = 5,
		MAGIC_SHADER_HIT = 6,
	};


	BSTempEffect();
	virtual ~BSTempEffect();

	virtual void			Initialize();			// 35
	virtual BSTempEffect*	GetSelf();				// 36 | ???? Used by particles
	virtual bool			Update(float afTime);	// 37 | Returns true if effect is finished
	virtual NiNode*			Get3D();				// 38 | Returns geometry
	virtual Type			GetType();				// 39 |	Returns 3 for base
	virtual bool			IsLoaded();				// 40 | ???? Used by MagicModelHitEffect and particles
	virtual UInt32			GetSaveSize();			// 41
	virtual void			SaveGame();				// 42
	virtual void			SaveGame2();			// 43 | Saves cell refid, skips bInitialized
	virtual void 			LoadGame();				// 44
	virtual void			LoadGame2();			// 45 | Loads cell refid?
	virtual void			SetTarget();			// 46 | Used by MagicHitEffect
	virtual void			Unk_47();				// 47 | ????
	virtual bool			IsFirstPerson();		// 48 | Used by shell casings

	Float32			fLifetime;
	TESObjectCELL*	pkCell;
	Float32			fAge;
	bool			bInitialized;
};
static_assert(sizeof(BSTempEffect) == 0x18);