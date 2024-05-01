#pragma once

#include "NiRefObject.hpp"

class Actor;
class ActorMover;
class PathingRequest;
class PathingSolution;

class ActorPathHandler {
public:
	ActorPathHandler();
	virtual ~ActorPathHandler();

	Actor*						pActor;
	ActorMover*					pMover;
	NiPointer<PathingSolution>	spPathingSolution;
	NiPointer<PathingRequest>	spPathingRequest;
};

ASSERT_SIZE(ActorPathHandler, 0x14);