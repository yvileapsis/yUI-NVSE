#pragma once

#include "bhkSerializable.hpp"

class hkpEntity;

class bhkConstraint : public bhkSerializable {
public:
	virtual void		SetEntityA(hkpEntity* apEntity);
	virtual void		SetEntityB(hkpEntity* apEntity);
	virtual hkpEntity*  GethkEntityA();
	virtual hkpEntity*  GethkEntityB();
	virtual void		ProcessEntityB(bhkWorld* apWorld, bool abAdd);
};