#pragma once

#include "NiPSysEmitter.hpp"
#include "NiPoint3.hpp"
#include "NiGeometry.hpp"

NiSmartPointer(NiPSysMeshEmitter);
NiSmartPointer(NiSkinnedEmitterData);

class NiPSysMeshEmitter : public NiPSysEmitter {
public:
	NiPSysMeshEmitter();
	virtual ~NiPSysMeshEmitter();

	enum MeshEmissionType {
		NI_EMIT_FROM_VERTICES = 0,
		NI_EMIT_FROM_FACE_CENTER,
		NI_EMIT_FROM_EDGE_CENTER,
		NI_EMIT_FROM_FACE_SURFACE,
		NI_EMIT_FROM_EDGE_SURFACE,
		NI_EMIT_MAX
	};

	enum InitialVelocityType {
		NI_VELOCITY_USE_NORMALS = 0,
		NI_VELOCITY_USE_RANDOM,
		NI_VELOCITY_USE_DIRECTION,
		NI_VELOCITY_MAX
	};

	class NiSkinnedEmitterData : public NiRefObject {
	public:
		NiSkinnedEmitterData();
		virtual ~NiSkinnedEmitterData();

		NiSkinPartitionPtr m_spSkinPartition;
	};

	NiTObjectArray<NiGeometryPtr>			m_kGeomEmitterArray;
	NiTObjectArray<NiSkinnedEmitterDataPtr>	m_kSkinnedEmitterData;
	InitialVelocityType						m_eInitVelocityType;
	MeshEmissionType						m_eEmissionType;
	NiPoint3								m_kEmitAxis;

	CREATE_OBJECT(NiPSysMeshEmitter, 0xC1F970);
};

ASSERT_SIZE(NiPSysMeshEmitter, 0x88);