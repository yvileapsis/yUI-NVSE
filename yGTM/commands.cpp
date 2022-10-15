#include <CommandTable.h>
#include <GameAPI.h>
#include <GameSound.h>

#include "GameObjects.h"
#include "GameProcess.h"
#include "PluginAPI.h"

void __fastcall DoFireWeaponEx(TESObjectREFR* refr, int EDX, TESObjectWEAP* weapon, TESAmmo* ammo, NiNode* node)
{
	const auto highProcess = static_cast<HighProcess*>(static_cast<Actor*>(refr)->baseProcess);

	TESObjectWEAP* oldWeapon = nullptr;
	if (weapon && highProcess->weaponInfo)
	{
		oldWeapon = highProcess->weaponInfo->weapon;
		highProcess->weaponInfo->weapon = weapon;
	}

	TESAmmo* oldAmmo = nullptr;
	if (ammo && highProcess->ammoInfo)
	{
		oldAmmo = highProcess->ammoInfo->ammo;
		highProcess->ammoInfo->ammo = ammo;
	}

	NiNode* oldNode = nullptr;
	if (node) {
		oldNode = highProcess->projectileNode;
		highProcess->projectileNode = node;
	}

	ThisCall<void>(0x523150, weapon, refr);

	if (oldNode) highProcess->projectileNode = oldNode;
	if (oldWeapon) highProcess->weaponInfo->weapon = oldWeapon;
	if (oldAmmo) highProcess->ammoInfo->ammo = oldAmmo;
}

extern inline bool Cmd_FireWeaponAlt_Execute(COMMAND_ARGS)
{
	TESObjectWEAP* weapon = nullptr;
	TESAmmo* ammo = nullptr;
	UInt32 mods = 0;
	Float32 condition = 0;
	char nodeName[0x40] = "\0";
	if (!(thisObj->IsActor() && ExtractArgsEx(EXTRACT_ARGS_EX, &weapon, &nodeName, &ammo, &mods, &condition) && weapon->typeID == kFormType_TESObjectWEAP && weapon->projectile)) return true;

	const auto node = nodeName[0] ? thisObj->GetNode(nodeName) : nullptr;

	if (CdeclCall<bool>(0x8C7AA0))
	{
		ScrapHeapQueue::QueuedCmdCall qCall(DoFireWeaponEx, thisObj->refID, 1);
		qCall.args[0] = weapon;
		qCall.args[1] = ammo;
		qCall.args[2] = node;
		ScrapHeapQueue::GetSingleton()->AddQueuedCmdCall(qCall);
	}
	else DoFireWeaponEx(thisObj, 0, weapon, ammo, node);

	return true;
}