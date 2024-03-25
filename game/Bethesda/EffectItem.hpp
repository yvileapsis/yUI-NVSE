#pragma once

#include "BSString.hpp"
#include "BSSimpleList.hpp"
#include "TESCondition.hpp"
#include "EffectSetting.hpp"

// 0x24
class EffectItem {
public:
	EffectItem();
	~EffectItem();

	enum
	{
		kRange_Self = 0,
		kRange_Touch,
		kRange_Target,
	};

	struct ScriptEffectInfo
	{
		UInt32		uiScriptRefID;
		UInt32		uiSchool;
		BSStringT	kEffectName;
		UInt32		uiVisualEffectCode;
		UInt32		uiIsHostile;

		void SetName(const char* name);
		void SetSchool(UInt32 school);
		void SetVisualEffectCode(UInt32 code);
		void SetIsHostile(bool bIsHostile);
		bool IsHostile() const;
		void SetScriptRefID(UInt32 refID);

		ScriptEffectInfo* Clone() const;
		void CopyFrom(const ScriptEffectInfo* from);
		static ScriptEffectInfo* Create();
	};

	// mising flags
	UInt32			uiMagnitude;			// 00	used as a float
	UInt32			uiArea;				// 09
	UInt32			uiDuration;			// 08
	UInt32			uiRange;				// 0C
	UInt32			uiActorValueOrOther;	// 10
	EffectSetting*	pSetting;	// 14
	Float32			fCost;				// 18 on autocalc items this seems to be the cost
	TESCondition	conditions;			// 1C

	//bool HasActorValue() const;
	//UInt32 GetActorValue() const;
	//bool IsValidActorValue(UInt32 actorValue) const;
	//void SetActorValue(UInt32 actorValue);

	//bool IsScriptedEffect() const;
	//UInt32 ScriptEffectRefId() const;
	//UInt32 ScriptEffectSchool() const;
	//UInt32 ScriptEffectVisualEffectCode() const;
	//bool IsScriptEffectHostile() const;

	//EffectItem* Clone() const;
	//void CopyFrom(const EffectItem* from);
	//static EffectItem* Create();
	//static EffectItem* ProxyEffectItemFor(UInt32 effectCode);
	//
	//bool operator<(EffectItem*rhs) const;
	//// return the magicka cost of this effect item
	//// adjust for skill level if actorCasting is used
	//float MagickaCost(TESForm* actorCasting = NULL) const;

	//void SetMagnitude(UInt32 magnitude);
	//void ModMagnitude(float modBy);
	//void SetArea(UInt32 area);
	//void ModArea(float modBy);
	//void SetDuration(UInt32 duration);
	//void ModDuration(float modBy);
	//void SetRange(UInt32 range);
	//bool IsHostile() const;
	__forceinline int	GetSkillCode() { return ThisCall<signed int>(0x403EA0, this); };
};
static_assert(sizeof(EffectItem) == 0x24);

// 0x10
class EffectItemList : public BSSimpleList<EffectItem*> {
public:
	EffectItemList();
	~EffectItemList();

	UInt32	unk00C;	// 00C

	virtual void Unk_00();
	virtual void Unk_01();
	virtual void CopyFromBase();
	virtual void Unk_03();
	virtual void Unk_04(); // ret5

	EffectItem* ItemAt(UInt32 whichItem);
	UInt32		CountItems() const;
	const char* GetNthEIName(UInt32 whichEffect) const;

	//	bool HasNonHostileItem() const;
	//	UInt32 CountHostileItems() const;
	//	UInt32 AddItem(EffectItem* effectItem);
	//	UInt32 AddItemCopy(EffectItem* effectItem);
	//	UInt32 CopyItemFrom(EffectItemList& fromList, UInt32 whichItem);
	//	bool RemoveItem(UInt32 whichItem);
};
static_assert(sizeof(EffectItemList) == 0x10);