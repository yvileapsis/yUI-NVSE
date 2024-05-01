#pragma once

#include "BSShader.hpp"
#include "NiPoint4.hpp"

class ParticleShader : public BSShader {
public:
	DWORD					dword6C;
	DWORD					InstanceData;
	NiD3DPassPtr			spPass0;
	NiD3DPassPtr			spPass1;
	NiD3DVertexShaderPtr	spVertexShader;
	NiD3DPixelShaderPtr		spPixelShader;
	NiPoint4				fVars[5];
	NiPoint3				Velocity;
	NiPoint3				Acceleration;
	NiPoint4				Wind;
	NiColorA				Color1;
	NiColorA				Color2;
	NiColorA				Color3;

	static void __fastcall SetupTexturesEx(ParticleShader* apThis, void*, NiPropertyState* apProperties);


	static __forceinline float GetFOVAdjust() {
		return *(float*)0x11ADCC8;
	}

	static __forceinline void SetFOVAdjust(float afFOVAdjust) {
		*(float*)0x11ADCC8 = afFOVAdjust;
	}
};