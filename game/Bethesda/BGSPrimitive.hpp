#pragma once
#include "NiColor.hpp"
#include "NiPoint3.hpp"
#include "NiSmartPointer.hpp"
#include "NiGeometry.hpp"
#include "BSFadeNode.hpp"

class BSMultiBoundShape;

// 0x34
class BGSPrimitive {
public:
	virtual ~BGSPrimitive();
	virtual void Build();
	virtual void SetColor(NiColorA*);
	virtual void SetBounds(NiPoint3*);
	virtual bool TestBounds(NiPoint3*);
	virtual BSMultiBoundShape*  MakeMultiBoundShape(NiPoint3*);

	enum PrimitiveType : UInt32 {
		BOX = 0x1,
		SPHERE = 0x2,
		PLANE = 0x3,
	};

	PrimitiveType	eType;
	NiColorA		kColor;
	NiPoint3		kRadii;
	NiPointer<NiGeometry> spGeometry;
	NiPointer<NiGeometry> spFrame;
	NiPointer<BSFadeNode> spNode;
	UInt32 unk30;
};
static_assert(sizeof(BGSPrimitive) == 0x34);