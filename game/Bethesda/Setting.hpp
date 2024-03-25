#pragma once

// 0xC
class Setting {
public:
	Setting();
	virtual ~Setting();
	virtual bool IsPrefSetting();

	enum EnumType {
		kSetting_Bool = 0,
		kSetting_c,
		kSetting_h,
		kSetting_Integer,
		kSetting_Unsigned,
		kSetting_Float,
		kSetting_String,
		kSetting_r,
		kSetting_a,
		kSetting_Other
	};

	union Info {
		const char*		str;
		SInt32			i;
		UInt32			u;
		float			f;
		bool			b;
		char			c;
		char			h;
	};

	Info		uValue;
	const char* pKey;

	UInt32		GetType();
	bool		Get(double& out);
	bool		Get(const char* str);
	const char* Get();
	bool		Set(double newVal);
	bool		Set(const char* str);
};
static_assert(sizeof(Setting) == 0xC);