#include <main.h>

#include <TESForm.h>
#include <Menu.h>
#include <SimpleINILibrary.h>

#include "functions.h"
#include "dinput8.h"
#include "RTTI.h"

namespace UserInterface::WeaponHweel
{
	bool		enable = false;

	Tile*		tileMain = nullptr;

	UInt32		key = 35;
	UInt32		button = 128;
	bool		toggle = false;

	UInt32		imod = 4;


	Float64		alpha = 0.0;
	Float64		alphaMult = 0.75;

	Float64		height = 800.0;
	Float64		width = 800.0;

	Float64		offsetHeight = 0.5;
	Float64		offsetWidth = 0.5;

	UInt32 color = 0;

	bool stats = true;
	bool sounds = true;


	UInt32 function9 = 1;
	UInt32 function10 = 2;
	UInt32 function11 = 3;

	bool drop = true;
	Float64 timemult = 0.25;
	Float64 radiusMin = 50;
	Float64 radiusMax = -1;
	bool rumble = true;
	bool autoFill = false;

	UInt32		font = 0;
	Float64		fontY = 0.0;

	void HandleINI()
	{
		const auto iniPath = GetCurPath() + yUI_INI;
		CSimpleIniA ini;
		ini.SetUnicode();


		if (ini.LoadFile(iniPath.c_str()) == SI_FILE) return;

		enable = ini.GetOrCreate("General", "bHitMarker", true, "; enable 'Hit Marker' feature. If required files are not found this will do nothing.");

		ini.SaveFile(iniPath.c_str(), false);
	}

	UInt8 selectedHotkey = 0;
	UInt8 offsetmax = 7;

	namespace Wheel
	{
		void Update()
		{
			const auto [entry, extra] = InventoryChanges::HotkeyGet(selectedHotkey);

			std::string name;

			if (entry)
			{
				name = entry->form->GetTheName();
				if (entry->GetWeaponMod()) name += "+";
			}

			/*
			if (const auto condition = snd->GetHealthPercentAlt(true); condition != -1)
			{
				fst->SetFloat("_Meter", 1);
				fst->SetFloat("_MeterValue", condition);
				fst->SetFloat("_MeterArrow", snd->form->TryGetREFRParent()->typeID == kFormType_TESObjectWEAP ? 0.75 : 0.5);
			}
			else fst->SetFloat("_Meter", 0);
			*/


			tileMain->SetString("_Text", name.c_str());
			tileMain->SetFloat("_Slice", selectedHotkey + 1);

			for (UInt8 i = 0; i < InventoryChanges::kHotkeyStewie; i++)
			{
				const auto image = tileMain->GetChild("SliceBox" + std::to_string(i + 1));
				if (const auto [item, itemExtra] = InventoryChanges::HotkeyGet(i); item)
				{
					TESIcon* icon;
					if (const auto bipedModel = DYNAMIC_CAST(item->form, TESForm, TESBipedModelForm)) icon = bipedModel->icon;
					else icon = DYNAMIC_CAST(item->form, TESForm, TESIcon);
					if (icon) image->SetString("_ImageFilename", icon->ddsPath.CStr());
					image->SetFloat("_Image", true);

					const auto extraAmmo = (ExtraAmmo*)itemExtra->GetByType(kExtraData_Ammo);
					if (extraAmmo && extraAmmo->ammo)
					{
						const auto totalAmmo = PlayerCharacter::GetSingleton()->GetItemCount(extraAmmo->ammo);
						const auto clipSize = min(item->GetClipSize(), extraAmmo->count);
						std::string ammoCount = std::to_string(clipSize) + "/" + std::to_string(totalAmmo - clipSize);
						image->SetString("_AmmoCount", ammoCount.c_str());
						image->SetString("_AmmoName", extraAmmo->ammo->abbreviation.CStr());
					}
					else
					{
						image->SetString("_AmmoCount", "");
						image->SetString("_AmmoName", "");
					}
				}
				else
				{
					image->SetFloat("_Image", false);
				}
			}
		}

