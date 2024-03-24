#pragma once

#include "NiPoint3.hpp"

#ifdef USE_DX_MATH
class NiMatrix3 : public XMFLOAT3X3, public NiMemObject {
public:
	NiMatrix3() : XMFLOAT3X3(1.f, 0.f, 0.f,
							 0.f, 1.f, 0.f,
							 0.f, 0.f, 1.f) {};
	NiMatrix3(const float m00, const float m10, const float m20,
			  const float m01, const float m11, const float m21,
			  const float m02, const float m12, const float m22) : XMFLOAT3X3(m00, m01, m02,
						  													  m10, m11, m12,
						  													  m20, m21, m22) {};
	NiMatrix3(const XMFLOAT3X3& src) : XMFLOAT3X3(src) {};
	NiMatrix3(const XMFLOAT3X3* src) : XMFLOAT3X3(*src) {};
	NiMatrix3(const XMMATRIX& src) { XMStoreFloat3x3(this, src); };
	NiMatrix3(const XMMATRIX* src) { XMStoreFloat3x3(this, *src); };
	~NiMatrix3() {};
#else
class NiMatrix3 : public NiMemObject {
public:
	float	m_pEntry[3][3];

	NiMatrix3() {}
	NiMatrix3(float m00, float m10, float m20, float m01, float m11, float m21, float m02, float m12, float m22)
	{
		m_pEntry[0][0] = m00;
		m_pEntry[0][1] = m10;
		m_pEntry[0][2] = m20;
		m_pEntry[1][0] = m01;
		m_pEntry[1][1] = m11;
		m_pEntry[1][2] = m21;
		m_pEntry[2][0] = m02;
		m_pEntry[2][1] = m12;
		m_pEntry[2][2] = m22;
	}
#endif
	bool operator==(const NiMatrix3& mat) const;
	NiMatrix3 operator+(const NiMatrix3& mat) const;
	NiMatrix3 operator-(const NiMatrix3& mat) const;
	NiMatrix3 operator*(const NiMatrix3& mat) const;
	NiMatrix3 operator*(float fScalar) const;
	NiPoint3 operator*(const NiPoint3& pt) const;
	friend NiPoint3 operator*(const NiPoint3& pt, const NiMatrix3& mat);

	NiMatrix3 operator/(float fScalar) const;

	void MakeXRotation(float fAngle);

	void MakeYRotation(float fAngle);

	void MakeZRotation(float fAngle);

	void MakeRotation(float fAngle, float x, float y, float z);
	void MakeRotation(float angle, const NiPoint3& axis);

	void FromEulerAnglesXYZ(float fXAngle, float fYAngle, float fZAngle);

	__forceinline void SetCol(UInt32 uiCol, const NiPoint3& col) {
#ifdef USE_DX_MATH
		m[0][uiCol] = col.x;
		m[1][uiCol] = col.y;
		m[2][uiCol] = col.z;
#else
		m_pEntry[0][uiCol] = col.x;
		m_pEntry[1][uiCol] = col.y;
		m_pEntry[2][uiCol] = col.z;
#endif
	}
	__forceinline void GetCol(UInt32 uiCol, float& f0, float& f1, float& f2) const{
#ifdef USE_DX_MATH
		f0 = m[0][uiCol];
		f1 = m[1][uiCol];
		f2 = m[2][uiCol];
#else
		f0 = m_pEntry[0][uiCol];
		f1 = m_pEntry[1][uiCol];
		f2 = m_pEntry[2][uiCol];
#endif
	}

	__forceinline void GetCol(UInt32 uiCol, NiPoint3& col) const {
#ifdef USE_DX_MATH
		col.x = m[0][uiCol];
		col.y = m[1][uiCol];
		col.z = m[2][uiCol];
#else
		col.x = m_pEntry[0][uiCol];
		col.y = m_pEntry[1][uiCol];
		col.z = m_pEntry[2][uiCol];
#endif
	}

	__forceinline NiPoint3 GetCol(UInt32 uiCol) const {
		NiPoint3 col;
#ifdef USE_DX_MATH
		col.x = m[0][uiCol];
		col.y = m[1][uiCol];
		col.z = m[2][uiCol];
#else
		col.x = m_pEntry[0][uiCol];
		col.y = m_pEntry[1][uiCol];
		col.z = m_pEntry[2][uiCol];
#endif
		return col;
	}

	NiMatrix3(const NiPoint3& col0, const NiPoint3& col1, const NiPoint3& col2) {
		SetCol(0, col0);
		SetCol(1, col1);
		SetCol(2, col2);
	}

	__forceinline void GetRow(UInt32 uiRow, NiPoint3& row) const {
#ifdef USE_DX_MATH
		row.x = m[uiRow][0];
		row.y = m[uiRow][1];
		row.z = m[uiRow][2];
#else
		row.x = m_pEntry[uiRow][0];
		row.y = m_pEntry[uiRow][1];
		row.z = m_pEntry[uiRow][2];
#endif
	}

	float* GetRow(UInt32 uiRow) {
		NiPoint3 row;
#ifdef USE_DX_MATH
		return &m[uiRow][0];
#else
		return &m_pEntry[uiRow][0];
#endif
	}

	__forceinline void SetRow(UInt32 uiRow, float f0, float f1, float f2) {
#ifdef USE_DX_MATH
		m[uiRow][0] = f0;
		m[uiRow][1] = f1;
		m[uiRow][2] = f2;
#else
		m_pEntry[uiRow][0] = f0;
		m_pEntry[uiRow][1] = f1;
		m_pEntry[uiRow][2] = f2;
#endif
	}

	__forceinline float GetEntry(UInt32 uiRow, UInt32 uiCol) const {
#ifdef USE_DX_MATH
		return m[uiRow][uiCol];
#else
		return m_pEntry[uiRow][uiCol];
#endif
	}

	__forceinline void SetEntry(UInt32 uiRow, UInt32 uiCol, float fValue) {
#ifdef USE_DX_MATH
		m[uiRow][uiCol] = fValue;
#else
		m_pEntry[uiRow][uiCol] = fValue;
#endif
	}

	__forceinline NiMatrix3 Transpose() {
		NiPoint3 row[3];

		GetRow(0, row[0]);
		GetRow(1, row[1]);
		GetRow(2, row[2]);

		return NiMatrix3(row[0], row[1], row[2]);
	}

	NiMatrix3 TransposeTimes(const NiMatrix3& mat) const;
	bool Inverse(NiMatrix3& inv) const;

	D3DMATRIX ToD3D() const;

	static const NiMatrix3 IDENTITY;

	const char* GetViewerStrings(const char* apText);
};


ASSERT_SIZE(NiMatrix3, 0x24);

typedef NiMatrix3 NiMatrix33;