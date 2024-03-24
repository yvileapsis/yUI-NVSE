#pragma once

struct ActorPathingMessage {
	UInt32					unk00;
	UInt32					unk04;
	ActorPathingMessage*	pNext;
};

ASSERT_SIZE(ActorPathingMessage, 0xC);