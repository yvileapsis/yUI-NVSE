#pragma once

#include "NiProperty.hpp"
#include "BSMemObject.hpp"

NiSmartPointer(NiPropertyState);

class NiAlphaProperty;
class NiCullingProperty;
class NiMaterialProperty;
class NiShadeProperty;
class NiStencilProperty;
class NiTexturingProperty;

class NiPropertyState : public BSMemObject {
public:
	NiPropertyState();
	NiPropertyState(const NiPropertyState& arSource);
	~NiPropertyState();

	enum PropertyID {
		ALPHA = 0,
		CULLING = 1,
		MATERIAL = 2,
		SHADE = 3,
		STENCIL = 4,
		TEXTURING = 5,
		UNK = 6, // Never seen this one used - maybe I could add custom properties here?
		MAX,
	};

	union {
		struct {
			NiPointer<NiAlphaProperty>		m_spAlphaProperty;
			NiPointer<NiCullingProperty>	m_spCullingProperty;
			NiPointer<NiMaterialProperty>	m_spMaterialProperty;
			NiPointer<NiShadeProperty>		m_spShadeProperty;
			NiPointer<NiStencilProperty>	m_spStencilProperty;
			NiPointer<NiTexturingProperty>	m_spTextureProperty;
			NiPointer<NiProperty>			m_spUnknownProperty;
		};
		NiPropertyPtr	m_aspProps[MAX];
	};

	void Reset();

	__forceinline NiAlphaProperty* GetAlphaProperty() const { return m_spAlphaProperty.m_pObject; };

	__forceinline NiCullingProperty* GetCullingProperty() const { return m_spCullingProperty.m_pObject; };

	__forceinline NiMaterialProperty* GetMaterialProperty() const { return m_spMaterialProperty.m_pObject; };

	template <class T> 
	T* GetShadeProperty() const { return static_cast<T*>(m_spShadeProperty.m_pObject); };

	__forceinline NiStencilProperty* GetStencilProperty() const { return m_spStencilProperty.m_pObject; };

	__forceinline NiTexturingProperty* GetTexturingProperty() const { return m_spTextureProperty.m_pObject; };
};

ASSERT_SIZE(NiPropertyState, 0x1C);