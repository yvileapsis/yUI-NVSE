#pragma once
#include "BaseFormComponent.hpp"
#include "BSSimpleList.hpp"

class TESPackage;

// 0x20
class TESAIForm : public BaseFormComponent
{
public:
	TESAIForm();
	~TESAIForm();

	virtual UInt32	GetSaveSize(UInt32 changedFlags);
	virtual void	Save(UInt32 changedFlags);
	virtual void	Load(UInt32 changedFlags);

	UInt8	ucAgression;				// 04
	UInt8	ucConfidence;				// 05
	UInt8	ucEnergyLevel;				// 06
	UInt8	ucResponsibility;			// 07
	UInt8	ucMood;						// 08
	UInt8	pad09[3];					// 09

	UInt32	uiBuySellsAndServices;		// 0C
	UInt8	ucTeaches;					// 10
	UInt8	ucMaximumTrainingLevel;		// 11
	UInt8	ucAssistance;				// 12
	UInt8	ucAggroRadiusBehavior;		// 13
	SInt32	iAggroRadius;				// 14

	BSSimpleList<TESPackage*>	packageList;	// 18

	UInt32	GetPackageCount() const {
		return packageList.ItemsInList();
	}

	// return the nth package
	TESPackage* GetNthPackage(SInt32 anIndex) const
	{
		auto entry = packageList.GetAt(anIndex);
		return entry ? entry->GetItem() : nullptr;
	}

	//// replace the nth package
	//TESPackage* SetNthPackage(TESPackage* pPackage, SInt32 anIndex)
	//{
	//	return packageList.ReplaceNth(anIndex == -1 ? eListEnd : anIndex, pPackage);
	//}

	//// return the nth package
	//SInt32 AddPackageAt(TESPackage* pPackage, SInt32 anIndex)
	//{
	//	return packageList.AddAt(pPackage, anIndex == -1 ? eListEnd : anIndex);
	//}

	//TESPackage* RemovePackageAt(SInt32 anIndex)
	//{
	//	return packageList.RemoveNth(anIndex == -1 ? eListEnd : anIndex);
	//}

	//// removes all packages and returns how many were removed
	//UInt32 RemoveAllPackages() const
	//{
	//	UInt32 cCount = GetPackageCount();
	//	packageList.RemoveAll();
	//	return cCount - GetPackageCount();
	//}
};
static_assert(sizeof(TESAIForm) == 0x20);