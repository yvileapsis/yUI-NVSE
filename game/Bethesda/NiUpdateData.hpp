#pragma once

class NiUpdateData {
public:
	NiUpdateData(float afTime = 0.f, bool abUpdateControllers = false, bool abIsMultiThreaded = false, bool abMTParticles = false, bool abUpdateGeomorphs = false, bool abUpdateShadowSceneNode = false)
		: fTime(afTime), bUpdateControllers(abUpdateControllers), bIsMultiThreaded(abIsMultiThreaded), bMTParticles(abMTParticles), bUpdateGeomorphs(abUpdateGeomorphs), bUpdateShadowSceneNode(abUpdateShadowSceneNode)
	{}
	~NiUpdateData() {};

	float fTime;
	bool bUpdateControllers;
	bool bIsMultiThreaded;
	bool bMTParticles;
	bool bUpdateGeomorphs;
	bool bUpdateShadowSceneNode;

	static NiUpdateData kDefaultUpdateData;
};