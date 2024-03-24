#pragma once

#include "NiRefObject.hpp"

class NiShaderDescBase : public NiRefObject {
public:
	char* m_pcName;
	char* m_pcDesc;
};