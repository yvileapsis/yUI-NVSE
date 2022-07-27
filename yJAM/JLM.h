#pragma once

inline UInt32	g_JLM					= 1;

namespace JLM
{
	
	inline UInt32	g_Key1				= 18;
	inline UInt32	g_Key2				= 19;
	inline UInt32	g_Key3				= 20;
	inline UInt32	g_KeyAlt			= 42;
	inline UInt32	g_KeyScrollUp		= 264;
	inline UInt32	g_KeyScrollDown		= 265;

	inline UInt32	g_Button1			= 0x1000;
	inline UInt32	g_Button2			= 0x4000;
	inline UInt32	g_Button3			= 0;
	inline UInt32	g_ButtonAlt			= 0x2;

	inline UInt32	g_TakeModeSimple	= 0x0;
	inline UInt32	g_TakeModeStack		= 0x0;
	inline UInt32	g_TakeModeSmart		= 0x1;
	inline UInt32	g_TakeModeEquip		= 0x2;
	inline UInt32	g_TakeModeOpen		= 0x3;
	inline UInt32	g_TakeModeAll		= 0x4;

	inline UInt32	g_TakeSmartMin		= 5;
	inline UInt32	g_TakeWeightless	= 1;

	inline UInt32	g_Block				= 0;
	inline UInt32	g_LegacyMode		= 0;
	inline UInt32	g_HidePrompt		= 1;
	inline UInt32	g_HideName			= 0;
	inline UInt32	g_ItemsMax			= 5;

	inline UInt32	g_Justify			= 3;
	inline Float64	g_HeightMin			= 32.0;
	inline Float64	g_HeightMax			= 640.0;
	inline Float64	g_WidthMin			= 400.0;
	inline Float64	g_WidthMax			= 640.0;
	inline Float64	g_OffsetX			= 0.6;
	inline Float64	g_OffsetY			= 0.5;

	inline Float64	g_IndentItem		= 8.0;
	inline Float64	g_IndentTextX		= 10.0;
	inline Float64	g_IndentTextY		= 10.0;

	inline UInt32	g_WeightVisible		= 2;
	inline UInt32	g_WeightAltColor	= 1;
	inline UInt32	g_Font				= 0;
	inline UInt32	g_FontHead			= 0;
	inline Float64	g_FontY				= 0;
	inline Float64	g_FontHeadY			= 0;

	inline UInt32	g_Sounds			= 1;
	inline UInt32	g_ShowEquip			= 1;
	inline UInt32	g_ShowIcon			= 1;
	inline UInt32	g_ShowMeter			= 1;

	void Initialize();
	void Reset();
	void OnRender();
	void MainLoop();
	void OnAddDropUpdate(TESObjectREFR* thisObj, void* parameters);
	void OnPreActivate(TESObjectREFR* thisObj, void* parameters);
}