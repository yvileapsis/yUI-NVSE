#pragma once

class ScriptLocal {
public:
	UInt32 id;
	ScriptLocal* nextEntry;
	union
	{
		double data;
		UInt32 formId;
	};

	UInt32 GetFormId();
};