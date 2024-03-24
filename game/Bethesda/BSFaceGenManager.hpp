#pragma once

#include "NiRefObject.hpp"
#include "BSFaceGenModelMap.hpp"

class NiSourceTexture;
class NiAlphaProperty;
class NiNode;

struct LargeVector {
	DWORD unk[4];
	UInt32 useOffset;
	UInt32 maxOffset;
};

class BSFaceGenManager {
public:
	struct FaceGenData {
		LargeVector	 vector;
		UInt32 count;
		UInt32 size;
	};

	struct FaceGenDatas {
		FaceGenData faceGens[2][2];
	};

	struct Struct0 {
		LargeVector unkStructs[4];
	};

	struct Struct1 {
		struct Struct00 {
			FaceGenData facegendata00[2];
			FaceGenData facegendata40;
			FaceGenData facegendata60[2];
		};

		bool			byte0;
		Struct00		data04[5];
		char			char324[800];
		char			char644[1800];
		FaceGenData		datasD4C[20];
	};


	NiNode*			pNode0;
	UInt32			unk004;
	FaceGenDatas	kDefaultFaceGenData;
	LargeVector		kVec088;
	LargeVector		kVec0A0;
	Struct0			kStruct0;
	Struct1			kStruct1;
	UInt32 unk10E4;
	UInt32 unk10E8;
	UInt32 unk10EC;
	UInt32 unk10F0;
	UInt32 unk10F4;
	UInt32 unk10F8;
	UInt32 unk10FC;
	UInt32 unk1100;
	UInt32 unk1104;
	UInt32 unk1108;
	UInt32 unk110C;
	UInt32 unk1110;
	UInt32 unk1114;
	UInt32 unk1118;
	UInt32 unk111C;
	UInt32 unk1120;
	UInt32 unk1124;
	UInt32 unk1128;
	UInt32 unk112C;
	UInt32 unk1130;
	UInt32 unk1134;
	UInt32 unk1138;
	UInt32 unk113C;
	UInt32 unk1140;
	UInt32 unk1144;
	UInt32 unk1148;
	UInt32 unk114C;
	UInt32 unk1150;
	UInt32 unk1154;
	UInt32 unk1158;
	UInt32 unk115C;
	UInt32 unk1160;
	UInt32 unk1164;
	UInt32 unk1168;
	UInt32 unk116C;
	UInt32 unk1170;
	UInt32 unk1174;
	UInt32 unk1178;
	UInt32 unk117C;
	UInt32 unk1180;
	UInt32 unk1184;
	UInt32 unk1188;
	UInt32 unk118C;
	UInt32 unk1190;
	BSFaceGenModelMap*			pModelMap;
	NiPointer<NiAlphaProperty>	spAlphaProp;
	NiPointer<NiSourceTexture>	spBaseTexture;
	NiPointer<NiSourceTexture>	spDetailTexture;

	static BSFaceGenManager* GetSingleton();
};

ASSERT_SIZE(BSFaceGenManager, 0x11A4u);
ASSERT_SIZE(BSFaceGenManager::FaceGenData, 0x20);