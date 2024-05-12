#pragma once

#include "NiObject.hpp"

NiSmartPointer(NiAdditionalGeometryData);

class BSPackedAdditionalGeometryData;

class NiAdditionalGeometryData : public NiObject {
public:
	enum Types : UInt32 {
		AGD_NITYPE_INVALID	= 0,
		AGD_NITYPE_FLOAT1	= 1,
		AGD_NITYPE_FLOAT2	= 2,
		AGD_NITYPE_FLOAT3	= 3,
		AGD_NITYPE_FLOAT4	= 4,
		AGD_NITYPE_LONG1	= 5,
		AGD_NITYPE_LONG2	= 6,
		AGD_NITYPE_LONG3	= 7,
		AGD_NITYPE_LONG4	= 8,
		AGD_NITYPE_ULONG1	= 9,
		AGD_NITYPE_ULONG2	= 10,
		AGD_NITYPE_ULONG3	= 11,
		AGD_NITYPE_ULONG4	= 12,
		AGD_NITYPE_SHORT1	= 13,
		AGD_NITYPE_SHORT2	= 14,
		AGD_NITYPE_SHORT3	= 15,
		AGD_NITYPE_SHORT4	= 16,
		AGD_NITYPE_USHORT1	= 17,
		AGD_NITYPE_USHORT2	= 18,
		AGD_NITYPE_USHORT3	= 19,
		AGD_NITYPE_USHORT4	= 20,
		AGD_NITYPE_BYTE1	= 21,
		AGD_NITYPE_BYTE2	= 22,
		AGD_NITYPE_BYTE3	= 23,
		AGD_NITYPE_BYTE4	= 24,
		AGD_NITYPE_UBYTE1	= 25,
		AGD_NITYPE_UBYTE2	= 26,
		AGD_NITYPE_UBYTE3	= 27,
		AGD_NITYPE_UBYTE4	= 28,
		AGD_NITYPE_BLEND1	= 29,
		AGD_NITYPE_BLEND2	= 30,
		AGD_NITYPE_BLEND3	= 31,
		AGD_NITYPE_BLEND4	= 32,
		AGD_NITYPE_COUNT	= 33,
		AGD_NITYPE_CUSTOM	= 34,
	};


	class NiAGDDataStream {
	public:
		UInt8  m_ucflags;
		UInt32 m_uiType;
		UInt32 m_uiUnitSize;
		UInt32 m_uiTotalSize;
		UInt32 m_uiStride;
		UInt32 m_uiBlockIndex;
		UInt32 m_uiBlockOffset;
	};

	class NiAGDDataBlock : public NiMemObject {
	public:
		NiAGDDataBlock();
		virtual ~NiAGDDataBlock();
		virtual void*	Allocate(void*);
		virtual void	Unk2(UInt8* apucDataBlock, UInt32 auiBlockSize); // Does effectively nothing
		virtual void	Unk3(UInt8* apucDataBlock, UInt32 auiBlockSize); // Does effectively nothing
		virtual void	Deallocate(void*);

		UInt32	m_uiDataBlockSize;
		UInt8*	m_pucDataBlock;
		bool	bIsCopied;
		bool	bIsSmall; // Set below 64 verts + copied
	};

	NiAdditionalGeometryData();
	virtual ~NiAdditionalGeometryData();

	virtual bool			IsBSPackedAdditionalGeometryData();
	virtual NiAGDDataBlock* CreateDataBlock();

	UInt32								m_uiRevID;
	UInt16								m_usVertexCount;
	UInt32								m_uiDataStreamCount;
	NiAGDDataStream*					m_pkDataStreamEntries;
	UInt32								unk18; // Set to 1 by Precipitation
	NiTPrimitiveArray<NiAGDDataBlock*>	m_aDataBlocks;

	CREATE_OBJECT(NiAdditionalGeometryData, 0xA73E20);
	static NiAdditionalGeometryData* Create(UInt16 ausVertCount, UInt32 auiObjectCount);

	bool SetDataBlock(UInt32 uiIndex, UInt8* pucData, UInt32 uiTotalSize, bool bCopyData);
	void SetDataBlockCount(UInt32 auiCount);
	bool SetDataStream(UInt32 auiStreamIndex, UInt32 auiBlockIndex, UInt32 auiBlockOffset, Types auiType, UInt16 ausCount, UInt32 auiUnitSize, UInt32 auiStride);
	bool GetDataStream(UInt32 auiStreamIndex, UInt8*& apucData, UInt32& auiType, UInt16& ausCount, UInt32& auiTotalSize, UInt32& auiUnitSize, UInt32& auiStride);

	static const char* GetTypeName(UInt32 auiType);
};

ASSERT_SIZE(NiAdditionalGeometryData, 0x2C)

#define IS_PACKED(object) ((*(UInt32**)object)[35 * 4 >> 2] == 0x8D0360)