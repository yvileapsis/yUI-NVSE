#pragma once

#include "NiTriBasedGeomData.hpp"

NiSmartPointer(NiTriShapeData);

class NiTriShapeData : public NiTriBasedGeomData {
public:
	NiTriShapeData();
	virtual ~NiTriShapeData();

	class SharedNormalArray : public NiMemObject {
	public:
		UInt16	m_usNumSharedNormals;
		UInt16* m_pusSharedNormalIndexArray;
	};

	class SNAMemBlock : public NiMemObject {
	public:
		UInt16* m_pusBlock;
		UInt16* m_pusFreeBlock;
		UInt32	m_uiBlockSize;
		UInt32	m_uiFreeBlockSize;

		SNAMemBlock* m_pkNext;
	};

	UInt32				m_uiTriListLength;
	UInt16*				m_pusTriList;
	SharedNormalArray*	m_pkSharedNormals;
	UInt16				m_usSharedNormalsArraySize;
	SNAMemBlock*		m_pkSNAMemoryBlocks;

	CREATE_OBJECT(NiTriShapeData, 0xA7B790);
	NIRTTI_ADDRESS(0x11F4A88);

	static NiTriShapeData* Create(UInt16 usVertices, NiPoint3* pkVertex, NiPoint3* pkNormal, NiColorA* pkColor, NiPoint2* pkTexture, UInt16 usNumTextureSets, UInt32 eNBTMethod, UInt16 usTriangles, UInt16* pusTriList);

	UInt32 GetTriListLength();
	UInt16* GetTriList();
};

ASSERT_SIZE(NiTriShapeData, 0x58)