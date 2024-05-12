#pragma once

// 0xC
class Setting {
public:
	Setting();
	virtual ~Setting();
	virtual bool IsPrefSetting();

	enum EnumType {
		kSetting_Bool = 0,
		kSetting_Char,
		kSetting_Hex,
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

	UInt32		GetType() const;
	bool		Get(double& out) const;
	bool		Get(const char* str) const;
	const char* Get() const;
	bool		Set(double newVal);
	bool		Set(const char* str);

	std::string GetAsString() const;
	Float32 GetAsFloat() const;
};
static_assert(sizeof(Setting) == 0xC);