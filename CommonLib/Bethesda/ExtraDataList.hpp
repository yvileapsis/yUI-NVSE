#pragma once
#include "BSExtraData.hpp"

class BSExtraData;
class TESObjectREFR;

// 0x20
class BaseExtraList
{
public:
	BaseExtraList() { ThisCall(0x40F740, this); }; 
	virtual ~BaseExtraList() { ThisCall(0x40F780, this); };

	__forceinline UInt32		ItemsInList() { return ThisStdCall<UInt32>(0x40FE20, this); }
	__forceinline bool			HasType(BSExtraData::Type aeType) const { return ThisStdCall<bool>(0x40FE80, this, aeType); };
	__forceinline UInt32		SetHasType(BSExtraData::Type aeType, bool abCleared) { return ThisStdCall<UInt32>(0x40FEE0, this, aeType, abCleared); };
	__forceinline void			RemoveAll(bool abDoFree) { ThisStdCall(0x40FAE0, this, abDoFree); }
	__forceinline void			RemoveAllDefault(bool abDoFree) { ThisStdCall(0x040FCB0, this, abDoFree); }
	__forceinline BSExtraData*	AddExtra(BSExtraData* pkExtra) { return ThisStdCall<BSExtraData*>(0x40FF60, this, pkExtra); }
	__forceinline void			RemoveExtra(BSExtraData* pkExtra, bool abDoFree) { ThisStdCall(0x410020, this, pkExtra, abDoFree); }
	__forceinline void			RemoveExtra(BSExtraData::Type aeType) { ThisStdCall(0x410140, this, aeType); }
	// called GetExtraData but it's the only one where BSExtraData isn't shortened to Extra
	__forceinline BSExtraData*	GetExtra(BSExtraData::Type aeType) const { return ThisStdCall<BSExtraData*>(0x410220, this, aeType); }
	__forceinline BSExtraData*	GetPrevExtra(BSExtraData::Type aeType) const { return ThisStdCall<BSExtraData*>(0x4102D0, this, aeType); }

	BSExtraData*	pkData;						// 004
	UInt8			ucPresenceBitfield[0x15];	// 008 - if a bit is set, then the extralist should contain that extradata
	// bits are numbered starting from the lsb


	enum JIPExtraListFlags : UInt8
	{
		kJIPExtraListFlag_Update3D				= 1 << 0,
		kJIPExtraListFlag_DisableCollision		= 1 << 1,
		kJIPExtraListFlag_AltRefName			= 1 << 2,
		kJIPExtraListFlag_RefrModelPath			= 1 << 3,
		kJIPExtraListFlag_RefNameOrModel		= kJIPExtraListFlag_AltRefName | kJIPExtraListFlag_RefrModelPath,
		kJIPExtraListFlag_NoZPosReset			= 1 << 4,
	};

	JIPExtraListFlags eJIPFlags;
	UInt16 usJIPPerksInfo;
};
static_assert(sizeof(BaseExtraList) == 0x20);

// 0x20
class ExtraDataList : public BaseExtraList
{
public:
	ExtraDataList() : BaseExtraList() { ThisCall(0x410360, this); }
	~ExtraDataList() override { ThisCall(0x410380, this); };

	__forceinline void CopyExtra(BSExtraData* apkExtra) { ThisStdCall(0x4103D0, this, apkExtra); }
	__forceinline void CopyList(ExtraDataList* apkExtraDataList) { ThisStdCall(0x411EC0, this, apkExtraDataList); }
	__forceinline void RemoveAllCopyableExtra() { ThisStdCall(0x411FD0, this); }
	__forceinline void RemoveNonPersistentCellData() { ThisStdCall(0x4120B0, this); }
	__forceinline void CopyListForContainer(ExtraDataList* apkExtraDataList, char abDoNotRemove) { ThisStdCall(0x4121E0, this, apkExtraDataList, abDoNotRemove); }
	__forceinline void DuplicateExtraListForContainer(ExtraDataList* apkExtraDataList) { ThisStdCall(0x412380, this, apkExtraDataList); }
	__forceinline void CopyListForReference(ExtraDataList* apkExtraDataList, char abRemove) { ThisStdCall(0x412490, this, apkExtraDataList, abRemove); }
	__forceinline bool CompareListForContainer(ExtraDataList* apkExtraDataList, char abIsInMenu, char abIsNotGun) { ThisStdCall(0x4126C0, this, apkExtraDataList, abIsInMenu, abIsNotGun); }
	__forceinline void SaveForm() { ThisStdCall(0x412970, this); }
	__forceinline void LoadForm(void* unk01, void* unk02) { ThisStdCall(0x412970, this, unk01, unk02); }
	__forceinline void RemoveAllBrokenExtraForForm(TESForm* apkForm) { ThisStdCall(0x416BE0, this, apkForm); }

	static ExtraDataList* Create(BSExtraData* xBSData = NULL);

//	BSSimpleList<BSPointer<TESObjectREFR>*>* GetReflectedRefs(); // walltuh
	void* GetRefractionProperty();
};
static_assert(sizeof(ExtraDataList) == 0x20);