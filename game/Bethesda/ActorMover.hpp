#pragma once

#include "PathingLocation.hpp"
#include "ActorPathingMessageQueue.hpp"
#include "PathingRequest.hpp"
#include "PathingSolution.hpp"

class DetailedActorPathHandler;

class ActorMover {
public:
	ActorMover();
	
	virtual				~ActorMover();
	virtual void		SetMovementFlag(UInt16);
	virtual void		ClearMovementFlag();
	virtual void		SetMovementFlags(UInt16 movementFlags);
	virtual void		Unk_04();
	virtual void		Unk_05();
	virtual void		HandleTurnAnimationTimer(float timePassed);
	virtual void		Unk_07();
	virtual UInt32		GetMovementFlags();
	virtual void		GetOverrideMovementVector();
	virtual void		SaveGame();
	virtual void		LoadGame();
	virtual void		Unk_0C();
	virtual void		Unk_0D();
	virtual void		Reset();

	enum MovementFlags {
		kMoveFlag_Forward		= 0x1,
		kMoveFlag_Backward		= 0x2,
		kMoveFlag_Left			= 0x4,
		kMoveFlag_Right			= 0x8,
		kMoveFlag_TurnLeft		= 0x10,
		kMoveFlag_TurnRight		= 0x20,
		kMoveFlag_NonController = 0x40,
		kMoveFlag_Walking		= 0x100,
		kMoveFlag_Running		= 0x200,
		kMoveFlag_Sneaking		= 0x400,
		kMoveFlag_Swimming		= 0x800,
		kMoveFlag_Jump			= 0x1000,
		kMoveFlag_Flying		= 0x2000,
		kMoveFlag_Fall			= 0x4000,
		kMoveFlag_Slide			= 0x8000,
	};


	NiPoint3						kPt04;							
	NiPoint3						kOverrideMovement;				
	PathingRequestPtr				spPathingRequest;			
	PathingSolutionPtr				spPathingSolution;
	DetailedActorPathHandler*		pPathHandler;	
	Actor*							pActor;							
	UInt32							uiDoorRefIDForPathing;				
	ActorPathingMessageQueuePtr		spPathingMsgQueue;
	UInt32							uiMovementFlags1;
	UInt32							unk38;							
	UInt32							uiMovementFlags2;
	UInt16							wrd40;							
	UInt16							wrd42;							
	PathingLocation					kPathingLocation;		
	UInt32							unk6C;							
	bool							bPathingFailed;
	UInt8							byte71;							
	UInt8							byte72;							
	UInt8							byte73;							
	bool							bIsWaitingOnPath;
	UInt8							byte75;							
	UInt8							byte76;							
	bool							bIsOverrideMovement;				
	UInt8							byte78;							
	UInt32							uiTime7C;							
	UInt32							uiTime80;							
	UInt32							uiCount84;							
};

ASSERT_SIZE(ActorMover, 0x88);