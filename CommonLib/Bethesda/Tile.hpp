#pragma once
#include "NiNode.hpp"
#include "BSSimpleArray.hpp"
#include "BSString.hpp"
#include "BSSimpleList.hpp"
#include "NiTList.hpp"

//	Tile			
//		TileRect		3C	ID=385
//			TileMenu	40	ID=389
//		TileImage		48	ID=386
//			RadialTile		ID=38C but answer as TileImage !
//		TileText		4C	ID=387
//		Tile3D			50	ID=388
//	Close by:
//		HotRect	ID=38A
//		Window	ID=38B

class NiColorAlpha;
class Menu;

// 0x38
class Tile : public NiTList<Tile*>
{
public:
	enum EnumActionType : UInt32
	{
		kAction_copy = 0x7D0,
		kAction_add,
		kAction_sub,
		kAction_mul,
		kAction_div,
		kAction_min,
		kAction_max,
		kAction_mod,
		kAction_floor,
		kAction_ceil,
		kAction_abs,
		kAction_round,
		kAction_gt,
		kAction_gte,
		kAction_eq,
		kAction_neq,
		kAction_lt,
		kAction_lte,
		kAction_and,
		kAction_or,
		kAction_not,
		kAction_onlyif,
		kAction_onlyifnot,
		kAction_ref,
		kAction_begin,
		kAction_end,
	};

	enum EnumID : UInt32
	{
		kID_rect = 0x0385,
		kID_image,
		kID_text,
		kID_3D,
		kID_nif = kID_3D,
		kID_menu,

		// Not a Tile descendend
		kID_hotrect,
		kID_window,
		// This one descend from TileImage
		kID_radial,

		kID_max,

		kID_template = 0x3E7,

		kID_value = 0xBB9,
		kID_name = 0xBBA,
		kID_src = 0xBBB,
		kID_trait = 0xBBC,
	};

	enum EnumValue : UInt32 
	{
		kValue_x = 0x0FA1,
		kValue_y,
		kValue_visible,
		kValue_class,
		kValue_clipwindow = 0x0FA6,
		kValue_stackingtype,
		kValue_locus,
		kValue_alpha,
		kValue_id,
		kValue_disablefade,
		kValue_listindex,
		kValue_depth,
		kValue_clips,
		kValue_target,
		kValue_height,
		kValue_width,
		kValue_red,
		kValue_green,
		kValue_blue,
		kValue_tile,
		kValue_childcount,
		kValue_child_count = kValue_childcount,
		kValue_justify,
		kValue_zoom,
		kValue_font,
		kValue_wrapwidth,
		kValue_wraplimit,
		kValue_wraplines,
		kValue_pagenum,
		kValue_ishtml,
		kValue_cropoffsety,
		kValue_cropy = kValue_cropoffsety,
		kValue_cropoffsetx,
		kValue_cropx = kValue_cropoffsetx,
		kValue_menufade,
		kValue_explorefade,
		kValue_mouseover,
		kValue_string,
		kValue_shiftclicked,
		kValue_clicked = 0x0FC7,
		kValue_clicksound = 0x0FCB,
		kValue_filename,
		kValue_filewidth,
		kValue_fileheight,
		kValue_repeatvertical,
		kValue_repeathorizontal,
		kValue_animation = 0x0FD2,
		kValue_linecount = 0x0DD4,
		kValue_pagecount,
		kValue_xdefault,
		kValue_xup,
		kValue_xdown,
		kValue_xleft,
		kValue_xright,
		kValue_xbuttona = 0x0FDD,
		kValue_xbuttonb,
		kValue_xbuttonx,
		kValue_xbuttony,
		kValue_xbuttonlt,
		kValue_xbuttonrt,
		kValue_xbuttonlb,
		kValue_xbuttonrb,
		kValue_xbuttonstart = 0x0FE7,
		kValue_mouseoversound,
		kValue_draggable,
		kValue_dragstartx,
		kValue_dragstarty,
		kValue_dragoffsetx,
		kValue_dragoffsety,
		kValue_dragdeltax,
		kValue_dragdeltay,
		kValue_dragx,
		kValue_dragy,
		kValue_wheelable,
		kValue_wheelmoved,
		kValue_systemcolor,
		kValue_brightness,
		kValue_linegap = 0x0FF7,
		kValue_resolutionconverter,
		kValue_texatlas,
		kValue_rotateangle,
		kValue_rotateaxisx,
		kValue_rotateaxisy,

		kValue_user0 = 0x01004,
		kValue_user1,
		kValue_user2,
		kValue_user3,
		kValue_user4,
		kValue_user5,
		kValue_user6,
		kValue_user7,
		kValue_user8,
		kValue_user9,
		kValue_user10,
		kValue_user11,
		kValue_user12,
		kValue_user13,
		kValue_user14,
		kValue_user15,
		kValue_user16,

		kValue_max,

		kValue_parent	= 5001,
		kValue_self		= 5002,
		kValue_unk1		= 5004,
		kValue_child	= 5005,
		kValue_root		= 5006,
		kValue_globals	= 5007,
		kValue_unk2		= 5008,
		kValue_grandparent = 5009,
		kValue_tilemenu = 5010,

		kValue_fadeout = 0x1772,
	};


