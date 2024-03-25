#pragma once
#include "MemoryManager.hpp"
#include "NiNode.hpp"
#include "BSString.hpp"

class BGSSceneInfo {
public:
	BGSSceneInfo();
	virtual ~BGSSceneInfo();

	struct SCENE_INFO_DATA_STRUCT {
		SCENE_INFO_DATA_STRUCT();
		~SCENE_INFO_DATA_STRUCT();

		float pData[16];
	};


	SCENE_INFO_DATA_STRUCT SceneData;
	TESForm* pAssociatedForm;

	BS_ALLOCATOR;

	void SetAssociatedForm(TESForm* apForm);
	TESForm* GetAssociatedForm();

	void SetSceneData(float apData, UInt32 auiIndex);
	float GetSceneData(UInt32 auiIndex);

	double GetMsUsage(UInt32 aeIndex);

	void CollectSceneDataFromNode(NiNode* apNode, bool abIncludeAddonNodes, bool abTrackActors);

	static void CollectSceneDataForLoadedArea(float& afOutTNumber, float& afOutMNumber, bool abCollectTextures, bool abIncludeAddonNodes);
	static void BuildMNumberString(float afMnumber, BSStringT& aOutText, bool abActor);
};