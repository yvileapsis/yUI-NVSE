#pragma once

class hkStringPtr {
public:
	const char* m_stringAndFlag;

	const char* c_str() const {
		return reinterpret_cast<char*>(UInt32(m_stringAndFlag) & ~1);
	}

	operator const char*() const {
		return c_str();
	}
};