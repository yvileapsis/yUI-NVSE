#pragma once

#include "BSSoundMessage.hpp"

class TESSound;

class BSGameSound : public BSMemObject {
public:
	BSGameSound();
	virtual				~BSGameSound();
	virtual bool		IsPlaying();
	virtual bool		GetState80000();
	virtual bool		GetState40();
	virtual float		GetVolume();
	virtual void		SetPaused(bool abSet);
	virtual void		SetEnvironmentType(UInt32 aeEnvironmentType);
	virtual bool		ComparePriority(UInt32 auiOtherPriority);
	virtual bool		PriorityTest(BSGameSound* apOtherSound);
	virtual void*		ReadFile();
	virtual void		Unk_0A();
	virtual void		CopyFrom(BSGameSound*, bool);
	virtual void		Start(void*);
	virtual bool		Stop();
	virtual bool		ResetPlayback();
	virtual bool		SetVolume(float afVolume);
	virtual void		Unk_10(); // Update position?
	virtual bool		Unk_11();
	virtual void		Unk_12(); // Stub
	virtual void		SetPosition(NiPoint3 akPos);
	virtual bool		SetPositionF(float afPosX, float afPosY, float afPosZ);
	virtual NiPoint3*	GetPosition(NiPoint3& arPos);
	virtual void		Unk_16(); // Stub
	virtual void		SetDistances(float, float);
	virtual void		Unk_18(UInt16, UInt16, UInt16, UInt16, UInt16);
	virtual bool		SetFrameFrequencyPerc(float);
	virtual float		GetFrameFrequencyPerc();
	virtual void		Seek(UInt32);

	enum SoundFlags {
		FLAG_2D								= 1 << 0,
		FLAG_3D								= 1 << 1,
		FLAG_VOICE							= 1 << 2,
		FLAG_FOOTSTEPS						= 1 << 3,
		FLAG_LOOP							= 1 << 4,
		FLAG_SYSTEM_SOUND					= 1 << 5,
		FLAG_RANDOM_FREQUENCY_SHIFT			= 1 << 6,
		FLAG_UNKBIT7						= 1 << 7,
		FLAG_UNKBIT8						= 1 << 8,
		FLAG_UNKBIT9						= 1 << 9,
		FLAG_UNKBIT10						= 1 << 10,
		FLAG_IS_MUSIC						= 1 << 11,
		FLAG_REGION_MUTE_WHEN_SUBMERGED		= 1 << 12,
		FLAG_MAYBE_UNDERWATER				= 1 << 13,
		FLAG_UNKBIT14						= 1 << 14,
		FLAG_CACHED							= 1 << 15,
		FLAG_DONT_CACHE						= 1 << 16,
		FLAG_UNKBIT17						= 1 << 17,
		FLAG_FIRS_TPERSON					= 1 << 18,
		FLAG_MODULATED						= 1 << 19,
		FLAG_IS_RADIO						= 1 << 20,
		FLAG_IGNORE_TIMESCALE				= 1 << 21,
		FLAG_UNKBIT22						= 1 << 22,
		FLAG_UNKBIT23						= 1 << 23,
		FLAG_UNKBIT24						= 1 << 24,
		FLAG_ENVELOP_EFAST					= 1 << 25,
		FLAG_ENVELOPE_SLOW					= 1 << 26,
		FLAG_2DRADIUS						= 1 << 27,
		FLAG_UNKBIT28						= 1 << 28,
		FLAG_UNKBIT29						= 1 << 29,
		FLAG_ANIMATION_DRIVEN				= 1 << 30,
		FLAG_BEAM_EMITTER					= 1 << 31,
	};

	enum Flags00C {
		Flag_MuteWhenSubmerged		= 1 << 0,
		Flag_StartAtRandomPosition	= 1 << 1,
	};

