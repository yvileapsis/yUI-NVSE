#pragma once

#include "TESObjectREFR.hpp"
#include "BaseProcess.hpp"

class MobileObject : public TESObjectREFR {
public:
	MobileObject();
	virtual ~MobileObject();

	virtual bool			MovetoHigh();		// 090
	virtual bool			MoveToLow();
	virtual bool			MoveToMiddleLow();
	virtual bool			MovetoMiddleHigh();
	virtual void			Move(float, NiPoint3, int);
	virtual void			Jump();
	virtual void			Unk_96();
	virtual void			Process();
	virtual void			UpdateProcessLevel();
	virtual void			Unk_99();
	virtual void			UpdateAnimation(float = 0);
	virtual void			Unk_9B();
	virtual bool			Unk_9C();
	virtual bool			IsNPCInCombat(bool abNoSearch) const;
	virtual void			SetIsInDialogMenu(bool abInDialog);
	virtual void			Unk_9F();
	virtual void			StartConversation();	// StartConversation(targetActor, subjectLocationData, targetLocationData, headTrack, allowMovement, arg, topicID, arg, arg
	virtual void			DoSpeechLoadLipFiles();
	virtual void			Unk_A2();
	virtual void			SetRunOnceExtraPackage(TESPackage* apPackage, UInt8 aucDay);
	virtual bool			HasStartingWorldOrCellA3();
	virtual TESWorldSpace*	GetStartingWorldspace();
	virtual TESObjectCELL*	GetStartingCell();
	virtual NiPoint3*		GetStartingPos(NiPoint3&);
	virtual void			Unk_A8();
	virtual void			Unk_A9(NiPoint3&);
	virtual void			SetPosition(NiPoint3& arPosition);
	virtual void			Unk_AB();
	virtual void			HandleRunDetection();
	virtual void			GetDetectionTimer();
	virtual void			Unk_AE();
	virtual float			AdjustRot(UInt32 arg1);
	virtual void			Unk_B0();
	virtual void			SetRotation();
	virtual void			GetCurrentTarget();
	virtual bool			IsAtPoint(NiPoint3& arPoint, float afRadius, bool abExpandRadius, bool abAlwaysTestHeight);
	virtual UInt32			GetExpression();
	virtual void			SetExpression(UInt32);
	virtual UInt32			GetEmotionValue();
	virtual void			SetEmotionValue(UInt32);
	virtual void			SetCharControllerSize(float afSize);
	virtual void			Unk_B9();
	virtual void			Unk_BA();
	virtual void			Unk_BB();
	virtual void			ToggleCollisionDebug(bool abShow);
	virtual void			Unk_BD();
	virtual void			Update(float afDelta);
	virtual void			Unk_BF();
	virtual TESSound*		GetActivatorCloseSound(void*, void*);

	BaseProcess*	pBaseProcess;	// 068
	TESObjectREFR*	pActivatorRef;
	TESObjectREFR*	unk70;
	float			unk74;
	float			unk78;
	UInt8			unk7C;
	UInt8			unk7D;
	UInt8			unk7E;
	UInt8			unk7F;
	UInt8			unk80;
	bool			bTalkingThroughActivator;
	bool			bChangingProcess;
	UInt8			unk83;
	bool			bSoundCallbackSet;
	UInt8			unk85;
	Bitfield8		ucExtraInfoGeneralTopicDataElementFlags;
	UInt8			unk87;

	bool IsTalkingThroughActivator() const { return bTalkingThroughActivator; };
	TESObjectREFR* GetActivatorRef() const { return pActivatorRef; };

	void RemoveStaleSayOncePerDayTopics();
};
ASSERT_OFFSET(MobileObject, pBaseProcess, 0x068);
ASSERT_SIZE(MobileObject, 0x088);