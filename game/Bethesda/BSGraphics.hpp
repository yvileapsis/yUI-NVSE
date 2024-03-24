#pragma once

#include "NiDX9Renderer.hpp"

enum TextureFilterMode
{
	TEXTURE_FILTER_MODE_NEAREST = 0x0,
	TEXTURE_FILTER_MODE_BILERP = 0x1,
	TEXTURE_FILTER_MODE_TRILERP = 0x2,
	TEXTURE_FILTER_MODE_ANISO = 0x3,
	TEXTURE_FILTER_MODE_COMP_BILERP = 0x4,
	TEXTURE_FILTER_MODE_COUNT = 0x5,
	TEXTURE_FILTER_MODE_DEFAULT = 0x3,
};

enum TextureAddressMode
{
	TEXTURE_ADDRESS_MODE_CLAMP_S_CLAMP_T = 0x0,
	TEXTURE_ADDRESS_MODE_CLAMP_S_WRAP_T = 0x1,
	TEXTURE_ADDRESS_MODE_WRAP_S_CLAMP_T = 0x2,
	TEXTURE_ADDRESS_MODE_WRAP_S_WRAP_T = 0x3,
	TEXTURE_ADDRESS_MODE_COUNT = 0x4,
	TEXTURE_ADDRESS_MODE_DEFAULT = 0x0,
};

class BSGraphics {
public:
	class OcclusionQuery : public BSMemObject {
	public:
		OcclusionQuery();
		~OcclusionQuery();

		IDirect3DQuery9* pOcclusionQuery;

		bool Begin();
		void End();

		bool GetOcclusionQueryResults(void* apData, bool abFlush);
	};

	static NiPoint3* GetCameraWorldTranslate() {
		return &(*(NiPoint3*)0x11F474C);
	}

	static bool* const bLetterBox;
	static bool* const bOcclusionQueryActive;

	static bool IsLetterBox(); // Need to verify in 1000%, but should be this

	static bool IsEyefinity() {
		return *(bool*)0x11C70ED && *(bool*)0x11C70EE;
	}
};