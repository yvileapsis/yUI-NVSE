#include <main.h>
#include <functions.h>

namespace UserInterface::EventLayer
{
	void OnRender() { for (const auto& i : onRender) i(); }
	void OnHit(Actor* target, void* args) { for (const auto& i : onHit) i(target); }

	struct ReferenceForm
	{
		TESObjectREFR* ref;
		TESForm* form;
	};

	struct ActorBool
	{
		Actor* actor;
		bool isActivationNotPrevented;
	};

	void OnAddDrop(TESObjectREFR* thisObj, void* parameters)
	{
		const auto ptr = static_cast<ReferenceForm*>(parameters);
		for (const auto i : onAddDrop) i(thisObj, ptr->ref, ptr->form);
	}

	void OnPreActivate(TESObjectREFR* thisObj, void* parameters)
	{
		const auto ptr = static_cast<ActorBool*>(parameters);
		bool preventActivate = false;
		for (const auto i : onPreActivate) if (!i(thisObj, ptr->actor, ptr->isActivationNotPrevented)) preventActivate = true;
		SetNativeHandlerFunctionBool(!preventActivate);
	}

	void MainLoopDoOnce()
	{
		RegisterEvent("yUI:JG:OnRender", 0, nullptr, NVSEEventManagerInterface::kFlag_IsUserDefined);
		auto expression = CompileExpression(R"(SetOnRenderUpdateEventHandler 1 ({} => DispatchEventAlt "yUI:JG:OnRender") 4)");
		if (!expression) Log("Missing JohnnyGuitar NVSE", 3);
		CallFunctionAlt(expression, nullptr, 0);
		SetEventHandler("yUI:JG:OnRender", OnRender);

		RegisterEvent("yUI:JIP:OnHit", 0, nullptr, NVSEEventManagerInterface::kFlag_IsUserDefined);
		expression = CompileExpression(R"(SetOnHitEventHandler ({} => (this).DispatchEventAlt "yUI:JIP:OnHit") 1)");
		if (!expression) Log("Missing JIP LN NVSE", 3);
		CallFunctionAlt(expression, nullptr, 0);
		SetEventHandler("yUI:JIP:OnHit", OnHit);

		SetEventHandler("OnAdd", OnAddDrop);
		SetEventHandler("OnDrop", OnAddDrop);

		SetEventHandler("ShowOff:OnPreActivate", OnPreActivate);
	}

	void Init()
	{
		mainLoopDoOnce.emplace_back(MainLoopDoOnce);
	}
}
