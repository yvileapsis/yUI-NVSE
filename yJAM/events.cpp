#include <events.h>
#include <GameAPI.h>

#include <GameObjects.h>

void EventLayer()
{
	RegisterEvent("yJAM:JIP:OnHit", 0, nullptr, kFlag_IsUserDefined);
	CallFunctionAlt(CompileExpression(R"(SetOnHitEventHandler ({} => (this).DispatchEventAlt "yJAM:JIP:OnHit") 1)"), nullptr, 0);
}
