#pragma once

class NiNode;

class TESTerrainLODManager {
public:

	struct ShaderSettings {
		float	x;
		float	y;
		int		iLastEnabledPasses;
		bool	bGrassLoaded;
		bool	b;
		bool	bWaterEnabled;
		bool	d;
	};

	static NiNode* GetGrassNode();
	static ShaderSettings* GetShaderSettings();

	static bool IsGrassLoaded();
	static bool IsWaterEnabled();
};