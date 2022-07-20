#include <events.h>
#include <GameAPI.h>

#include <GameObjects.h>

void EventLayer()
{
	RegisterEvent("yJAM:JG:OnRender", 0, nullptr, kFlag_IsUserDefined);
	auto expression = CompileExpression(R"(SetOnRenderUpdateEventHandler 1 ({} => DispatchEventAlt "yJAM:JG:OnRender") 4)");
	if (!expression) Log("MISSING REQUIREMENTS - JG WAAAAAAAAAAAAA");
	CallFunctionAlt(expression, nullptr, 0);

	RegisterEvent("yJAM:JIP:OnHit", 0, nullptr, kFlag_IsUserDefined);
	expression = CompileExpression(R"(SetOnHitEventHandler ({} => (this).DispatchEventAlt "yJAM:JIP:OnHit") 1)");
	if (!expression) Log("MISSING REQUIREMENTS - JIP LN WAAAAAAAAAAAAA");
	CallFunctionAlt(expression, nullptr, 0);
}
