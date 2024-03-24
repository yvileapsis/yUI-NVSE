#pragma once

#include "NiAVObject.hpp"
#include "NiTArray.hpp"

NiSmartPointer(NiNode);

class NiFixedString;

class NiNode : public NiAVObject {
public:
	NiNode();
	virtual ~NiNode();

	virtual void			AttachChild(NiAVObject* apChild, bool abFirstAvail);
	virtual void			InsertChildAt(UInt32 i, NiAVObject* apChild);
	virtual void			DetachChild(NiAVObject* apChild, NiAVObjectPtr* aspAVObject);
	virtual void			DetachChildAlt(NiAVObject* apChild);
	virtual void			DetachChildAt(UInt32 i, NiAVObjectPtr* aspAVObject);
	virtual NiAVObject*		DetachChildAtAlt(UInt32 i);
	virtual void			SetAt(UInt32 i, NiAVObject* apChild, NiAVObjectPtr* aspAVObject);
	virtual void			SetAtAlt(UInt32 i, NiAVObject* apChild);
	virtual void			UpdateUpwardPass();

	NiTObjectArray<NiAVObjectPtr> m_kChildren;

	CREATE_OBJECT(NiNode, 0xA5F030);
	NIRTTI_ADDRESS(0x11F4428);

	static NiNode* Create(UInt16 ausChildCount = 0);

	UInt32 GetArrayCount() const;
	UInt32 GetChildCount() const;
	NiAVObject* GetAt(UInt32 i) const;
	NiAVObject* GetAtSafely(UInt32 i) const;
	NiAVObject* GetLastChild();
	void CompactChildArray();

	NiNode* FindNodeByName(const NiFixedString& akName);

	void UpdatePropertiesUpward(NiPropertyState*& apParentState);

	static void __fastcall UpdateSelectedDownwardPassEx(BSFadeNode* apThis, void*, const NiUpdateData& arData, UInt32 auiFlags);
	static void __fastcall ApplyTransformEx(NiNode* apThis, void*, NiMatrix3& kMat, NiPoint3& kTrn, bool abOnLeft);
	static void __fastcall OnVisibleEx(NiNode* apThis, void*, NiCullingProcess* apCuller);

	static void SetFlagRecurse(NiNode* apNode, UInt32 auiFlag, bool abSet);
};

ASSERT_SIZE(NiNode, 0xAC);