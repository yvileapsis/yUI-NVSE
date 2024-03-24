#pragma once

#include "BSShaderProperty.hpp"
#include "NiTListBase.hpp"

NiSmartPointer(BSShaderLightingProperty);

class BSShaderLightingProperty : public BSShaderProperty {
public:
	BSShaderLightingProperty();
	virtual ~BSShaderLightingProperty();

	virtual void CopyTo2();
	virtual void Unk_31(BSShaderLightingProperty*);

	NiTPointerList<ShadowSceneLight*>	kLightList;
	float								fUnk06C;
	UInt32								uiRefID;
	bool								bLightListChanged;
	NiTListIterator						kLightIterator;

	CREATE_OBJECT(BSShaderLightingProperty, 0xB71920);

	static ShadowSceneLight* pEmptyLight;

	UInt32				GetActiveLightCount() const;
	UInt32				GetActiveNonShadowLightCount() const;

	ShadowSceneLight*	GetNextLight(NiTListIterator& apIter) const;

	ShadowSceneLight*	GetIlluminatingLight(NiTListIterator& apIter) const;
	ShadowSceneLight*	GetNextIlluminatingLight(NiTListIterator& apIter) const;

	ShadowSceneLight*	GetNonShadowLight(NiTListIterator& apIter) const;
	ShadowSceneLight*	GetNextNonShadowLight(NiTListIterator& apIter) const;

	void				MoveEmptyLightToTop();
	void				SortLights(NiBound* apObjBound);
	void				AttachLight(ShadowSceneLight* apLight, NiBound* apBound);
	void				RemoveLights(NiGeometry* apGeometry);
	void				RemoveLight(ShadowSceneLight* apLight);
};

ASSERT_SIZE(BSShaderLightingProperty, 0x7C);