		void Activate()
		{
			ToggleVanityWheel(false);
			g_HUDMainMenu->tileReticleCenter->SetFloat(kTileValue_visible, false);
			g_HUDMainMenu->tileInfo->SetFloat(kTileValue_visible, false);
			Update();
			tileMain->SetFloat("_Visible", true);
		}

		void Deactivate()
		{
			ToggleVanityWheel(true);
			g_HUDMainMenu->tileReticleCenter->SetFloat(kTileValue_visible, true);
			g_HUDMainMenu->tileInfo->SetFloat(kTileValue_visible, true);

			tileMain->SetFloat("_Visible", false);
		}

	}

	bool lock = false;

	UInt32		keyScrollUp = 264;
	UInt32		keyScrollDown = 265;
	bool		overScroll = false;


	namespace Scroll
	{
		void Update()
		{
			bool update = false;

			if (!IsKeyPressed(keyScrollDown)) {}
			else if (selectedHotkey < offsetmax) { update = true; selectedHotkey++; }
			else if (overScroll) { update = true; selectedHotkey = 0; }

			if (!IsKeyPressed(keyScrollUp)) {}
			else if (selectedHotkey > 0) { update = true; selectedHotkey--; }
			else if (overScroll) { update = true; selectedHotkey = offsetmax; }

			if (update)
			{
				PlayGameSound("UIPipBoyScroll");
				Wheel::Update();
			}
		}
	}

	bool lockTake = false;


	void Controls()
	{
		if (!IsKeyPressed(0x23, DIHookControl::kFlag_RawState))
		{
			if (lock == true)
			{
				Wheel::Deactivate();
			}
			lock = false;
		}
		else if (lock == false)
		{
			lock = true;
			Wheel::Activate();
		}

		if (lock == true)
		{
			Scroll::Update();

			if (!IsKeyPressed(GetControl(5, OSInputGlobals::kControlType_Keyboard), DIHookControl::kFlag_RawState))
			{
				if (lockTake == true)
				{
					//Wheel::Deactivate();
				}
				lockTake = false;
			}
			else if (lockTake == false)
			{
				lockTake = true;

				if (const auto [entry, extra] = InventoryChanges::HotkeyGet(selectedHotkey); entry) entry->Equip(PlayerCharacter::GetSingleton(), extra);
				//Wheel::Activate();
			}

		}
	}

	void MainLoop() {

		Controls();

	}

	void Reset()
	{
		HandleINI();

		if (!enable)
		{
			return;
		}

		mainLoop.push_back(MainLoop);

//		tileMain->SetFloat("_Color", systemcolor);

		tileMain->SetFloat("_AlphaBase", alpha);
		tileMain->SetFloat("_AlphaAC", HUDMainMenu::GetOpacity());
		tileMain->SetFloat("_AlphaMult", alphaMult);
		tileMain->SetFloat("_WidthBase", width);
		tileMain->SetFloat("_HeightBase", height);
		tileMain->SetFloat("_OffsetHorizontal", offsetHeight);
		tileMain->SetFloat("_OffsetVertical", offsetWidth);
		tileMain->SetFloat("_FontBase", font);
		tileMain->SetFloat("_FontYBase", fontY);
	}

	void MainLoopDoOnce()
	{
		tileMain = g_HUDMainMenu->tile->GetChild("JWH");
		if (!tileMain)
		{
			g_HUDMainMenu->tile->InjectUIXML(R"(Data\menus\yUI\WeaponHweel.xml)");
			tileMain = g_HUDMainMenu->tile->GetChild("JWH");
		}
		if (!tileMain) return;
		RegisterEvent("JWH:Reset", 0, nullptr, 4);
		SetEventHandler("JWH:Reset", Reset);
		Reset();
	}

	void Init()
	{
		if (g_nvseInterface->isEditor) return;
		mainLoopDoOnce.emplace_back(MainLoopDoOnce);
		HandleINI();
	}
}