	Tile();
	virtual								~Tile();
	virtual void						Init(Tile* parent, const char* name, Tile* replacedChild);
	virtual NiNode*						CalcNode();
	virtual UInt32						GetType() const;	// returns one of kTileValue_XXX
	virtual const char*					GetTypeName();		// 4-byte id i.e. 'TILE'
	virtual bool						OverRadialTile(Float32 x, Float32 y);
	virtual UInt32						InitMenu(UInt32 valueID, Float32 Float32Value, const char* strValue);
	virtual void						RecursiveHandleImageFilenameChange();
	virtual const char*					GetShaderProperty();
	virtual void						SetShaderPropertyColorAlpha(NiNode* a1, Float32 alpha, NiColorAlpha* arg2);

	struct Value;

	// 0C
	class Action
	{
	public:
		Action();
		~Action();

		virtual Float32	GetFloat();
		virtual Value*	GetValue();

		EnumActionType		eActionType;		// 04
		Action*				pkNextAction;		// 08
	};

	// 10
	class RefValueAction : public Action
	{
	public:
		RefValueAction();
		~RefValueAction();

		Value* pkTileValue;	// 0C
	};

	// 10
	class FloatAction : public Action
	{
	public:
		FloatAction();
		~FloatAction();

		Float32		fValue;		// 0C
	};

	// 14
	struct Value
	{
		EnumValue		eID;				// 00
		Tile*			pkParentTile;		// 04
		Float32			fNumericalValue;	// 08
		char*			pcStringValue;		// 0C
		Action*			pkActionValue;		// 10

		__forceinline void Refresh(bool string) { ThisCall<void>(0xA09410, this, string); };
		__forceinline void SetFloat(Float32 flt, char chr) { ThisCall<void>(0xA0A270, this, flt, chr); }
		__forceinline void SetString(const char* str, char chr) { ThisCall<void>(0xA0A300, this, str, chr); }
	};

	BSSimpleArray<Value*>		kValues;			// 10
	BSStringT<char>				kName;				// 20
	Tile*						pkParent;			// 28
	NiNode*						pkNode;				// 2C
	UInt32						uiFlags;			// 30
	UInt8						ucUpdateDeferred;	// 34
	UInt8						ucShouldPaintRed;	// 35
	UInt8						pad35[2];			// 36

	__forceinline static EnumValue		TraitNameToID(const char* traitName) { return CdeclCall<EnumValue>(0x00A01860, traitName); }

	__forceinline Value*		Get(const EnumValue aeID) const { return ThisCall<Value*>(0xA01000, const_cast<Tile*>(this), aeID);};
	__forceinline Value*		Get(const char* valueName) const { return Get(TraitNameToID(valueName)); }

	// 0xA012D0 without critical section
	__forceinline void			Set(const EnumValue aeID, const Float32 afValue, const bool abPropagate = true)
	{ ThisCall<Value*>(0xA01000, this, aeID)->SetFloat(afValue, abPropagate); }
	__forceinline void			Set(const char* apcID, const Float32 afValue, const bool abPropagate = true)
	{ Set(TraitNameToID(apcID), afValue, abPropagate); }

	// 0xA0137F without critical section
	__forceinline void			SetString(const EnumValue aeID, const char* apcValue, const bool abPropagate = true)
	{ ThisCall<Value*>(0xA01000, this, aeID)->SetString(apcValue, abPropagate); }
	__forceinline void			SetString(const char* apcID, const char* apcValue, const bool abPropagate = true)
	{ SetString(TraitNameToID(apcID), apcValue, abPropagate); }

	__forceinline void			SetGradual(const EnumValue aeID, const Float32 afStartValue, const Float32 afEndValue, const Float32 afSeconds, const UInt32 uiChangeMode = 0)
	{ ThisCall(0xA07C60, this, aeID, afStartValue, afEndValue, afSeconds, uiChangeMode); }
	__forceinline void			SetGradual(const char* apcID, const Float32 afStartValue, const Float32 afEndValue, const Float32 afSeconds, const UInt32 uiChangeMode = 0)
	{ SetGradual(TraitNameToID(apcID), afStartValue, afEndValue, afSeconds, uiChangeMode); }

	__forceinline Menu*			GetParentMenu() { return ThisCall<Menu*>(0xA03C90, this); };

	__forceinline void			HandleChange(const EnumValue id) { ThisCall<void>(0xA074D0, this, id); }
	__forceinline Tile*			GetChildByID(const EnumValue id) { return ThisCall<Tile*>(0xA03EB0, this, id); }; // THANKS STEWIE
	__forceinline Tile*			GetChildByName(const char* name) { return ThisCall<Tile*>(0xA03DA0, this, name); };
	__forceinline Tile*			GetChildByTraitName(const char* traitName) { return CdeclCall<Tile*>(0xA08B20, this, traitName); };

	bool						PlayTileSound(EnumValue id = kValue_clicksound) { return ThisCall<char>(0xA0B110, this, id); }
	Tile*						ReadXML(const char* apcXMLPath) { return ThisCall<Tile*>(0xA01B00, this, apcXMLPath); };

	std::string					GetFullPath() const;
};
static_assert(sizeof(Tile) == 0x38);