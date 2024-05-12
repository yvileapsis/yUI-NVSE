#pragma once

#include "TESChildCell.hpp"
#include "TESForm.hpp"
#include "BSSimpleArray.hpp"
#include "NiPoint3.hpp"
#include "NiColor.hpp"
#include "NiLines.hpp"
#include "NiTPointerMap.hpp"
#include "QueuedFile.hpp"
#include "TESGrassAreaParam.hpp"

class TESObjectCELL;
class NiNode;
class NiObject;
class TESLandTexture;

class TESObjectLAND : public TESForm, TESChildCell {
public:
	struct LoadedLandData
	{
		struct Geometry {
			NiPoint3 quad0Vertices[289];
		};

		struct Struct08 {
			NiPoint3 quad0Unk[289];
		};

		struct Colors {
			NiColorA vertColor[289];
		};

		struct Textures {
			TESLandTexture* pLandTexture[5];
		};

		NiNode**									ppMesh;
		Geometry**									geometry;
		Struct08**									spGeom;
		Colors**									pColors;
		void**										ptr10;
		NiLinesPtr									spBorder;
		float										minHeight;
		float										maxHeight;
		TESLandTexture*								pDefQuadTexture[4];
		Textures*									pQuadTextureArray[4];
		float**										pPercentArrays[4]; // 4 * 289 * 6 
		UInt32										unk50;
		NiTPointerMap<UInt32, TESGrassAreaParam**>	grassParams54[4];
		NiObject*									object94;
		SInt32										iCellX;
		SInt32										iCellY;
		float										fBaseHeight;
	};
	static_assert(sizeof(LoadedLandData) == 0xA4);

	UInt32				iFlags;
	TESObjectCELL*		pParentCell;
	QueuedFilePtr		spQueuedTextures;
	LoadedLandData*		pLoadedData;

	NiPoint2* GetMinMaxLandHeight(NiPoint2& apPoint);
	bool GetLandHeight(NiPoint3& apPoint, float* apfHeight);
	SInt32 GetDataX() const;
	SInt32 GetDataY() const;

	bool LoadVertices(bool abLoad3D);
};
static_assert(sizeof(TESObjectLAND) == 0x2C);