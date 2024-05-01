#pragma once
#include "NiPoint3.hpp"
#include "BSSimpleArray.hpp"

class ParentSpaceNode;
class TeleportLink;

// 0x38
struct QuestTarget {
	BSSimpleArray<ParentSpaceNode*> parentSpaceNodes;
	BSSimpleArray<TeleportLink*> teleportLinks;
	NiPoint3 startPos;
	NiPoint3 endPos;
};
static_assert(sizeof(QuestTarget) == 0x38);