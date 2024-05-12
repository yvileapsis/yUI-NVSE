#pragma once

#include "NiRefObject.hpp"
#include "NiRTTI.hpp"
#include "NiTArray.hpp"

class bhkBlendCollisionObject;
class bhkNiCollisionObject;
class bhkLimitedHingeConstraint;
class bhkRigidBody;
class BSFadeNode;
class BSMultiBoundNode;
class BSResizableTriShape;
class BSSegmentedTriShape;
class NiCloningProcess;
class NiGeometry;
class NiLines;
class NiNode;
class NiParticles;
class NiStream;
class NiTriBasedGeom;
class NiTriShape;
class NiTriStrips;
class NiControllerManager;
class NiObjectGroup;
class NiBound;

NiSmartPointer(NiObject);

class NiObject : public NiRefObject {
public:
	NiObject();
	virtual ~NiObject();

	virtual const NiRTTI*				GetRTTI() const;												// 02 | Returns NiRTTI of the object
	virtual NiNode*						IsNiNode() const;												// 03 | Returns this if it's a NiNode, otherwise nullptr
	virtual BSFadeNode*					IsFadeNode() const;												// 04 | Returns this if it's a BSFadeNode, otherwise nullptr
	virtual BSMultiBoundNode*			IsMultiBoundNode() const;										// 05 | Returns this if it's a BSMultiBoundNode, otherwise nullptr
	virtual NiGeometry*					IsGeometry() const;												// 06 | Returns this if it's a NiGeometry, otherwise nullptr
	virtual NiTriBasedGeom*				IsTriBasedGeometry() const;										// 07 | Returns this if it's a NiTriBasedGeom, otherwise nullptr
	virtual NiTriStrips*				IsTriStrips() const;											// 08 | Returns this if it's a NiTriStrips, otherwise nullptr
	virtual NiTriShape*					IsTriShape() const;												// 09 | Returns this if it's a NiTriShape, otherwise nullptr
	virtual BSSegmentedTriShape*		IsSegmentedTriShape() const;									// 10 | Returns this if it's a BSSegmentedTriShape, otherwise nullptr
	virtual BSResizableTriShape*		IsResizableTriShape() const;									// 11 | Returns this if it's a BSResizableTriShape, otherwise nullptr
	virtual NiParticles*				IsParticlesGeom() const;										// 12 | Returns this if it's a NiParticles, otherwise nullptr
	virtual NiLines*					IsLinesGeom() const;											// 13 | Returns this if it's a NiLines, otherwise nullptr
	virtual bhkNiCollisionObject*		IsBhkNiCollisionObject() const;									// 14 | Returns this if it's a bhkNiCollisionObject, otherwise nullptr
	virtual bhkBlendCollisionObject*	IsBhkBlendCollisionObject() const;								// 15 | Returns this if it's a bhkBlendCollisionObject, otherwise nullptr
	virtual bhkRigidBody*				IsBhkRigidBody() const;											// 16 | Returns this if it's a bhkRigidBody, otherwise nullptr
	virtual bhkLimitedHingeConstraint*	IsBhkLimitedHingeConstraint() const;							// 17 | Returns this if it's a bhkLimitedHingeConstraint, otherwise nullptr
	virtual NiObject*					CreateClone(NiCloningProcess* apCloning);						// 18 | Creates a clone of this object
	virtual void						LoadBinary(NiStream* apStream);									// 19 | Loads objects from disk
	virtual void						LinkObject(NiStream* apStream);									// 20 | Called by the streaming system to resolve links to other objects once it can be guaranteed that all objects have been loaded
	virtual void						RegisterStreamables(NiStream* apStream);						// 21 | When an object is inserted into a stream, it calls register streamables to make sure that any contained objects or objects linked in a scene graph are streamed as well
	virtual void						SaveBinary(NiStream* apStream);									// 22 | Saves objects to disk
	virtual bool						IsEqual(NiObject* apObject) const;								// 23 | Compares this object with another
	virtual void						GetViewerStrings(NiViewerStringsArray* apStrings);				// 24 | Gets strings containing information about the object
	virtual void						AddViewerStrings(NiViewerStringsArray* apStrings);				// 25 | Adds additional strings containing information about the object
	virtual void						ProcessClone(NiCloningProcess* apCloning);						// 26 | Post process for CreateClone
	virtual void						PostLinkObject(NiStream* apStream);								// 27 | Called by the streaming system to resolve any tasks that require other objects to be correctly linked. It is called by the streaming system after LinkObject has been called on all streamed objects
	virtual bool						StreamCanSkip();												// 28
	virtual const NiRTTI*				GetStreamableRTTI();											// 29
	virtual void						SetBound(NiBound* apNewBound);									// 30 | Replaces the bound of the object
	virtual void						GetBlockAllocationSize();										// 31 | Used by geometry data
	virtual NiObjectGroup*				GetGroup();														// 32 | Used by geometry data
	virtual void						SetGroup(NiObjectGroup* apGroup);								// 33 | Used by geometry data
	virtual NiControllerManager*		IsControllerManager() const;									// 34 | Returns this if it's a NiControllerManager, otherwise nullptr

