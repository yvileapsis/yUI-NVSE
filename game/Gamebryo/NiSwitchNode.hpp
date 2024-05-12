#pragma once

#include "NiNode.hpp"

NiSmartPointer(NiSwitchNode);

class NiSwitchNode : public NiNode {
public:
	NiSwitchNode();
	~NiSwitchNode();

	enum {
		UPDATE_ONLY_ACTIVE_CHILD = 1,
		UPDATE_CONTROLLERS		 = 2,
	};

	Bitfield16					m_usFlags;
	SInt32						m_iIndex;
	float						m_fSavedTime;
	UInt32						m_uiRevID;
	NiTPrimitiveArray<UInt32>	m_kChildRevID;

	CREATE_OBJECT(NiSwitchNode, 0xA94550);

	NiAVObject* GetActiveChild();

	bool GetUpdateOnlyActiveChild() const;
	void SetUpdateOnlyActiveChild(bool abEnable);
	
	bool GetUpdateControllers() const;
	void SetUpdateControllers(bool abEnable);

	float GetSavedTime() const;

	static void __fastcall OnVisibleEx(NiSwitchNode* apThis, void*, NiCullingProcess* apCuller);
};

ASSERT_SIZE(NiSwitchNode, 0xCC)