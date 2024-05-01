#pragma once

struct GrassParam {
	GrassParam();

	const char* pcGeomFilename;
	UInt32 iGrassID;
	float fPositionRange;
	float fHeightRange;
	float fColorRange;
	float fWaveOffsetRange;
	float fWavePeriod;
	bool bVertexLighting;
	bool bUniformScaling;
	bool bFitToSlope;
	bool bWind;
};


struct TESGrassAreaParam {
	GrassParam kGrassParam;
	float pfDensity[9];
};
