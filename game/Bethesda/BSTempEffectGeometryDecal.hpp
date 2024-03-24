#pragma once

#include "BSTempEffect.hpp"
#include "NiPoint3.hpp"
#include "NiNode.hpp"
#include "NiTriShape.hpp"
#include "BGSTextureSet.hpp"

class Actor;

NiSmartPointer(BSTempEffectGeometryDecal)

class DECAL_CREATION_DATA {
public:
	DECAL_CREATION_DATA();
	~DECAL_CREATION_DATA() {};

	NiPoint3			kOrigin;
	NiPoint3			kDirection;
	NiPoint3			kSurfaceNormal;
	Actor*				pRefr;
	NiNode*				pNode;
	UInt32				unk2C;
	BGSTextureSet*		pTexSet;
	SInt32				iIndex;
	float				fDecalWidth;
	float				fDecalHeight;
	float				fDecalDepth;
	float				rng44;
	TESObjectCELL*		pParentCell;
	float				fParallaxOcclusionScale;
	NiNode*				pSkinnedDecalNode;
	float				fSpecular;
	float				fDecalEpsilon;
	float				fPlacementRadius;
	NiColor				kVertexColor;
	unsigned int		iBodyParts;
	char				cSubTex;
	bool				bPermanent;
	bool				byte72;
	bool				bParallax;
	bool				bAlphaTesting;
	bool				bAlphaBlending;
	bool				ucParallaxOcclusionMaxPasses;
	bool				bIgnoreDistToPlayer;
	bool				byte78;
	bool				bIsImpactMaterialGlass;
	bool				gap7A[2];
};


class BSTempEffectGeometryDecal : public BSTempEffect {
public:
	UInt32 unk18;
	NiTriShapePtr spShape;
	NiNode* pParentNode;
	DWORD dword24;
	BYTE bDestroyed;
	NiNodePtr spFadeNode;
	NiRefObject* spObject30;
	NiRefObject* spObject34;
	NiPoint3 Origin;
	NiPoint3 Direction;
	float fScale;
	float float54;
	BGSTextureSet* pTextureSet;
	DWORD dword5C;
	UInt32 iBodyParts;
};

ASSERT_SIZE(BSTempEffectGeometryDecal, 0x64);