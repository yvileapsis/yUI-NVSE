#pragma once

#include "Ni2DBuffer.hpp"

NiSmartPointer(NiDepthStencilBuffer);

class NiDepthStencilBuffer : public Ni2DBuffer {
};

ASSERT_SIZE(NiDepthStencilBuffer, 0x014);