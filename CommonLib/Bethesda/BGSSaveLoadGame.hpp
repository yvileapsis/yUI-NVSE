#pragma once

#include "NiTMap.hpp"
#include "NiTPointerMap.hpp"
#include "BSSimpleArray.hpp"

class BGSCellNumericIDArrayMap;
class BGSLoadGameSubBuffer;
class BGSReconstructFormsInFileMap;
class BGSReconstructFormsInAllFilesMap;
class BGSLoadFormBuffer;
class BGSSaveLoadChangesMap;
class BGSSaveLoadReferencesMap;
class Actor;
class TESObjectCELL;
class TESForm;

// 0x24C
class BGSSaveLoadGame {
public:
	BGSSaveLoadGame();
	~BGSSaveLoadGame();

	typedef UInt32 RefID;
	typedef UInt32 IndexRefID;
	struct RefIDIndexMapping // reversible map between refID and loaded form index
	{
		NiTMap<RefID, IndexRefID>* refIDToIndex; // 000
		NiTMap<IndexRefID, RefID>* indexToRefID; // 010
		UInt32 countRefID;				   // 020
	};

	struct SaveChapters // 06E	chapter table in save
	{
		struct RefIDArray // List of all refID referenced in save for tranlation in RefIDIndexMapping
		{
			UInt32 count; // 000
			RefID IDs[1]; // 004
		};

		RefIDArray* arr000; // 000
		RefIDArray* arr004; // 004
	};

	struct Struct010
	{
		NiTPointerMap<UInt32, UInt32>* map000;					   // 000
		BGSCellNumericIDArrayMap* map010;				   // 010
		NiTPointerMap<BGSCellNumericIDArrayMap*, UInt32>* map020; // 020
	};

	BGSSaveLoadChangesMap*					pChangesMap;
	BGSSaveLoadChangesMap*					previousChangeMap;
	RefIDIndexMapping*						refIDmapping;
	RefIDIndexMapping*						visitedWorldspaces;
	BGSSaveLoadReferencesMap*				referencesMap;
	NiTMap<TESForm*, BGSLoadGameSubBuffer>* maps014;
	NiTMap<UInt32, UInt32>*					pkChangedFormIDMap;
	char** History;
	BGSReconstructFormsInAllFilesMap*		rfiafMap;
	BSSimpleArray<BGSLoadFormBuffer*>		kChangedForms;
	NiTMap<UInt32, Actor*>					kQueuedInitPackageLocationsActorMap;
	UInt8		ucSaveMods[255];
	UInt8		ucLoadedMods[255];
	UInt16		pad242;
	UInt32		uiGlobalFlags;
	UInt8		ucCurrentMinorVersion;
	UInt8		pad249[3];

	static BGSSaveLoadGame* GetSingleton();

	bool GetGlobalAllowChanges() const;
	bool GetSaveGameLoading() const;
	bool GetSaveGameSaving() const;
	bool GetInitingForms() const;
	bool GetDeferInitForms() const;
	bool GetPositioningPlayerCharacter() const;
	bool GetPlayerLocationInvalid() const;
	bool GetSaveLoadFailed() const;

	bool GetThreadAllowChanges() const;
	[[nodiscard("Previous value")]] bool SetThreadAllowChanges(bool abEnable);

	bool GetLoadingMovedRefs() const;
	bool GetReconstructingForms() const;

	bool GetAllowChanges() const;

};
static_assert(sizeof(BGSSaveLoadGame) == 0x24C);