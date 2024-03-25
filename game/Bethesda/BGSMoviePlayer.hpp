#pragma once

class BGSMoviePlayer
{
	virtual void Func0000();
	virtual void MuteAudio();
	virtual void UnMuteMusic();
	virtual void PauseMusic();
	virtual void UnPauseMusic();
	virtual void Func0005();
	virtual void Func0006();
	virtual void Func0007();
	virtual void Func0008();
	virtual void Func0009();
	virtual void Func000A();
	virtual void Func000B();
	virtual void Func000C();
	virtual void Func000D();
	virtual void Func000E();
	virtual void Func000F();
	virtual void ShouldExit();

	struct Unk0C
	{
		BinkStruct *binkStruct;
		float unk04;
		UInt32 ptr08;
		float unk0C;
		float unk10;
		float unk14;
		float unk18;
		float unk1C;
		UInt32 unk20;
		UInt8 byte24;
		UInt8 gap25[3];
	};

	UInt32 unk04;
	HANDLE *thread;
	BGSMoviePlayer::Unk0C *ptr0C;
	BGSMoviePlayer::Unk0C *ptr10;
	UInt32 ptr14;
	UInt8 byte18;
	UInt8 byte19;
	UInt8 gap1A[2];
	UInt32 unk1C;
	UInt8 byte20;
	UInt8 byte21;
	UInt8 byte22;
	UInt8 byte23;
	UInt8 byte24;
	UInt8 gap25[3];
	UInt32 unk28;
	UInt32 unk2C;
	Semaphore semaphor;
	UInt8 byte3C;
	UInt8 gap3D[3];
	NiTMapBaseIntInt nitMap;
	UInt32 unk50;
};