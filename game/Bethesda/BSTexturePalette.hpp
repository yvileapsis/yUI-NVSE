#pragma once

#include "BSFileEntry.hpp"
#include "NiSourceTexture.hpp"
#include "BSSimpleList.hpp"
#include "BSSpinLock.hpp"

// Inheriting Gamebryo's palette? - ../../../NiMain/BSTexturePalette.cpp - PS3 FO3

class BSTexturePalette {
public:
	enum TextureLevel : UInt16 {
		LEVEL_0 = 0, // 64x64
		LEVEL_1 = 1, // 128x128
		LEVEL_2 = 2, // 256x256
		LEVEL_3 = 3, // 512x512
		LEVEL_4 = 4, // 1024x1024
		LEVEL_5 = 5, // 2048x2048
		LEVEL_6 = 6, // 4096x4096
		LEVEL_COUNT
	};

	class TexContainer : public BSMemObject {
	public:
		TexContainer() : spTexture(nullptr), pFileEntry(nullptr), pNext(nullptr), usLevel(LEVEL_COUNT), usPathOffset(0) {}
		~TexContainer() {};

		NiTexturePtr	spTexture;
		BSFileEntry*	pFileEntry;
		TexContainer*	pNext;
		TextureLevel	usLevel;
		UInt16			usPathOffset;

		NiTexture* GetTexture() const { return spTexture; }
		BSFileEntry * GetFileEntry() const { return pFileEntry; }
		TexContainer* GetNext() const { return pNext; }
		TextureLevel GetLevel() const { return usLevel; }
		UInt16 GetPathOffset() const { return usPathOffset; }
		const char* GetFilePath() const { 
			if (!spTexture.m_pObject)
				return "NULL TEXTURE";
			const char* pName = spTexture->GetPath()->m_kHandle;
			return pName ? pName : "GENERATED TEXTURE";
		}
		const char* GetOffsetFilePath() const { return &GetFilePath()[usPathOffset]; }
	};


	struct FileEntryMap {
		BSSimpleList<TexContainer*>* pContainerLists[1001];
		UInt32						 uiCount;

		static FileEntryMap* GetSingleton() { return *(FileEntryMap**)0x11F4468; }

		void GetTexture(const UInt32 auiContainer, BSFileEntry* apFileEntry, NiTexturePtr& arTextureOut) const;
		void GetTextureFromEntry(BSFileEntry* apFileEntry, NiTexturePtr& arTextureOut) const;
		void AddTextureContainer(BSFileEntry* apFileEntry, TexContainer* apContainer);
		void RemoveTextureContainer(BSFileEntry* apFileEntry);
	};

	class RemovalEntry {
	public:
		TexContainer*	pContainer;
		UInt32			uiContainerNumber;
		UInt32			uiLevel;

		bool GetTexture(NiTexturePtr& arTextureOut, bool abForceRetry);
	};


	struct PathMap {
		struct TextureGroup {
			TexContainer*	pContainers[713];
			UInt32			uiCount;

			void RemoveContainer(TexContainer* apContainer);
			// Destructor?
			static void __fastcall FreeTextures(TextureGroup* apThis);

			TexContainer* AddTexture(NiTexture* apTexture);
			bool GetTexture(UInt32 auiContainer, const char* apPath, NiTexturePtr& apTextureOut) const;
			bool GetRemovalData(BSTexturePalette::RemovalEntry& arEntry, NiTexturePtr& arTexture) const;
			bool GetLastContainer(BSTexturePalette::RemovalEntry& arEntry) const;
		};

		TextureGroup kGroups[LEVEL_COUNT];
		UInt32		 uiCount;

		static PathMap* GetSingleton() { return *(PathMap**)0x11F4464; }
	};

	static UInt32 uiCacheHits;
	static BSSpinLock* const ms_kTextureListCriticalSection;
	static void Lock() { ms_kTextureListCriticalSection->Lock(); }
	static void Unlock() { ms_kTextureListCriticalSection->Unlock(); }

	static bool __cdecl ArePathsEqual(const char* apRequestedPath, const char* apPath);

	static TextureLevel GetTextureLevel(NiTexture* apTexture);
	static UInt32 GetEntryIndex(BSHash* apHash);
	static UInt32 GetContainerNumber(const char* apPath);
	static TexContainer* __cdecl GetContainer(NiTexture* apTexture);

	static bool __cdecl GetTexture(const char* apPath, NiTexturePtr& arTextureOut);
	static void __cdecl GetTextureFromEntry(BSFileEntry* apFileEntry, NiTexturePtr& arTextureOut);

	static void __cdecl AddTexture(NiTexture* apTexture, BSFileEntry* apFileEntry);

	static bool __cdecl RemoveTexture(NiTexture* apTexture);
	static bool __cdecl RemoveTextureByPath(const char* apPath);
	static bool __cdecl CreateRemovalEntry(RemovalEntry& arEntry);
	static void __stdcall EmergencyTextureRelease();
};