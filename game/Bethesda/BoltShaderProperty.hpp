#pragma once

#include "BSShaderProperty.hpp"

class BoltShaderProperty : public BSShaderProperty {
public:
	DWORD dword60;
	float float64;
	float float68;
	float float6C;
	float float70;
	DWORD dword74;
	DWORD dword78;
	DWORD dword7C;
	DWORD dword80;
	float float84;
	float float88;
	BYTE gap8C[156];
	DWORD dword128;
	float float12C;
	float float130;
	float float134;
	float float138;
	float float13C;
	DWORD dword140;
	float float144;
	float float148;
	DWORD dword14C;
	float float150;
	NiColorA colorA;
	NiColorA colorB;
	BYTE byte174;
	BYTE byte175;
	BYTE byte176;
	BYTE byte177;
	DWORD dword178;
	float float17C;
	float float180;
	DWORD dword184;
	DWORD dword188;
	float float18C;
};

ASSERT_SIZE(BoltShaderProperty, 0x190)