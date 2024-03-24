#pragma once
#include "NiMemObject.hpp"

#ifdef USE_DX_MATH
class NiQuaternion : public XMFLOAT4, public NiMemObject {
#else
class NiQuaternion : public NiMemObject {
public:
	float x, y, z, w;

	NiQuaternion() : x(0), y(0), z(0), w(0) {};
	NiQuaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {};

	NiQuaternion operator+(const NiQuaternion& rhs) const {
		return NiQuaternion(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
	}

	NiQuaternion operator-(const NiQuaternion& rhs) const {
		return NiQuaternion(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
	}

	NiQuaternion operator*(const float& rhs) const {
		return NiQuaternion(x * rhs, y * rhs, z * rhs, w * rhs);
	}

	NiQuaternion operator/(const float& rhs) const {
		return NiQuaternion(x / rhs, y / rhs, z / rhs, w / rhs);
	}

	NiQuaternion& operator+=(const NiQuaternion& rhs) {
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		w += rhs.w;
		return *this;
	}

	NiQuaternion& operator-=(const NiQuaternion& rhs) {
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		w -= rhs.w;
		return *this;
	}

	NiQuaternion& operator*=(const float& rhs) {
		x *= rhs;
		y *= rhs;
		z *= rhs;
		w *= rhs;
		return *this;
	}

	NiQuaternion& operator/=(const float& rhs) {
		x /= rhs;
		y /= rhs;
		z /= rhs;
		w /= rhs;
		return *this;
	}

	bool operator==(const NiQuaternion& rhs) const {
		return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
	}

	bool operator!=(const NiQuaternion& rhs) const {
		return x != rhs.x || y != rhs.y || z != rhs.z || w != rhs.w;
	}

	NiQuaternion& operator=(const NiQuaternion& rhs) {
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		w = rhs.w;
		return *this;
	}

	NiQuaternion& operator=(const float& rhs) {
		x = rhs;
		y = rhs;
		z = rhs;
		w = rhs;
		return *this;
	}
#endif
};