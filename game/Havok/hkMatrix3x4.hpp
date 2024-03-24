#pragma once

ALIGN16 class hkMatrix3x4 {
public:
	float	cr[3][4];

	hkMatrix3x4() { memset(cr, 0, sizeof(cr)); }
	hkMatrix3x4(float m00, float m10, float m20, float m01, float m11, float m21, float m02, float m12, float m22)
	{
		cr[0][0] = m00; cr[0][1] = m10; cr[0][2] = m20;
		cr[1][0] = m01; cr[1][1] = m11; cr[1][2] = m21;
		cr[2][0] = m02; cr[2][1] = m12; cr[2][2] = m22;
	}
};

ASSERT_SIZE(hkMatrix3x4, 0x30);