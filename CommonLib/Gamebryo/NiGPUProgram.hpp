#pragma once

#include "NiRefObject.hpp"

class NiGPUProgram : public NiRefObject {
public:
	NiGPUProgram();
	virtual ~NiGPUProgram();

	enum ProgramType {
		PROGRAM_VERTEX = 0,
		PROGRAM_PIXEL = 2,
		PROGRAM_GEOMETRY,
		PROGRAM_MAX
	};

	ProgramType m_eProgramType;
};