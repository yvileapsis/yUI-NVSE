#pragma once

class InterfacedClass {
public:
	InterfacedClass() {};
	virtual ~InterfacedClass();
	virtual void AllocateTLSValue();		// not implemented
};