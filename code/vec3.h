#pragma once

#include <math.h>

#define DECL_FLOAT_OP(op) \
	vec3 operator op(const float n) const \
	{ \
		return vec3(x op n, y op n, z op n); \
	} \
	vec3 operator op##=(const float n) \
	{ \
		return *this = vec3(x op n, y op n, z op n); \
	} \

#define DECL_VEC_OP(op) \
	vec3 operator op(const vec3 v) const \
	{ \
		return vec3(x op v.x, y op v.y, z op v.z); \
	} \
	vec3 operator op##=(const vec3 v) \
	{ \
		return *this = vec3(x op v.x, y op v.y, z op v.z); \
	}


struct vec3
{
	float x, y, z;

	vec3() : x(0.f), y(0.f), z(0.f)
	{
	}

	vec3(const float x, const float y, const float z) : x(x), y(y), z(z)
	{
	}

	DECL_FLOAT_OP(*);
	DECL_FLOAT_OP(/ );

	DECL_VEC_OP(+);
	DECL_VEC_OP(-);
	DECL_VEC_OP(*);
	DECL_VEC_OP(/ );

	float length() const
	{
		return sqrtf(x * x + y * y + z * z);
	}
	float length_sqr() const
	{
		return x * x + y * y + z * z;
	}

	vec3 normal() const
	{
		const auto len = length();
		return len == 0.F ? vec3() : vec3(x / len, y / len, z / len);
	}

	static float dot(const vec3& v1, const vec3& v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	static vec3 cross(const vec3& v1, const vec3& v2)
	{
		return vec3(
			v1.y * v2.z - v1.z * v2.y,
			v1.z * v2.x - v1.x * v2.z,
			v1.x * v2.y - v1.y * v2.x);
	}
};

#undef DECL_FLOAT_OP
#undef DECL_VEC_OP