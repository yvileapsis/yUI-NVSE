#pragma once

#include "NiObject.hpp"
#include "NiColor.hpp"
#include "NiTSet.hpp"
#include "NiTexturingProperty.hpp"

NiSmartPointer(NiScreenTexture);

class NiScreenTexture : public NiObject {
public:
	NiScreenTexture();
	~NiScreenTexture();

	class ScreenRect : public NiMemObject {
	public:
		SInt16 m_sPixTop;
		SInt16 m_sPixLeft;
		UInt16 m_usPixWidth;
		UInt16 m_usPixHeight;

		UInt16 m_usTexTop;
		UInt16 m_usTexLeft;

		NiColorA m_kColor;
	};

	NiTPrimitiveSet<ScreenRect> m_kScreenRects;
	NiTexturingPropertyPtr m_spTexProp;
	UInt16 m_usDirtyFlags;
	void* m_pkBuffData;

	CREATE_OBJECT(NiScreenTexture, 0xA958B0)
};

ASSERT_SIZE(NiScreenTexture, 0x20)