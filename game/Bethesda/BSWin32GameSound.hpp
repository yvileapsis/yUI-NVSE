#pragma once

#include "BSGameSound.hpp"
#include "BSFile.hpp"
#include <dsound.h>
#include "vorbisfile.h"

class BSWin32GameSound : public BSGameSound {
public:
	BSWin32GameSound();
	virtual ~BSWin32GameSound();

#pragma pack(push, 1)
	struct SoundInfo {
		UInt16	Unk00;
		UInt16	usChannels;
		int		uiSampleRate;
		int		Unk08;
		UInt16	Unk0C;
		UInt16	Unk0E;
		UInt16	Unk10;
	};
#pragma pack(pop)

	struct Unk228 {
		UInt32	unk00;
		UInt32	unk04;
		UInt32	unk08;
		UInt32	unk0C;
		UInt32	unk10;
		UInt32	uiPlayCursor;
		void*	pUnk18;
		void*	pUnk1C;
		void*	pUnk20;
	};


	struct OggVorbisFile : public OggVorbis_File {
		BSFile*			pFile;
		SInt32			iSampleCount;
		vorbis_info*	pInfo;
		bool			bLoaded;
	};


	LPDIRECTSOUND8				pDirectSound8;
	LPDIRECTSOUNDBUFFER8		pSoundBuffer;
	LPDIRECTSOUNDBUFFER8		pReverbBuffer;
	DSBUFFERDESC				kBufferDesc;
	byte*						pRawBuffer;
	LPDIRECTSOUNDBUFFER			pSoundBuffer_1CC;
	LPDIRECTSOUND3DBUFFER8		pSoundBuffer3D_1D0;
	LPDIRECTSOUND3DBUFFER8		pSoundBuffer3D_1D4;
	LPDIRECTSOUND3DBUFFER8		pSoundBuffer3D_1D8;
	UInt32						chunkSizeInBytes_2;
	LPDIRECTSOUNDFXI3DL2REVERB	pReverbEffect;
	UInt32						unk1E4;
	UInt32						unk1E8;
	UInt32						unk1EC;
	DSFXDistortion				kDistortionData;
	UInt32						unk204;
	LPDIRECTSOUNDFXDISTORTION	pDistortionEffect;
	UInt16						unk20C;
	UInt16						unk20E;
	UInt16						unk210;
	UInt16						unk212;
	UInt16						unk214;
	NiPoint3					kPosition;
	bool						bPositionChanged;
	Unk228*						pUnk228;
	OggVorbisFile*				pOggFile;

	static void __fastcall SetDistancesEx(BSWin32GameSound* apThis, void*, float afMinDist, float afMaxDist);
};

ASSERT_SIZE(BSWin32GameSound, 0x230);
ASSERT_SIZE(BSWin32GameSound::SoundInfo, 0x12);
ASSERT_SIZE(BSWin32GameSound::Unk228, 0x24);
