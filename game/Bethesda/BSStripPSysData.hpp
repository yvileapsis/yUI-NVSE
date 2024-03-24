#pragma once

#include "NiPSysData.hpp"

NiSmartPointer(BSStripPSysData);

class BSStripPSysData : public NiPSysData {
public:
	BSStripPSysData();
	virtual ~BSStripPSysData();

	struct PointData {
		UInt32		unk0;
		UInt32		unk4;
		UInt32		unk8;
		UInt32		unkC;
		UInt32		unk10;
		UInt32		unk14;
		UInt32		unk18;
		UInt32		unk1C;
		UInt32		unk20;
		UInt32		unk24;
		UInt32		unk28;
		UInt32		unk2C;
		UInt32		unk30;
		UInt32		unk34;
		UInt32		unk38;
		UInt32		unk3C;
		UInt32		unk40;
		UInt32		unk44;
		UInt32		unk48;
		UInt32		unk4C;
		UInt32		unk50;
		UInt32		unk54;
		UInt32		unk58;
		UInt32		unk5C;
		UInt32		unk60;
		UInt32		unk64;
		UInt32		unk68;
		NiColorA	kColor;
		float		unk7C;
		float		unk80;
	};


	struct StripData {
		PointData*	pPointList;
		UInt16		usPointCount;
		UInt16		usHighestPointCount;
		UInt16*		unk8;
		UInt16*		unkC;
		UInt16*		usMaxPointCount;
		SInt32		iFirstIndex;
		SInt32		iLastIndex;
		float		fLastTime;
	};


	StripData*	pStripList;
	UInt16		usMaxPointCount;
	float		fStartCapSize;
	float		fEndCapSize;
	bool		bDoZPrePass;

	CREATE_OBJECT(BSStripPSysData, 0xC25CA0);
};

ASSERT_SIZE(BSStripPSysData, 0x90);