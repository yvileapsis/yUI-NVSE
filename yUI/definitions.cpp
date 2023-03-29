#include <definitions.h>

#define INIT_MODULE(mod) namespace mod { extern void Init(); }

INIT_MODULE(ConfigurationMenu)

INIT_MODULE(SortingIcons)

INIT_MODULE(UserInterface::EventLayer)
INIT_MODULE(UserInterface::DynamicCrosshair)
INIT_MODULE(UserInterface::HitMarker)
INIT_MODULE(UserInterface::HitIndicator)
INIT_MODULE(UserInterface::VisualObjectives)
INIT_MODULE(UserInterface::LootMenu)
INIT_MODULE(UserInterface::WeaponHweel)

INIT_MODULE(Fix::DroppedItems)
INIT_MODULE(Fix::TablineSelected)
INIT_MODULE(Fix::ReorderMCM)
INIT_MODULE(Fix::ProjectileEffects)
INIT_MODULE(Fix::TouchpadScrolling)
INIT_MODULE(Fix::LocaleStrings)

INIT_MODULE(Patch::RestoreFO3Spread)
INIT_MODULE(Patch::ArmedUnarmed)
INIT_MODULE(Patch::ExplosionForce)
INIT_MODULE(Patch::MatchedCursor)
INIT_MODULE(Patch::MultiplicativeShots)
INIT_MODULE(Patch::TimeMult)
INIT_MODULE(Patch::CharGenMenuSRemoval)
INIT_MODULE(Patch::B42InjectHideCrosshair)

void Inits()
{
//	ConfigurationMenu::Init();
	SortingIcons::Init();

	UserInterface::EventLayer::Init();
	UserInterface::DynamicCrosshair::Init();
	UserInterface::HitMarker::Init();
	UserInterface::HitIndicator::Init();
	UserInterface::VisualObjectives::Init();
	UserInterface::LootMenu::Init();
//	UserInterface::WeaponHweel::Init();

	Fix::DroppedItems::Init();
	Fix::TablineSelected::Init();
//	Fix::ReorderMCM::Init();
//	Fix::ProjectileEffects::Init();
	Fix::TouchpadScrolling::Init();
	Fix::LocaleStrings::Init();

//	Patch::RestoreFO3Spread::Init();
//	Patch::ArmedUnarmed::Init();
	Patch::ExplosionForce::Init();
	Patch::MatchedCursor::Init();
//	Patch::MultiplicativeShots::Init();
	Patch::TimeMult::Init();
	Patch::CharGenMenuSRemoval::Init();
	Patch::B42InjectHideCrosshair::Init();
}