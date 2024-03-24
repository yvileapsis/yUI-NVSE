#pragma once

#include "BSMemObject.hpp"
#include "NiTPointerMap.hpp"
#include "NiTList.hpp"
#include "NiAVObject.hpp"
#include "BSSoundMessage.hpp"
#include "BSAudioManagerThread.hpp"
#include "AudioLoadTask.hpp"
#include "BSSoundInfo.hpp"

class BSGameSound;

class SoundMessageStack {
public:
	UInt32			uiCount;
	bool			bBottomless;
	BSSoundMessage* pTop;
	UInt32			uiTimeLastAllocated;

	void Push(BSSoundMessage* apMessage);
	BSSoundMessage* Pop();
};

typedef NiTPointerList<BSSoundMessage*> SoundMessageList;

class BSAudioManager : public BSMemObject {
public:
	BSAudioManager();
	virtual ~BSAudioManager();

	struct UnkStruct {
		UInt32 uiFlags;
		UInt16 usState;
	};


	UInt32									uiMessageListIndex;
	UInt32									uiMessageProcessingListIndex;
	SoundMessageList						kMessages[2];
	SoundMessageStack						kFreeMessages;
	SoundMessageStack						kRequest034;
	SoundMessageStack						kRequest044;
	NiTPointerMap<UInt32, BSGameSound*>		kPlayingSounds;
	NiTPointerMap<UInt32, BSSoundInfo*>		kPlayingSoundInfos1;
	NiTPointerMap<UInt32, BSSoundInfo*>		kDebugSoundInfos;
	NiTPointerMap<UInt32, NiAVObjectPtr>	kMovingObjects;
	NiTPointerList<BSGameSound*>			kCachedSounds;
	UInt32									unk0A0;
	UInt32									unk0A4;
	float									flt0A8;
	CRITICAL_SECTION						kMessageCS;
	CRITICAL_SECTION						kDebugCS;
	CRITICAL_SECTION						kCacheListCS;
	CRITICAL_SECTION						kMessageProcessingCS;
	CRITICAL_SECTION						kTaskCS;
	NiTPointerList<AudioLoadTask*>			kTaskList;
	UInt32									uiLastTickCount;
	bool									bInitedSounds;
	bool									byte135;
	UInt32									uiThreadID;
	BSAudioManagerThread*					pUpdateThread;
	float									fVolumes[12];
	UInt32									unk170;
	bool									byte174;
	bool									bIsPlayerUnderwater;
	UnkStruct*								pUnk178;
	UInt32									uiSomeCount_17C; // Counter for 178
	UInt32									uiSoundCacheIndex;
	bool									bIgnoreTimescale;
	bool									byte185;
	bool									byte186;
	bool									byte187;

	static BSAudioManager* GetSingleton();

	BSSoundMessage* PopFreeMessage();

	void SetCacheEnabled(bool abEnable);
	void SetAudioCacheSize(UInt32 auiSize);

	void Precache(BSGameSound* apSound);
	BSGameSound* GetFromCache(UInt32 auiSourceID, UInt32 auiAlternateID, BSGameSound::SoundFlags aeFlags);
	bool IsInCache(UInt32 auiSoundID, bool abCheckAlternateID);

	bool IsValid(UInt32 auiSoundID);
	bool Play(UInt32 auiSoundID, bool abUnk);
	bool SetStaticAttenuation(UInt32 auiSoundID, UInt16 ausAttenuation);
	bool SetReverbAttenuation(UInt32 auiSoundID, UInt16 ausAttenuation);
	bool SetVolume(UInt32 auiSoundID, float afVolume);
	bool SetSpeed(UInt32 auiSoundID, float afSpeed);
	bool SetPosition(UInt32 auiSoundID, NiPoint3& arPosition);
	bool SetMinMaxDistance(UInt32 auiSoundID, float afMin, float afMax);
	bool SetAttenuationCurve(UInt32 auiSoundID, SInt16 ausUnk1, SInt16 ausUnk2, UInt16 ausStaticAttenuation, UInt16 ausSystemAttenuation, UInt16 ausDistanceAttenuation);
	bool Stop(UInt32 auiSoundID);
	bool Release(UInt32 auiSoundID);

	static double dVoiceOffset;
};

ASSERT_SIZE(BSAudioManager, 0x188);