#pragma once

#include "BSShaderAccumulator.hpp"
#include "BSShaderManager.hpp"
#include "BSCullingProcess.hpp"
#include "NiCamera.hpp"

class BSCullingProcess;

class BSShaderUtil {
public:
	static PerfCounter accumulateGeometryCounter;

	static void RenderScene(NiCamera* apCamera, BSShaderAccumulator* apAccumulator);
	static void __cdecl AccumulateGeometry(const NiCamera* apCamera, NiAVObject* apNode, BSCullingProcess* apCullingProcess);
	static void AccumulateGeometryList(const NiCamera* apCamera, void* apGeometryList, BSCullingProcess* apCullingProcess);
};