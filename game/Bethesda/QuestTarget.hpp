#pragma once

#include "NiPoint3.hpp"
#include "BSSimpleArray.hpp"

class ParentSpaceNode;
class TeleportLink;

struct QuestTarget {
	BSSimpleArray<ParentSpaceNode*> parentSpaceNodes;
	BSSimpleArray<TeleportLink*> teleportLinks;
	NiPoint3 startPos;
	NiPoint3 endPos;
};