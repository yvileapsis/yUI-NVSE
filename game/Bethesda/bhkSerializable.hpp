#pragma once

#include "bhkRefObject.hpp"
#include "hkSerializableCinfo.hpp"

class bhkWorld;
class hkpWorld;

class bhkSerializable : public bhkRefObject {
public:
	bhkSerializable();
	virtual ~bhkSerializable();

	virtual hkpWorld*	GethkWorld();					// 37 | Returns the hkpWorld
	virtual bhkWorld*	GetbhkWorld();					// 38 | Same as above but null checks this
	virtual bool		Add(bhkWorld* apWorld);			// 39
	virtual bool		Remove();						// 40
	virtual void		KillCInfo(bool abCreated);		// 41
	virtual UInt32		GetCinfoSize();					// 42
	virtual UInt32		LoadCInfo(NiStream* arStream);	// 43
	virtual void		Init(void*);					// 44
	virtual void*		CreateCInfo(bool*);				// 45
	virtual void*		Kill();							// 46
	virtual void*		KillCInfo2(UInt32);				// 47 | Figure out the name
	virtual void*		Unk_48();						// 48

	hkSerializableCinfo* pInfo;
};

ASSERT_SIZE(bhkSerializable, 0x10);