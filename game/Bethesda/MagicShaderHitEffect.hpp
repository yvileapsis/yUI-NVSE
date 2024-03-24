#pragma once

#include "MagicHitEffect.hpp"
#include "ParticleShaderProperty.hpp"

class TextureEffectData;

class MagicShaderHitEffect : public MagicHitEffect {
public:
	MagicShaderHitEffect();
	~MagicShaderHitEffect();

	bool unk28;
	UInt32 unk2C;
	TESEffectShader* effectShader;
	float time34;					// 34	Init'd to float
	BSSimpleArray<NiPointer<ParticleShaderProperty>>	unk38;
	NiNode* shaderNode;
	bool isVisible;
	BSSimpleArray<NiPointer<NiAVObject>> objects;
	float flt60;
	float flt64;
	TextureEffectData* effectProperty;
};

ASSERT_SIZE(MagicShaderHitEffect, 0x6C);