#pragma once

class TESObjectREFR;

class MobIterOperator {
public:
	MobIterOperator();
	~MobIterOperator();

	virtual void Operate(TESObjectREFR* apRef) = 0;

	UInt32 eType;
};