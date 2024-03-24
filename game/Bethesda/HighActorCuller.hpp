#pragma once

#include "MobIterOperator.hpp"
#include "BSMultiBound.hpp"
#include "BSMultiBoundNode.hpp"
#include "BSFadeNodeCuller.hpp"
#include "BSMultiBoundSphere.hpp"

class HighActorCuller : public MobIterOperator {
public:
	HighActorCuller();
	~HighActorCuller();

	BSFadeNodeCuller		culler;
	BSMultiBoundNodePtr		spMBNode;
	BSMultiBoundPtr			spMultiBound;
	BSMultiBoundSpherePtr	spMBSphere;

	void Operate(TESObjectREFR* apRef) override;

	static bool bCreated;

	static HighActorCuller* GetSingleton();

	static void Run(NiCamera* apCamera);
};