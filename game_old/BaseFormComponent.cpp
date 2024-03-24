#include <BaseFormComponent.h>


// static
UInt32 TESBipedModelForm::MaskForSlot(UInt32 slot)
{
	return 0x1 << slot;
}

UInt32 TESBipedModelForm::GetSlotsMask() const {
	return partMask;
}

void TESBipedModelForm::SetSlotsMask(UInt32 mask)
{
	partMask = (mask & ePartBitMask_Full);
}

UInt32 TESBipedModelForm::GetBipedMask() const {
	return bipedFlags & 0xFF;
}

void TESBipedModelForm::SetBipedMask(UInt32 mask)
{
	bipedFlags = mask & 0xFF;
}

void  TESBipedModelForm::SetPath(const char* newPath, UInt32 whichPath, bool bFemalePath)
{
	String* toSet = nullptr;

	switch (whichPath)
	{
	case ePath_Biped:
		toSet = &bipedModel[bFemalePath ? 1 : 0].nifPath;
		break;
	case ePath_Ground:
		toSet = &groundModel[bFemalePath ? 1 : 0].nifPath;
		break;
	case ePath_Icon:
		toSet = &icon[bFemalePath ? 1 : 0].ddsPath;
		break;
	default: break;
	}

	if (toSet) toSet->Set(newPath);
}

const char* TESBipedModelForm::GetPath(UInt32 whichPath, bool bFemalePath)
{
	String* pathStr = nullptr;

	switch (whichPath)
	{
	case ePath_Biped:
		pathStr = &bipedModel[bFemalePath ? 1 : 0].nifPath;
		break;
	case ePath_Ground:
		pathStr = &groundModel[bFemalePath ? 1 : 0].nifPath;
		break;
	case ePath_Icon:
		pathStr = &icon[bFemalePath ? 1 : 0].ddsPath;
		break;
	default: break;
	}

	return pathStr ? pathStr->m_data : "";
}

SInt8 TESActorBaseData::GetFactionRank(TESFaction* faction)
{
	for (const auto iter : factionList) if (iter->faction == faction) return iter->rank;
	return -1;
}

SInt32 TESContainer::GetCountForForm(TESForm* form)
{
	SInt32 result = 0;
	for (const auto iter : formCountList)
		if (iter->form == form) result += iter->count;
	return result;
}