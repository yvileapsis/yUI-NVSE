#pragma once

class AStarNode;

class AStarQueue {
public:
	float fMaxFitness;
	AStarNode* Buckets[20];
};