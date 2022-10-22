#include <Sound.h>
#include <NiObjects.h>
#include <Objects.h>


void Sound::PlayEDID(const char* soundEDID, UInt32 flags, TESObjectREFR* refr)
{
	Sound sound;
	ThisCall(0xAD7550, BSWin32Audio::GetSingleton(), &sound, soundEDID, flags);
	if (sound.soundKey != 0xFFFFFFFF)
	{
		sound.SetPos(refr->PosVector());
		sound.Play();
	}
}

void Sound::PlayFile(const char* filePath, UInt32 flags, TESObjectREFR* refr)
{
	NiNode* refrNode = refr->GetNiNode();
	if (!refrNode) return;
	Sound sound;
	ThisCall(0xAD7480, BSWin32Audio::GetSingleton(), &sound, filePath, flags, nullptr);
	if (sound.soundKey != 0xFFFFFFFF)
	{
		sound.SetPos(refrNode->m_worldTransform.pos);
		sound.SetNiNode(refrNode);
		sound.Play();
	}
}

void Sound::PlayTESSound(TESSound* gameSound, UInt32 flags, TESObjectREFR* refr)
{
	const char* filePath = gameSound->soundFile.path.m_data;
	if (!filePath) return;
	NiNode* refrNode = refr->GetNiNode();
	if (!refrNode) return;
	Sound sound;
	ThisCall(0xAD7480, BSWin32Audio::GetSingleton(), &sound, filePath, flags, gameSound);
	if (sound.soundKey != 0xFFFFFFFF)
	{
		sound.SetPos(refrNode->m_worldTransform.pos);
		sound.SetNiNode(refrNode);
		sound.Play();
	}
}

void PlayGameSound(const char* soundPath)
{
	Sound sound(soundPath, 0x121);
	sound.Play();
}

namespace Radio
{
	TESObjectREFR* GetCurrentStation()
	{
		TESObjectREFR* station = nullptr;
		if (RadioEntry::GetSingleton())
		{
			station = RadioEntry::GetSingleton()->radioRef;
		}

		return station;
	}
	
	void GetNearbyStations(TList<TESObjectREFR>* dst)
	{
		CdeclCall(0x4FF1A0, PlayerCharacter::GetSingleton(), dst, nullptr);
	}

	TList<TESObjectREFR>* GetFoundStations() { return reinterpret_cast<TList<TESObjectREFR>*>(0x11DD59C); };
	bool GetEnabled() { return *reinterpret_cast<UInt8*>(0x11DD434); }
	void SetActiveStation(TESObjectREFR* station)
	{
		if (GetEnabled())
		{
			// stops the current playing dialogue line
			SetEnabled(false);
		}

		SetEnabled(true);

		SetStation(station, true);
	}
}

void Sound::Set3D(Actor* actor)
{
	if (!actor)
		return;
	auto* pos = actor->GetPos();
	auto* node = actor->GetNiNode();
	if (pos && node)
	{
		ThisStdCall(0xAD8B60, this, pos->x, pos->y, pos->z);
		ThisStdCall(0xAD8F20, this, node);
	}
}