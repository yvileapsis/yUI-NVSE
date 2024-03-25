#pragma once
#include "TESObjectREFR.hpp"
#include "BaseProcess.hpp"

// 0x88
class MobileObject : public TESObjectREFR
{
public:
	MobileObject();
	~MobileObject() override;
	virtual void			MovetoHigh(); // 0x90
	virtual void			MoveToLow();
	virtual void			MoveToMiddleLow();
	virtual void			MovetoMiddleHigh();
	virtual void			Move(float, NiPoint3, int);
	virtual void			Jump();
	virtual void			nullsub_96();
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
	virtual void			IsLifeStateDying();
	virtual void			Unk_BB();
	virtual void			ToggleCollisionDebug(bool abShow);
	virtual void			Unk_BD();
	virtual void			Update(float afDelta);
	virtual void			Unk_BF();
	virtual TESSound*		GetActivatorCloseSound(void*, void*);

	BaseProcess*		pkBaseProcess;						// 068
	TESObjectREFR*		pkActivatorRef;
	Actor*				pkConversationActor;				// 070 -
	Float32				nextHeadTrackUpdateTimer;			// 074 - loaded
	Float32				voiceTimer;							// 078 - loaded
	UInt8				unk07C;								// 07C - loaded
	UInt8				unk07D;								// 07D - loaded
	UInt8				unk07E;								// 07E - loaded
	UInt8				unk07F;								// 07F - loaded
	bool				bTalkingThroughActivator;
	bool				bChangingProcess;
	UInt8				unk083;								// 083 -	bool bSoundCallBackSet;
	UInt8				unk085;								// 085 - loaded
	UInt8				ucExtraInfoGeneralTopicDataElementFlags;		// 086 - loaded
	UInt8				unk087;								// 087	Init'd to the inverse of NoLowLevelProcessing
	
	bool IsTalkingThroughActivator() const { return bTalkingThroughActivator; };
	TESObjectREFR* GetActivatorRef() const { return pkActivatorRef; };

	void RemoveStaleSayOncePerDayTopics();
};
static_assert(sizeof(MobileObject) == 0x88);