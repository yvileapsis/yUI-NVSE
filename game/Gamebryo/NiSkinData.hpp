#pragma once

#include "NiObject.hpp"
#include "NiSkinPartition.hpp"
#include "NiTransform.hpp"
#include "NiBound.hpp"

NiSmartPointer(NiSkinData);

class NiSkinData : public NiObject {
public:
	NiSkinData();
	~NiSkinData();

	class BoneVertData {
	public:
		UInt16	m_usVert;
		float	m_fWeight;
	};

	class BoneData {
	public:
		NiTransform		m_kSkinToBone;
		NiBound			m_kBound;
		BoneVertData* BoneVertData;
		UInt16			m_pkBoneVertData;
	};

	NiSkinPartition* m_spSkinPartition;
	NiTransform			m_kRootParentToSkin;
	BoneData* m_pkBoneData;
	UInt32				m_uiBones;

	CREATE_OBJECT(NiSkinData, 0xA84D30)
};

ASSERT_SIZE(NiSkinData, 0x48);