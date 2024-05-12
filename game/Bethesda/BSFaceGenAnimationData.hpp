#pragma once

#include "BSFaceGenKeyframeMultiple.hpp"
#include "NiExtraData.hpp"
#include "NiPoint3.hpp"
#include "NiTList.hpp"

NiSmartPointer(BSFaceGenAnimationData);

class BSFaceGenAnimationData : public NiExtraData {
public:
	BSFaceGenKeyframeMultiple*	frameMultiple00C;
	BSFaceGenKeyframeMultiple	faceGenFrame010;
	BSFaceGenKeyframeMultiple	faceGenFrame024;
	BSFaceGenKeyframeMultiple	faceGenFrame038;
	BSFaceGenKeyframeMultiple	faceGenFrame04C;
	BSFaceGenKeyframeMultiple	faceGenFrame060;
	BSFaceGenKeyframeMultiple	faceGenFrame074;
	NiTList<UInt32>				list088;
	BSFaceGenKeyframeMultiple	faceGenFrame094;
	BSFaceGenKeyframeMultiple	faceGenFrame0A8;
	NiTList<UInt32>				list0BC;
	BSFaceGenKeyframeMultiple	faceGenFrame0C8;
	BSFaceGenKeyframeMultiple	faceGenFrame0DC;
	NiTList<UInt32>				list0F0;
	BSFaceGenKeyframeMultiple	faceGenFrame0FC;
	BSFaceGenKeyframeMultiple	faceGenFrame110;
	NiPoint3					pt124;
	float						unk130;
	float						unk134;
	float						unk138;
	float						unk13C;
	float						unk140;
	float						unk144;
	UInt32						unk148;
	UInt8						byte14C;
	UInt8						gap14D;
	UInt8						gap14E;
	UInt8						gap14F;
	float						unk150;
	float						unk154;
	float						unk158;
	float						unk15C;
	float						unk160;
	float						unk164;
	float						unk168;
	float						unk16C;
	float						unk170;
	UInt32						unk174;
	float						unk178;
	float						unk17C;
	float						unk180;
	float						unk184;
	UInt8						byte188;
	UInt8						byte189;
	UInt8						gap18A;
	bool						bChangedLastUpdate;
	bool						bMustUpdate;
	bool						bDoNotUpdateNextTime;
	bool						bDead;
	UInt8						sex18F;
	UInt8						ucLipSynchState;
	UInt8						byte191;
	UInt8						gap192;
	UInt8						gap193;
	float						unk194;
	UInt32						unk198;
	UInt32						unk19C;
	BSSpinLock					KAnimLock;
};

ASSERT_SIZE(BSFaceGenAnimationData, 0x1C0)