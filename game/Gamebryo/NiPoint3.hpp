#pragma once

#include "NiMemObject.hpp"

class NiPoint3 : public NiMemObject {
public:
	union {
		struct {
			float x;
			float y;
			float z;
		};

		struct {
			float f[3];
		};
	};
	

	NiPoint3() : x(0.f), y(0.f), z(0.f) {};

	NiPoint3(const float x, const float y, const float z) : x(x), y(y), z(z) {};

	NiPoint3(const float f) : x(f), y(f), z(f) {};

	~NiPoint3() { };

	float operator[] (unsigned int ui) const { return f[ui]; };

	bool operator== (const NiPoint3& pt) const { return x == pt.x && y == pt.y && z == pt.z; }
	bool operator!= (const NiPoint3& pt) const { return !(*this == pt); }

	NiPoint3 operator+ (const NiPoint3& pt) const { return NiPoint3(x + pt.x, y + pt.y, z + pt.z); };
	NiPoint3& operator+= (const NiPoint3& pt) {
		x += pt.x;
		y += pt.y;
		z += pt.z;
		return *this;
	};

	NiPoint3 operator- (const NiPoint3& pt) const { return NiPoint3(x - pt.x, y - pt.y, z - pt.z); };
	NiPoint3 operator- () const { return NiPoint3(-x, -y, -z); };
	NiPoint3& operator-= (const NiPoint3& pt) {
		x -= pt.x;
		y -= pt.y;
		z -= pt.z;
		return *this;
	};

	float operator* (const NiPoint3& pt) const { return x * pt.x + y * pt.y + z * pt.z; };
	NiPoint3 operator* (float fScalar) const { return NiPoint3(fScalar * x, fScalar * y, fScalar * z); };
	friend NiPoint3 operator* (float fScalar, const NiPoint3& pt) { return NiPoint3(fScalar * pt.x, fScalar * pt.y, fScalar * pt.z); };
	NiPoint3& operator*= (float fScalar) {
		x *= fScalar;
		y *= fScalar;
		z *= fScalar;
		return *this;
	};

	NiPoint3 operator/ (float fScalar) const {
		float fInvScalar = 1.0f / fScalar;
		return NiPoint3(fInvScalar * x, fInvScalar * y, fInvScalar * z);
	};

	NiPoint3& operator/= (float fScalar) {
		x /= fScalar;
		y /= fScalar;
		z /= fScalar;
		return *this;
	};

	__forceinline float Length() const {
		return std::sqrt(x * x + y * y + z * z);
	}

	static float __fastcall Length_Hook(NiPoint3* apThis) {
		return apThis->Length();
	};

	__forceinline float SqrLength() const {
		return x * x + y * y + z * z;
	}

	__forceinline float Dot(const NiPoint3& pt) const {
		return x * pt.x + y * pt.y + z * pt.z;
	}

	__forceinline NiPoint3 Cross(const NiPoint3& pt) const {
		return NiPoint3(y * pt.z - z * pt.y, z * pt.x - x * pt.z, x * pt.y - y * pt.x);
	}

	__forceinline float Unitize() {
		float fLength = Length();

		if (fLength > 1e-06f) {
			float fRecip = 1.0f / fLength;
			x *= fRecip;
			y *= fRecip;
			z *= fRecip;
		}
		else
		{
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
			fLength = 0.0f;
		}
		return fLength;
	}

	inline NiPoint3 UnitCross(const NiPoint3& pt) const {
		NiPoint3 cross(y * pt.z - z * pt.y, z * pt.x - x * pt.z, x * pt.y - y * pt.x);
		float fLength = cross.Length();
		if (fLength > 1e-06f)
			return cross / fLength;
		else
			return NiPoint3(0.0f, 0.0f, 0.0f);
	}

	static NiPoint3 UNIT_X;
	static NiPoint3 UNIT_Y;
	static NiPoint3 UNIT_Z;
	static NiPoint3 UNIT_ALL;
	static NiPoint3 ZERO;

	static void PointsPlusEqualFloatTimesPoints(NiPoint3* pkDst, float f, const NiPoint3* pkSrc, unsigned int uiVerts) {
		for (unsigned int i = 0; i < uiVerts; i++) {
			pkDst[i] += f * pkSrc[i];
		}
	}

	void Lerp(NiPoint3& to, NiPoint3& from, double by)
	{
		x = to.x * (1 - by) + from.x * by;
		y = to.y * (1 - by) + from.y * by;
		z = to.z * (1 - by) + from.z * by;
	}

	float GetLargest() const {
		float largest = x;
		if (y > largest)
			largest = y;
		if (z > largest)
			largest = z;
		return largest;
	}
};

ASSERT_SIZE(NiPoint3, 0xC);