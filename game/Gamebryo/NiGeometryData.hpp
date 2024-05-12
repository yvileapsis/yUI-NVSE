#pragma once

#include "NiObject.hpp"
#include "NiBound.hpp"
#include "NiColor.hpp"
#include "NiPoint2.hpp"
#include "NiAdditionalGeometryData.hpp"
#include "NiGeometryBufferData.hpp"
#include "NiShaderDeclaration.hpp"

NiSmartPointer(NiGeometryData);

class NiTriStripsData;
class NiTriShapeData;
class BSAdditionalGeomDataBlock;

class NiGeometryData : public NiObject {
public:
	NiGeometryData();
	virtual ~NiGeometryData();

	virtual void				SetActiveVertexCount(UInt16 usActive);
	virtual UInt16				GetActiveVertexCount();
	virtual NiTriStripsData*	IsStripsData();
	virtual NiTriShapeData*		IsShapeData();
	virtual bool				ContainsVertexData(NiShaderDeclaration::ShaderParameter eParameter);
	virtual void				CalculateNormals();

	enum Consistency {
		MUTABLE				= 0x0000,
		STATIC				= 0x4000,
		CONSISTENCY_MASK	= 0x7000,
	};

	enum KeepFlags {
		KEEP_NONE		= 0x0,
		KEEP_XYZ		= 0x1,
		KEEP_NORM		= 0x2,
		KEEP_COLOR		= 0x4,
		KEEP_UV			= 0x8,
		KEEP_INDICES	= 0x10,
		KEEP_BONEDATA	= 0x20,
		KEEP_ALL		= 0x3F,
	};

	enum Compression {
		COMPRESS_NORM		= 0x1,
		COMPRESS_COLOR		= 0x2,
		COMPRESS_UV			= 0x4,
		COMPRESS_WEIGHT		= 0x8,
		COMPRESS_POSITION	= 0x10,
		COMPRESS_ALL		= 0x1F,
	};

	enum MarkAsChangedFlags {
		VERTEX_MASK		= 0x1,
		NORMAL_MASK		= 0x2,
		COLOR_MASK		= 0x4,
		TEXTURE_MASK	= 0x8,
		DIRTY_MASK		= 0xFFF,
	};

	UInt16						m_usVertices;
	UInt16						m_usID;
	UInt16						m_usDataFlags;
	UInt16						m_usDirtyFlags;
	NiBound						m_kBound;
	NiPoint3*					m_pkVertex;
	NiPoint3*					m_pkNormal;
	NiColorA*					m_pkColor;
	NiPoint2*					m_pkTexture;
	NiAdditionalGeometryDataPtr m_spAdditionalGeomData;
	NiGeometryBufferData*		m_pkBuffData;
	UInt8						m_ucKeepFlags;
	UInt8						m_ucCompressFlags;
	UInt8						bHasBSData;
	UInt8						Unk3B;
	bool						m_bCanSave;

	static NiGeometryData* Create(UInt16 usVertices, NiPoint3* pkVertex, NiPoint3* pkNormal, NiColorA* pkColor, NiPoint2* pkTexture, UInt16 usNumTextureSets, UInt32 eNBTMethod);
	static NiGeometryData* Create();

	static NiGeometryData* __fastcall CreateEx(NiGeometryData* apThis, void*, UInt16 usVertices, NiPoint3* pkVertex, NiPoint3* pkNormal, NiColorA* pkColor, NiPoint2* pkTexture, UInt16 usNumTextureSets, UInt32 eNBTMethod);


	static void __fastcall LoadBinary_Hook(NiGeometryData* apData, void*, NiStream* kStream);
	static void __fastcall SaveBinary_Hook(NiGeometryData* apData, void*, NiStream* kStream);

	NiPoint3* GetVertices() const;
	NiPoint3* GetNormals() const;
	NiColorA* GetColors() const;
	NiPoint2* GetTextures() const;

	UInt16 GetTextureSets() const;
	NiPoint2* GetTextureSet(UInt16 ausSet);

	UInt16 GetVertCount() const;

	void SetKeepFlags(KeepFlags aeFlags);
	void SetCompressFlags(Compression aeFlags);

	NiGeometryData::Consistency GetConsistency() const;
	void SetConsistency(Consistency aeFlags);

	void SetAdditionalGeomData(NiAdditionalGeometryData* apData);
	void GetBSData(BSAdditionalGeomDataBlock* apData);

	void MarkAsChanged(UInt32 aeFlags);
};

ASSERT_SIZE(NiGeometryData, 0x40);