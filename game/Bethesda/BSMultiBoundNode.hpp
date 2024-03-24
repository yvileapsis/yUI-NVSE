#pragma once

#include "BSNiNode.hpp"
#include "BSMultiBound.hpp"

NiSmartPointer(BSMultiBoundNode);

class BSCullingProcess;
class BSOcclusionPlane;
class BSMultiBoundRoom;

class BSMultiBoundNode : public BSNiNode {
public:
	BSMultiBoundNode();
	virtual ~BSMultiBoundNode();

	virtual BSMultiBoundRoom*	GetMultiBoundRoom();
	virtual bool				GetPointWithin(NiPoint3& akPoint);
	virtual UInt32				CheckBound(BSMultiBound*);
	virtual UInt32				CheckBoundAlt(NiBound*);

	BSMultiBoundPtr spMultiBound;
	UInt32			uiCullingMode;

	CREATE_OBJECT(BSMultiBoundNode, 0xC46DF0);
	NIRTTI_ADDRESS(0x1202E74);

	BSMultiBound* GetMultiBound();
	void SetMultiBound(BSMultiBound* apMultiBound);
	void SetCullingMode(UInt32 aiMode);
	void ResetCullResult();
	bool VisibleAgainstActiveOccluders(NiPoint3 akViewPoint, BSCullingProcess& arCuller, BSOcclusionPlane* apOccluder);
	
	static void __fastcall OnVisibleEx(BSMultiBoundNode* apThis, void*, BSCullingProcess* apCuller);
	static void __fastcall UpdateWorldBoundEx(BSMultiBoundNode* apThis);

};

ASSERT_SIZE(BSMultiBoundNode, 0xB4);