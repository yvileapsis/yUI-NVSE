#pragma once

#include "BSShaderProperty.hpp"
#include "NiAVObject.hpp"
#include "NiColor.hpp"
#include "NiPoint4.hpp"

class ParticleShaderProperty : public BSShaderProperty {
public:
	void* unk060;
	UInt32 unk064;
	float unk068;
	bool byte06C;
	UInt8 gap06D[3];
	UInt32 unk070;
	float unk074;
	float unk078;
	float unk07C;
	float unk080;
	float unk084;
	float unk088;
	float unk08C;
	NiPoint3 Velocity;
	NiPoint3 Acceleration;
	float unk0A8;
	float unk0AC;
	float unk0B0;
	float unk0B4;
	float unk0B8;
	float unk0BC;
	float unk0C0;
	NiColorA Color1;
	NiColorA Color2;
	NiColorA Color3;
	NiPoint4 unkF4;
	float unk104;
	UInt32 srcBlend;
	UInt32 destBlend;
	UInt32 unk110;
	UInt32 unk114;
	NiTexture* pTexture_118;
	NiTObjectArray<NiPointer<NiAVObject>> unk11C;
	UInt32 unk12C;
	float fFade;
	float unk134;
	float unk138;
	float unk13C;
	float unk140;
	float unk144;
	float unk148;
};

ASSERT_SIZE(ParticleShaderProperty, 0x14C);