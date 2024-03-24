#pragma once

class NiAnimationKey {
public:
	enum KeyContent
	{
		FLOATKEY,
		POSKEY,
		ROTKEY,
		COLORKEY,
		TEXTKEY,
		BOOLKEY,
		NUMKEYCONTENTS
	};

	enum KeyType
	{
		NOINTERP,
		LINKEY,
		BEZKEY,
		TCBKEY,
		EULERKEY,
		STEPKEY,
		NUMKEYTYPES
	};

	float m_fTime;
};