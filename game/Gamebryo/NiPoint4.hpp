#pragma once

#include "NiRect.hpp"
#include "NiPoint3.hpp"

class NiPoint4 : public NiMemObject {
public:
	union {
		struct {
			float x;
			float y;
			float z;
			float w;
		};

		struct {
			float f[4];
		};
	};

	NiPoint4() : x(0.f), y(0.f), z(0.f), w(0.f)
	{
	};

	NiPoint4(const float x, const float y, const float z, const float w) : x(x), y(y), z(z), w(w)
	{
	};

	NiPoint4(const NiPoint3& src) : x(src.x), y(src.y), z(src.z), w(0.f)
	{
	};

	operator NiRect<float>() const
	{
		return NiRect<float>(x, y, z, w);
	};

	bool operator== (const NiPoint4& pt) const {
		return x == pt.x && y == pt.y && z == pt.z && w == pt.w;
	}

	NiPoint4 operator=(NiPoint3 pt) const {
		return NiPoint4(pt.x, pt.y, pt.z, w);
	};

	void operator=(NiPoint3* pt) {
		x = pt->x;
		y = pt->y;
		z = pt->z;
	};

	void operator=(const NiPoint3& pt) {
		x = pt.x;
		y = pt.y;
		z = pt.z;
	};

	NiPoint4 operator-(NiPoint3& pt) const {
		return NiPoint4(x - pt.x, y - pt.y, z - pt.z, w);
	};

	NiPoint4 operator-(NiPoint3* pt) const {
		return NiPoint4(x - pt->x, y - pt->y, z - pt->z, w);
	};

	NiPoint4* operator-(const NiPoint3& pt) const {
		return new NiPoint4(x - pt.x, y - pt.y, z - pt.z, w);
	};

	NiPoint4* operator-(const NiPoint3* pt) const {
		return new NiPoint4(x - pt->x, y - pt->y, z - pt->z, w);
	};
};