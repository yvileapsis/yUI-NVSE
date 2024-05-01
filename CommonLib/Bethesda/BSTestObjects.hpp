#pragma once

#include "NiPoint3.hpp"
#include "NiPoint4.hpp"
#include "NiColor.hpp"
#include "NiLines.hpp"
#include "NiTriShape.hpp"

namespace BSTestObjects {
	NiLines* MakeLine(const NiPoint3& arStart, const NiColorA& arStartColor, const NiPoint3& arEnd, const NiColorA& arEndColor, bool abUseShader = true);
	NiTriShape* MakePrism(float afScale, const NiColorA& apColor, bool abUseShader = true);
	NiTriShape* MakeTexturedSquare(float afScale, NiPoint3& apCenter, const NiColorA& apColor, bool abUseShader = true);
	NiTriShape* MakeTriangle(NiPoint3 aPoint0, NiPoint3 aPoint1, NiPoint3 aPoint2, NiPoint4* apColor, bool abUseShader = true);
	NiLines* MakeArrow(float afScale, const NiColorA& apColor, bool abUseShader = true);
	NiLines* MakeDirArrow(const NiPoint3& arDirection, const NiColorA& arColor, bool abUseShader = true);
	NiNode* MakeSphere(float afScale, const NiColorA& apColor, bool abUseShader = true);
	//static NiTriShape* MakeTorus();
	//static NiTriShape* MakeCylinder();
	NiLines* MakeCoordinateJack(float afScale, bool abUseShader = true);
	NiTriShape* MakeQuadBox(NiPoint3* apPoint0,
		NiPoint3* apPoint1,
		NiPoint3* apPoint2,
		NiPoint3* apPoint3,
		NiPoint3* apPoint4,
		NiPoint3* apPoint5,
		NiPoint3* apPoint6,
		NiPoint3* apPoint7,
		const NiColorA& apColor,
		bool abUseShader = true);

	NiTriShape* MakeBox(const NiPoint3& apExtents, const NiColorA& apColor, bool abUseShader = true);
	NiTriShape* MakeBox(NiPoint3* arLeftCorner, NiPoint3* arRightCorner, float afForwardDepth, float afBackwardDepth, float afUpwardHeight, float afDownwardHeight, const NiColorA& apLeftColor, const NiColorA& apRightColor, bool abUseShader = true);
	NiTriShape* MakeBox(const NiBound& akBound, const NiColorA& akColor, bool abUseShader = true);
	NiLines* MakeWireBox(NiPoint3 aDrawSize, const NiColorA& apColor, bool abUseShader = true);

}