	NIRTTI_ADDRESS(0x11F4418);

	NiObject* Clone() { return ThisStdCall<NiObject*>(0xA5D680, this); }

	template <class T_RTTI>
	bool IsKindOf() const {
		return IsKindOf(T_RTTI::ms_RTTI);
	}

	template <class T_RTTI>
	bool IsExactKindOf() const {
		return IsExactKindOf(T_RTTI::ms_RTTI);
	}


	bool IsKindOf(const NiRTTI& apRTTI) const;

	bool IsKindOf(const NiRTTI* const apRTTI) const;

	bool IsExactKindOf(const NiRTTI* const apRTTI) const;

	bool IsExactKindOf(const NiRTTI& apRTTI) const;

	static bool IsExactKindOf(const NiRTTI& apRTTI, NiObject* apObject);

	static bool IsExactKindOf(const NiRTTI* const apRTTI, NiObject* apObject);

	template <typename T>
	T* NiDynamicCast() const {
		return IsKindOf(T::ms_RTTI) ? (T*)this : nullptr;
	}

	const char* GetRTTIName() const {
		return GetRTTI()->GetName();
	}
};

ASSERT_SIZE(NiObject, 0x8);

// JIP macros to check types without virtual funcs
#define IS_NODE(object) ((*(UInt32**)object)[3 * 4 >> 2] == 0x6815C0)
#define IS_FADENODE(object) ((*(UInt32**)object)[4 * 4 >> 2] == 0x6815C0)
#define IS_MULTIBOUNDNODE(object) ((*(UInt32**)object)[5 * 4 >> 2] == 0x6815C0)
#define IS_GEOMETRY(object) ((*(UInt32**)object)[6 * 4 >> 2] == 0xE68810)
#define IS_TRIGEOMETRY(object) ((*(UInt32**)object)[7 * 4 >> 2] == 0xE68810)
#define IS_TRISTRIPS(object) ((*(UInt32**)object)[8 * 4 >> 2] == 0xE68810)
#define IS_TRISHAPE(object) ((*(UInt32**)object)[9 * 4 >> 2] == 0xE68810)
#define IS_SEGMENTEDTRISHAPE(object) ((*(UInt32**)object)[10 * 4 >> 2] == 0xE68810)
#define IS_RESIZABLETRISHAPE(object) ((*(UInt32**)object)[11 * 4 >> 2] == 0xE68810)
#define IS_PARTICLE(object) ((*(UInt32**)object)[12 * 4 >> 2] == 0xE68810)
#define IS_LINE(object) ((*(UInt32**)object)[13 * 4 >> 2] == 0xE68810)
#define IS_NICOLLISION(object) ((*(UInt32**)object)[14 * 4 >> 2] == 0x6815C0)
#define IS_BLENDCOLLISION(object) ((*(UInt32**)object)[15 * 4 >> 2] == 0xE68810)
#define IS_RIGIDBODY(object) ((*(UInt32**)object)[16 * 4 >> 2] == 0x6815C0)
#define IS_HINGECONSTRAINT(object) ((*(UInt32**)object)[17 * 4 >> 2] == 0xE68810)
#define IS_CONTROLLERMANAGER(object) ((*(UInt32**)object)[34 * 4 >> 2] == 0xE68810)