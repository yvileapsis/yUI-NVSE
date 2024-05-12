#pragma once

#include "TESForm.hpp"

class TESPackage;

class TESAIForm : public BaseFormComponent {
public:
	TESAIForm();
	~TESAIForm();

	typedef BSSimpleList<TESPackage*> PackageList;

	virtual UInt32	GetSaveSize(UInt32 changedFlags);
	virtual void	Save(UInt32 changedFlags);
	virtual void	Load(UInt32 changedFlags);

	UInt8	agression;				// 04
	UInt8	confidence;				// 05
	UInt8	energyLevel;			// 06
	UInt8	responsibility;			// 07
	UInt8	mood;					// 08
	UInt8	pad09[3];				// 09

	UInt32	buySellsAndServices;	// 0C
	UInt8	teaches;				// 10
	UInt8	maximumTrainingLevel;	// 11
	UInt8	assistance;				// 12
	UInt8	aggroRadiusBehavior;	// 13
	SInt32	aggroRadius;			// 14

	PackageList	packageList;	// 18

	UInt32 GetPackageCount() const {
		return packageList.ItemsInList();
	}

	// return the nth package
	TESPackage* GetNthPackage(SInt32 anIndex) const {
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

ASSERT_SIZE(TESAIForm, 0x20);