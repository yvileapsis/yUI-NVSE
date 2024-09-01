#pragma once
struct TLSData {
	enum HavokSyncMode {
		NONE		= 0,
		UNK_1		= 1,
		UNK_2		= 2,
		CONTROLLER	= 3,
	};

	UInt32					unk000;
	UInt32					unk004;
	class ExtraDataList*	pLastExtraList;		
	UInt32					unk00C;
	class BSExtraData*		xDatas[0x93];
	bool					bLoadedActorIsHighDetail;
	bool					bLoadingTempCellData;
	class NiNode*			pLastNiNode;
	class TESObjectREFR*	pLastNiNodeREFR;
	bool					bConsoleMode;
	UInt32					unk26C[4];
	class TESForm*			pLastRefVar;
	SInt32					iLastVarIndex;
	class ScriptLocals*		pLastEventList;
	class Script*			pLastScript;
	UInt32					uiActivateRecursionDepth;
	UInt32					unk290;
	UInt32					uiFlags;
	bool					bUnk298;
	UInt32					unk29C;
	float					fLastScaledTime;
	UInt32					eLastCycle;
	float					fLastWeightedPhaseTime;
	float					fLastLoKeyTime;
	float					fLastHiKeyTime;
	UInt32					uiHeapIndex;
	UInt32					uiWarningCount;
	UInt32					uiAccumulatorCount;
    UInt32					eHavokSyncMode;

	static TLSData* GetTLSData();
	static UInt32 GetHeapIndex();
	static UInt32 GetAccumulatorCount();
	static void SetHeapIndex(UInt32 index);
};
ASSERT_SIZE(TLSData, 0x2C4);