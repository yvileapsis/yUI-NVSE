#pragma once

#include "NiGPUProgram.hpp"
#include "NiObject.hpp"

class NiD3DShaderProgramCreator;
class NiShaderConstantMapEntry;
class NiD3DRenderState;

class NiD3DShaderProgram : public NiGPUProgram {
public:
	NiD3DShaderProgram();
	virtual ~NiD3DShaderProgram();

	virtual NiRTTI*						GetRTTI();
	virtual NiNode*						IsNiNode();
	virtual BSFadeNode*					IsFadeNode();
	virtual BSMultiBoundNode*			IsMultiBoundNode();
	virtual NiGeometry*					IsGeometry();
	virtual NiTriBasedGeom*				IsTriBasedGeometry();
	virtual NiTriStrips*				IsTriStrips();
	virtual NiTriShape*					IsTriShape();
	virtual BSSegmentedTriShape*		IsSegmentedTriShape();
	virtual BSResizableTriShape*		IsResizableTriShape();
	virtual NiParticles*				IsParticlesGeom();
	virtual NiLines*					IsLinesGeom();
	virtual bhkNiCollisionObject*		IsBhkNiCollisionObject();
	virtual bhkBlendCollisionObject*	IsBhkBlendCollisionObject();
	virtual bhkRigidBody*				IsBhkRigidBody();
	virtual bhkLimitedHingeConstraint*	IsBhkLimitedHingeConstraint();
	virtual UInt32						GetVariableCount();
	virtual const char*					GetVariableName();
	virtual const char*					GetName();
	virtual const char*					SetName(const char* pszName);
	virtual char*						GetShaderProgramName();
	virtual void						SetShaderProgramName(const char* pszName);
	virtual UInt32						GetCodeSize();
	virtual void*						GetCode();
	virtual void						SetCode(UInt32 uiSize, void* pvCode);
	virtual NiD3DShaderProgramCreator*	GetCreator();
	virtual void						SetCreator(NiD3DShaderProgramCreator* pkCreator);
	virtual void						SetShaderConstant(NiShaderConstantMapEntry*, const void*, UInt32);
	virtual bool						SetShaderConstantArray(NiShaderConstantMapEntry* pkEntry, const void* pvDataSource, UInt32 uiNumEntries, UInt32 uiRegistersPerEntry, UInt16 pusReorderArray);

	char*						m_pszName;
	char*						m_pszShaderProgramName;
	UInt32						m_uiCodeSize;
	void*						m_pvCode;
	NiD3DShaderProgramCreator*	m_pkCreator;
	LPDIRECT3DDEVICE9				m_pkD3DDevice;
	NiD3DRenderer*				m_pkD3DRenderer;
	NiD3DRenderState*			m_pkD3DRenderState;
};

ASSERT_SIZE(NiD3DShaderProgram, 0x2C);