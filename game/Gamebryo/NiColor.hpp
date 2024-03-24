#pragma once

#include "NiMemObject.hpp"
#include "NiPoint3.hpp"

class NiColor {
public:
	float r;
	float g;
	float b;

	inline const float operator[] (UInt32 i) const { return ((float*)&r)[i]; };
	inline float operator[] (UInt32 i) { return ((float*)&r)[i]; };

	operator float* () const { return (float*)this; };

	NiColor() : r(0.f), g(0.f), b(0.f) {};
	NiColor(float r, float g, float b) : r(r), g(g), b(b) {};
	NiColor(float r, float g, float b, float a) : r(r), g(g), b(b) {};
	NiColor(float f) : r(f), g(f), b(f) {};

	NiColor operator*(const float f) const {
		return NiColor(r * f, g * f, b * f);
	};

	NiColor operator*(const NiColor& color) const {
		return NiColor(r * color.r, g * color.g, b * color.b);
	};

	bool operator==(const NiColor& color) const {
		return r == color.r && g == color.g && b == color.b;
	};

	bool operator==(const float f) const {
		return r == f && g == f && b == f;
	};

	bool operator!=(const NiColor& color) const {
		return r != color.r || g != color.g || b != color.b;
	};

	bool operator!=(const float f) const {
		return r != f && g != f && b != f;
	};

	bool operator>(const float f) const {
		return r > f && g > f && b > f;
	}

	bool operator<(const float f) const {
		return r < f && g < f && b < f;
	}

	NiColor operator*=(const float f) {
		r *= f;
		g *= f;
		b *= f;
		return *this;
	};

	NiColor operator=(const NiPoint3& color) {
		r = color.x;
		g = color.y;
		b = color.z;
		return *this;
	};

	operator bool() const {
		return r != 0.0f || g != 0.0f || b != 0.0f;
	};

	const char* GetViewerStrings(const char* apText);

	double MultiplyBy3Something() {
		return r * 0.31f + g * 0.36f + b * 0.32f;
	};

	NiColor Min(float f) {
		NiColor comp = NiColor(f);
		if ((*this) > f) {
			return comp;
		}
		return *this;
	};

	NiColor Max(float f) {
		NiColor comp = NiColor(f);
		if ((*this) < f) {
			return comp;
		}
		return *this;
	};


	static NiColor Lerp(const NiColor& to, const NiColor& from, const double by) {
		return NiColor(to.r * (1 - by) + from.r * by,
			to.g * (1 - by) + from.g * by,
			to.b * (1 - by) + from.b * by);
	}

	static const NiColor Black;
	static const NiColor White;
	static const NiColor Red;
	static const NiColor Green;
	static const NiColor Blue;
	static const NiColor Yellow;
	static const NiColor Cyan;
	static const NiColor Magenta;
};

ASSERT_SIZE(NiColor, 0xC);

class NiColorA {
public:
	float r;
	float g;
	float b;
	float a;

	inline const float operator[] (UInt32 i) const { return ((float*)&r)[i]; };
	inline float operator[] (UInt32 i) { return ((float*)&r)[i]; };

	operator float* () const { return (float*)this; };

	NiColorA() : r(0.f), g(0.f), b(0.f), a(0.f) {};
	NiColorA(const NiColor& color) : r(color.r), g(color.g), b(color.b), a(1.0f) {};
	NiColorA(const NiColor& color, float a) : r(color.r), g(color.g), b(color.b), a(a) {};
	NiColorA(float r, float g, float b) : r(r), g(g), b(b), a(1.0f) {};
	NiColorA(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {};
	NiColorA(float f) : r(f), g(f), b(f), a(f) {};

	NiColorA operator*(const float f) const {
		return NiColorA(r * f, g * f, b * f, a * f);
	};

	NiColorA operator*(const NiColorA& color) const {
		return NiColorA(r * color.r, g * color.g, b * color.b, a * color.a);
	};

	bool operator==(const NiColorA& color) const {
		return r == color.r && g == color.g && b == color.b && a == color.a;
	};

	bool operator==(const float f) const {
		return r == f && g == f && b == f && a == f;
	};

	bool operator!=(const NiColorA& color) const {
		return r != color.r || g != color.g || b != color.b || a != color.a;
	};

	bool operator>(const float f) const {
		return r > f && g > f && b > f && a > f;
	}

	bool operator<(const float f) const {
		return r < f && g < f && b < f && a < f;
	}

	NiColorA operator*=(const float f) {
		r *= f;
		g *= f;
		b *= f;
		a *= f;
		return *this;
	};

	NiColorA operator=(const NiColor& color) {
		r = color.r;
		g = color.g;
		b = color.b;
		return *this;
	};

	NiColorA operator=(const NiPoint3& color) {
		r = color.x;
		g = color.y;
		b = color.z;
		return *this;
	};

	NiColorA operator+(const NiColorA& color) const {
		return NiColorA(r + color.r, g + color.g, b + color.b, a + color.a);
	}

	static const NiColorA Black;
	static const NiColorA White;
	static const NiColorA Red;
	static const NiColorA Green;
	static const NiColorA Blue;
	static const NiColorA Yellow;
	static const NiColorA Cyan;
	static const NiColorA Magenta;

	static NiColorA& GetRandomColor(float afAlpha = 1.f) {
		static NiColorA kColor;
		kColor.r = (float)(rand() % 255) / 255.f;
		kColor.g = (float)(rand() % 255) / 255.f;
		kColor.b = (float)(rand() % 255) / 255.f;
		kColor.a = afAlpha;
		return kColor;
	}
};

ASSERT_SIZE(NiColorA, 0x10);