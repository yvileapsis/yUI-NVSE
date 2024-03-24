#pragma once

#include "NiNode.hpp"
#include "BSFaceGenAnimationData.hpp"
#include "BSCullingProcess.hpp"

class Actor;

NiSmartPointer(BSFaceGenNiNode);

class BSFaceGenNiNode : public NiNode {
public:
	BSFaceGenNiNode();
	virtual ~BSFaceGenNiNode();
	virtual BSFaceGenAnimationData*		GetAnimData();
	virtual void						SetAnimData(BSFaceGenAnimationData* apData);
	virtual NiMatrix3*					GetRotation(NiMatrix3& arOut);
	virtual void						SetRotation(NiMatrix3& arRot);
	virtual bool						GetAnimationUpdate();
	virtual void						SetAnimationUpdate(bool abVal);
	virtual bool						GetApplyRotToParent();
	virtual void						SetApplyRotToParent(bool abVal);
	virtual bool						GetFixedNormals();
	virtual void						SetFixedNormals(bool abVal);
	virtual void						FixSkinInstances(NiNode* apNode, bool abWarnOnFail);

	BSFaceGenAnimationDataPtr	spAnimationData;
	NiMatrix3					kBaseRotation;
	bool						bForceBaseMorph;
	bool						bFixedNormals;
	bool						bAnimationUpdate;
	bool						bRotatedLastUpdate;
	bool						bApplyRotationToParent;
	float						fLastTime;
	bool						bUsingLoResHead;
	bool						bIAmPlayerCharacter;
	bool						bIsInDialogue;
	bool						padE3;
	bool						bForceUpdate;
	Actor*						pActor;
	UInt8						byteEC;

	static void __fastcall OnVisibleEx(BSFaceGenNiNode* apThis, void*, BSCullingProcess* apCuller);
};

ASSERT_SIZE(BSFaceGenNiNode, 0xF0)