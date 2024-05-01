#pragma once

#include "NiPoint3.hpp"

#ifdef USE_DX_MATH

class NiPoint2 : public XMFLOAT2 {
public:
	NiPoint2() : XMFLOAT2(0.f, 0.f) {};
	NiPoint2(const float x, const float y) : XMFLOAT2(x, y) {};
	NiPoint2(const float f) : XMFLOAT2(f, f) {};
	NiPoint2(const XMFLOAT2& src) : XMFLOAT2(src.x, src.y)   {};
	NiPoint2(const XMFLOAT2* src) : XMFLOAT2(src->x, src->y) {};
	NiPoint2(const XMFLOAT3& src) : XMFLOAT2(src.x, src.y)   {};
	NiPoint2(const XMFLOAT3* src) : XMFLOAT2(src->x, src->y) {};
	NiPoint2(const XMFLOAT4& src) : XMFLOAT2(src.x, src.y)   {};
	NiPoint2(const XMFLOAT4* src) : XMFLOAT2(src->x, src->y) {};
	NiPoint2(const XMVECTOR& src) { XMStoreFloat2(this, src); };
	NiPoint2(const XMVECTOR* src) { XMStoreFloat2(this, *src); };
	~NiPoint2() {};
#else
class NiPoint2 {
public:
	float x;
	float y;

	NiPoint2() : x(0.0f), y(0.0f) {};
	NiPoint2(float x, float y) : x(x), y(y) {};
	NiPoint2(float xy) : x(xy), y(xy) {};
	NiPoint2(const NiPoint3& point) : x(point.x), y(point.y) {};
	NiPoint2(const NiPoint3* point) : x(point->x), y(point->y) {};

	auto operator<=>(const NiPoint2&) const = default;
#endif

	inline const float operator[] (UInt32 i) const { return ((float*)&x)[i]; };
	inline float operator[] (UInt32 i) { return ((float*)&x)[i]; };

	inline operator float* () const { return (float*)this; };

	// Point operations
	inline NiPoint2 operator+(const NiPoint2& arPoint) const { return NiPoint2(x + arPoint.x, y + arPoint.y); }

	inline NiPoint2 operator-(const NiPoint2& arPoint) const { return NiPoint2(x - arPoint.x, y - arPoint.y); }

	inline NiPoint2 operator*(const NiPoint2& arPoint) const { return NiPoint2(x * arPoint.x, y * arPoint.y); }

	inline NiPoint2 operator/(const NiPoint2& arPoint) const { return NiPoint2(x / arPoint.x, y / arPoint.y); }

	inline NiPoint2 operator-() const { return NiPoint2(-x, -y); }

	inline NiPoint2& operator+=(const NiPoint2& arPoint) {
		x += arPoint.x;
		y += arPoint.y;
		return *this;
	}

	inline NiPoint2& operator-=(const NiPoint2& arPoint) {
		x -= arPoint.x;
		y -= arPoint.y;
		return *this;
	}

	inline NiPoint2& operator*=(const NiPoint2& arPoint) {
		x *= arPoint.x;
		y *= arPoint.y;
		return *this;
	}

	inline NiPoint2& operator/=(const NiPoint2& arPoint) {
		x /= arPoint.x;
		y /= arPoint.y;
		return *this;
	}

	// Scalar operations
	inline NiPoint2 operator+(float afScalar) const { return NiPoint2(x + afScalar, y + afScalar); }

	inline NiPoint2 operator-(float afScalar) const { return NiPoint2(x - afScalar, y - afScalar); }

	inline NiPoint2 operator*(float afScalar) const { return NiPoint2(x * afScalar, y * afScalar); }

	inline NiPoint2 operator/(float afScalar) const { return NiPoint2(x / afScalar, y / afScalar); }

	inline NiPoint2& operator+=(float afScalar) {
		x += afScalar;
		y += afScalar;
		return *this;
	}

	inline NiPoint2& operator-=(float afScalar) {
		x -= afScalar;
		y -= afScalar;
		return *this;
	}

	inline NiPoint2& operator*=(float afScalar) {
		x *= afScalar;
		y *= afScalar;
		return *this;
	}

	inline NiPoint2& operator/=(float afScalar) {
		x /= afScalar;
		y /= afScalar;
		return *this;
	}
};

typedef __declspec(align(8)) NiPoint2 NiPoint2A;