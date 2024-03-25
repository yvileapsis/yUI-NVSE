#pragma once

class AStarNode;

class AStarQueue {
public:
	Float32		fMaxFitness;
	AStarNode*	Buckets[20];
};