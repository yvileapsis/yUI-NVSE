#pragma once

#include "NiRefObject.hpp"
#include "ActorPathingMessage.hpp"
#include "BSTCommonLLMessageQueue.hpp"

NiSmartPointer(ActorPathingMessageQueue);

class ActorPathingMessageQueue : public BSTCommonLLMessageQueue<ActorPathingMessage>, public NiRefObject {
public:
};