#pragma once

class InterfacedClass {
public:
	InterfacedClass() {};
	virtual ~InterfacedClass();
	virtual void* AllocateInterface(uint32_t auiThread) const;
};