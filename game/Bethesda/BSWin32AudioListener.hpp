#pragma once

#include "BSAudioListener.hpp"
#include <dsound.h>

class BSWin32AudioListener : public BSAudioListener {
public:
	BSWin32AudioListener();
	virtual ~BSWin32AudioListener();

	LPDIRECTSOUND3DLISTENER8	pListener;
	NiPoint3					kTopOrientation;
	NiPoint3					kFrontOrientation;
};

ASSERT_SIZE(BSWin32AudioListener, 0x64);