	enum StateFlags {
		STATE_UNK_0		= 1 << 0,
		STATE_UNK_1		= 1 << 1,
		STATE_UNK_2		= 1 << 2,
		STATE_UNK_3		= 1 << 3,
		STATE_STOPPED	= 1 << 4,
		STATE_UNK_5		= 1 << 5,
		STATE_UNK_6		= 1 << 6,
		STATE_UNK_7		= 1 << 7,
		STATE_UNK_8		= 1 << 8,
		STATE_UNK_9		= 1 << 9,
		STATE_MUTED		= 1 << 10,
		STATE_UNK_11	= 1 << 11,
		STATE_UNK_12	= 1 << 12,
		STATE_UNK_13	= 1 << 13,
		STATE_UNK_14	= 1 << 14,
		STATE_UNK_15	= 1 << 15,
		STATE_UNK_16	= 1 << 16,
		STATE_ISLOOPING = 1 << 17,
		STATE_UNK_18	= 1 << 18,
		STATE_UNK_19	= 1 << 19,
		STATE_UNK_20	= 1 << 20,
	};

	enum EnvironmentType { // Shifted by 1 compared to DSound ones
		ENVIRONMENT_NONE = 0,
		ENVIRONMENT_DEFAULT,
		ENVIRONMENT_GENERIC,
		ENVIRONMENT_PADDEDCELL,
		ENVIRONMENT_ROOM,
		ENVIRONMENT_BATHROOM,
		ENVIRONMENT_LIVINGROOM,
		ENVIRONMENT_STONEROOM,
		ENVIRONMENT_AUDITORIUM,
		ENVIRONMENT_CONCERTHALL,
		ENVIRONMENT_CAVE,
		ENVIRONMENT_ARENA,
		ENVIRONMENT_HANGAR,
		ENVIRONMENT_CARPETEDHALLWAY,
		ENVIRONMENT_HALLWAY,
		ENVIRONMENT_STONECORRIDOR,
		ENVIRONMENT_ALLEY,
		ENVIRONMENT_FOREST,
		ENVIRONMENT_CITY,
		ENVIRONMENT_MOUNTAINS,
		ENVIRONMENT_QUARRY,
		ENVIRONMENT_PLAIN,
		ENVIRONMENT_PARKINGLOT,
		ENVIRONMENT_SEWERPIPE,
		ENVIRONMENT_UNDERWATER,
		ENVIRONMENT_SMALLROOM,
		ENVIRONMENT_MEDIUMROOM,
		ENVIRONMENT_LARGEROOM,
		ENVIRONMENT_MEDIUMHALL,
		ENVIRONMENT_LARGEHALL,
		ENVIRONMENT_PLATE
	};

	UInt32				uiID;
	Bitfield32			uiAudioFlags;
	Bitfield32			uiFlags00C;
	Bitfield32			uiStateFlags;
	UInt32				uiDuration;
	UInt16				usStaticAttenuation;
	UInt16				usSystemAttenuation;
	UInt16				usDistanceAttenuation;
	UInt16				usFaderAttenuation;
	UInt16				unk020;
	UInt16				unk022;
	float				fVolume;
	float				flt028;
	float				flt02C;
	UInt32				unk030;
	UInt16				usSampleRate;
	char				cFilePath[254];
	TESSound*			pSourceSound;
	float				fFrequencyMod;
	float				fMaxAttenuationDist;
	float				fMinAttenuationDist;
	UInt32				uiSourceID;
	UInt32				uiAlternateID;
	UInt32				uiChunkSizeInBytes;
	float				fListenerDistance;
	EnvironmentType		eEnvironmentType;
	UInt8				cRandFrequencyShift;
	UInt8				byte158;
	UInt16				usSamplingFreq;
	UInt32				unk15C;
	UInt32				uiAudioDelayMS;
	void*				pCallback164;
	void*				pCallback168;
	UInt32				time16C;
	UInt32				time170;
	UInt32				unk174;
	UInt32				unk178;
	NiPoint3			kSomePosition17C;
	UInt32				uiPriority;
	SoundMessageList	kMessageList;

	inline const char* GetFilePath() const { return cFilePath; }
	const char* GetEnvironmentTypeName() const;

	inline bool Is2D() const { return uiAudioFlags.GetBit(FLAG_2D) && uiAudioFlags.GetBit(FLAG_2DRADIUS); }
	inline bool Is3D() const { return uiAudioFlags.GetBit(FLAG_3D); }
};

ASSERT_SIZE(BSGameSound, 0x198);