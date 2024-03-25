#pragma once
#include "TESForm.hpp"

class NiNode;
class TESObjectREFR;
class TESBoundObject;
class TESWaterForm;
class TESObjectList;

// 0x24
class TESObject : public TESForm
{
public:
	TESObject();
	~TESObject() override;

	virtual UInt32			Unk_4E();
	virtual bool			Unk_4F();
	virtual TESWaterForm*	GetWaterType();
	virtual bool			Unk_51();
	virtual void			Unk_52(void* arg);
	virtual NiNode*			Clone3D(TESObjectREFR* apRequester, bool abDeepCopy);
	virtual void			UnClone3D(TESObjectREFR* apRequester);
	virtual bool			IsMarker();
	virtual bool			IsOcclusionMarker();
	virtual void			Unk_57();
	virtual bool			ReplaceModel();
	virtual bool			Unk_59(void* arg);
	virtual void			Unk_5A(void* arg0, void* arg1);
	virtual UInt32			Unk_5B();
	virtual UInt32			Unk_5C();
	virtual NiNode*			LoadGraphics(TESObjectREFR* apRef);

	TESObjectList*			pList;		// 018
	TESBoundObject*			pkPrev;		// 01C
	TESBoundObject*			pkNext;		// 020

	TESObject* GetNext() const;
	TESObject* GetPrev() const;
};
static_assert(sizeof(TESObject) == 0x24);