#pragma once

#include "NiCullingProcess.hpp"
#include "BSShaderAccumulator.hpp"

class BSCullingProcess : public NiCullingProcess {
public:
	BSCullingProcess(NiVisibleArray* apVisibleSet = nullptr);
	~BSCullingProcess() {};

	NIRTTI_ADDRESS(0x12030A8)

	const NiRTTI* GetRTTI() const override { return BSCullingProcess::ms_RTTI; };
	void Process(NiAVObject* apObject) override;
	void ProcessAlt(const NiCamera* apCamera, NiAVObject* apScene, NiVisibleArray* apVisibleSet) override;
	void AppendVirtual(NiGeometry* apGeom) override;

	enum BSCPCullingType {
		BSCP_CULL_NORMAL					= 0, // Normal culling
		BSCP_CULL_ALLPASS					= 1, // Everything is visible, but frustum culling still applies
		BSCP_CULL_ALLFAIL					= 2, // Everything is culled
		BSCP_CULL_IGNOREMULTIBOUNDS			= 3, // Doesn't cull using multibounds, frustum still applies
		BSCP_CULL_FORCEMULTIBOUNDSNOUPDATE	= 4, // Culls, but doesn't save the result
	};

	BSCPCullingType					eCullMode;			// Current mode
	BSCPCullingType					eTypeStack[10];		// Stored modes
	UInt32							uiStackIndex;		// Current index of the mode stack
	BSCompoundFrustum*				pCompoundFrustum;	// Frustum
	NiPointer<BSShaderAccumulator>	spAccumulator;		// Current accumulator

	void SetCullMode(BSCPCullingType aeType);
	void PushCullMode(BSCPCullingType aeType);
	void PopCullMode();

	void SetAccumulator(BSShaderAccumulator* pkAccumulator);

	bool AddShared(NiAVObject* apObject);
	void ClearSharedMap();

	static void __fastcall ProcessEx(BSCullingProcess* apThis, void*, NiAVObject* apScene);
	static void __fastcall ProcessAltEx(BSCullingProcess* apThis, void*, const NiCamera* apCamera, NiAVObject* apScene, NiVisibleArray* apVisibleSet);
	static void __fastcall AppendVirtualEx(BSCullingProcess* apThis, void*, NiGeometry* apGeom);
};
ASSERT_SIZE(BSCullingProcess, 0xC8)