#pragma once
#include <NiNodes.h>
#include <Containers.h>
#include <Utilities.h>

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

// 38+
enum eTileValue {
	kTileValue_Rect = 0x385,
	kTileValue_Image = 0x386,
	kTileValue_Text = 0x387,
	kTileValue_Nif = 0x388,
	kTileValue_Menu = 0x389,
	kTileValue_Hotrect = 0x38A,
	kTileValue_Window = 0x38B,
	kTileValue_Radial = 0x38C,
	kTileValue_Template = 0x3E7,
	kTileValue_Abs = 0x7DA,
	kTileValue_Add = 0x7D1,
	kTileValue_And = 0x7E2,
	kTileValue_Ceil = 0x7D9,
	kTileValue_Copy = 0x7D0,
	kTileValue_Div = 0x7D4,
	kTileValue_Eq = 0x7DE,
	kTileValue_Floor = 0x7D8,
	kTileValue_Gt = 0x7DC,
	kTileValue_Gte = 0x7DD,
	kTileValue_Lt = 0x7E0,
	kTileValue_Lte = 0x7E1,
	kTileValue_Max = 0x7D6,
	kTileValue_Min = 0x7D5,
	kTileValue_Mod = 0x7D7,
	kTileValue_Mul = 0x7D3,
	kTileValue_Neq = 0x7DF,
	kTileValue_Not = 0x7E4,
	kTileValue_OnlyIf = 0x7E5,
	kTileValue_OnlyIfNot = 0x7E6,
	kTileValue_Or = 0x7E3,
	kTileValue_Ref = 0x7E7,
	kTileValue_Round = 0x7DB,
	kTileValue_Sub = 0x7D2,
	kTileValue_Name = 0xBBA,
	kTileValue_Src = 0xBBB,
	kTileValue_Trait = 0xBBC,
	kTileValue_Value = 0xBB9,

	kTileValue_x = 0x0FA1,
	kTileValue_y,
	kTileValue_visible,
	kTileValue_class,
	kTileValue_clipwindow = 0x0FA6,
	kTileValue_stackingtype,
	kTileValue_locus,
	kTileValue_alpha,
	kTileValue_id,
	kTileValue_disablefade,
	kTileValue_listindex,
	kTileValue_depth,
	kTileValue_clips,
	kTileValue_target,
	kTileValue_height,
	kTileValue_width,
	kTileValue_red,
	kTileValue_green,
	kTileValue_blue,
	kTileValue_tile,
	kTileValue_childcount,
	kTileValue_child_count = kTileValue_childcount,
	kTileValue_justify,
	kTileValue_zoom,
	kTileValue_font,
	kTileValue_wrapwidth,
	kTileValue_wraplimit,
	kTileValue_wraplines,
	kTileValue_pagenum,
	kTileValue_ishtml,
	kTileValue_cropoffsety,
	kTileValue_cropy = kTileValue_cropoffsety,
	kTileValue_cropoffsetx,
	kTileValue_cropx = kTileValue_cropoffsetx,
	kTileValue_menufade,
	kTileValue_explorefade,
	kTileValue_mouseover,
	kTileValue_string,
	kTileValue_shiftclicked,
	kTileValue_clicked = 0x0FC7,
	kTileValue_clicksound = 0x0FCB,
	kTileValue_filename,
	kTileValue_filewidth,
	kTileValue_fileheight,
	kTileValue_repeatvertical,
	kTileValue_repeathorizontal,
	kTileValue_animation = 0x0FD2,
	kTileValue_linecount = 0x0DD4,
	kTileValue_pagecount,
	kTileValue_xdefault,
	kTileValue_xup,
	kTileValue_xdown,
	kTileValue_xleft,
	kTileValue_xright,
	kTileValue_xbuttona = 0x0FDD,
	kTileValue_xbuttonb,
	kTileValue_xbuttonx,
	kTileValue_xbuttony,
	kTileValue_xbuttonlt,
	kTileValue_xbuttonrt,
	kTileValue_xbuttonlb,
	kTileValue_xbuttonrb,
	kTileValue_xbuttonstart = 0x0FE7,
	kTileValue_mouseoversound,
	kTileValue_draggable,
	kTileValue_dragstartx,
	kTileValue_dragstarty,
	kTileValue_dragoffsetx,
	kTileValue_dragoffsety,
	kTileValue_dragdeltax,
	kTileValue_dragdeltay,
	kTileValue_dragx,
	kTileValue_dragy,
	kTileValue_wheelable,
	kTileValue_wheelmoved,
	kTileValue_systemcolor,
	kTileValue_brightness,
	kTileValue_linegap = 0x0FF7,
	kTileValue_resolutionconverter,
	kTileValue_texatlas,
	kTileValue_rotateangle,
	kTileValue_rotateaxisx,
	kTileValue_rotateaxisy,

	kTileValue_user0 = 0x01004,
	kTileValue_user1,
	kTileValue_user2,
	kTileValue_user3,
	kTileValue_user4,
	kTileValue_user5,
	kTileValue_user6,
	kTileValue_user7,
	kTileValue_user8,
	kTileValue_user9,
	kTileValue_user10,
	kTileValue_user11,
	kTileValue_user12,
	kTileValue_user13,
	kTileValue_user14,
	kTileValue_user15,
	kTileValue_user16,

	kTileValue_max
};


class Tile
{
public:

	// 14
	struct Value
	{
		enum ActionType
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

		// 0C
		class Action
		{
		public:
			Action();
			~Action();

			virtual float		GetFloat();
			virtual Value*		GetValue();

			UInt32	type;		// 04
			Action* next;		// 08
		};

		// 10
		class RefValueAction : public Action
		{
		public:
			RefValueAction();
			~RefValueAction();

			Value* tileVal;	// 0C
		};

		// 10
		class FloatAction : public Action
		{
		public:
			FloatAction();
			~FloatAction();

			float		value;		// 0C
		};

		UInt32		id;			// 00
		Tile*		parent;		// 04
		float		num;		// 08
		char*		str;		// 0C
		Action*		action;		// 10

		__forceinline void Refresh(bool string) { ThisCall<void>(0xA09410, this, string); };
		__forceinline void SetFloat(Float32 flt, char chr) { ThisCall<void>(0xA0A270, this, flt, chr); }
		__forceinline void SetString(const char* str, char chr) { ThisCall<void>(0xA0A300, this, str, chr); }
	};

	class ValueCaster
	{
		Value* ptr;
	public:
		ValueCaster(Value* ptr = nullptr) : ptr(ptr) {};
		explicit operator bool() const { return ptr && (ptr->num || ptr->str || ptr->action); }
		operator Float32() const { return ptr && ptr->num ? ptr->num : 0.0; }
		explicit operator UInt32() const { return ptr && ptr->num ? ptr->num : 0; }
		operator const char*() const { return ptr && ptr->str ? ptr->str : ""; }
	};

	enum eTileID {
		kTileID_rect = 0x0385,
		kTileID_image,
		kTileID_text,
		kTileID_3D,
		kTileID_nif = kTileID_3D,
		kTileID_menu,

		// Not a Tile descendend
		kTileID_hotrect,
		kTileID_window,
		// This one descend from TileImage
		kTileID_radial,

		kTileID_max
	};

	Tile();
	virtual ~Tile();
	virtual void			Init(Tile* parent, const char* name, Tile* replacedChild);
	virtual NiNode*			CalcNode();
	virtual UInt32			GetType();		// returns one of kTileValue_XXX
	virtual const char*		GetTypeName();	// 4-byte id i.e. 'TILE'
	virtual bool			OverRadialTile(Float32 x, Float32 y);
	virtual UInt32			InitMenu(UInt32 valueID, float floatValue, const char* strValue);
	virtual void			RecursiveHandleImageFilenameChange();
	virtual const char*		GetShaderProperty();
	virtual void			SetShaderPropertyColorAlpha(NiNode* a1, Float32 alpha, NiColorAlpha* arg2);

	struct ChildNode
	{
		ChildNode*				next;		// 000
		ChildNode*				prev;		// 004
		Tile*					child;		// 008
	};

	DList<Tile>					children;	// 04
	BSSimpleArray<Value*>		values;		// 10
	String						name;		// 20
	Tile*						parent;		// 28
	NiNode*						node;		// 2C
	UInt32						flags;		// 30
	UInt8						unk34;		// 34
	UInt8						unk35;		// 35
	UInt8						pad35[2];	// 36

	__forceinline static UInt32	TraitNameToID(const char* traitName) { return CdeclCall<UInt32>(0x00A01860, traitName); }
	static const char*			TraitIDToName(int id);	// slow
	__forceinline Value*		__fastcall	GetValue(UInt32 typeID);
	Value*						GetValue(const char* valueName) { return GetValue(TraitNameToID(valueName)); }

	Tile*						GetNthChild(UInt32 index);
	Tile*						GetChild(const std::string& childName) const;
	Tile*						GetComponent(const std::string& componentPath);

	__forceinline Tile*			ReadXML(const std::filesystem::path& xmlPath) { return ThisCall<Tile*>(0xA01B00, this, xmlPath.generic_string().c_str()); };
	__forceinline Tile*			InjectUIXML(const std::filesystem::path& str) { return exists(str) ? ReadXML(str) : nullptr; };
	Tile*						InjectUIXML(const std::filesystem::path& str, bool IgnoreUIO);

	__forceinline ValueCaster	Get(const UInt32 id) { return GetValue(id); }
	__forceinline ValueCaster	Get(const char* id) { return Get(TraitNameToID(id)); };

	__forceinline void			Set(UInt32 id, Float32 fltVal, bool bPropagate = true)
	{
		Value* value;
		if (value = GetValue(id); !value) value = ThisCall<Value*>(0xA01000, this, id);
		value->SetFloat(fltVal, bPropagate);
	}
	__forceinline void			Set(const char* id, const Float32 fltVal, bool bPropagate = true) { Set(TraitNameToID(id), fltVal, bPropagate); }

	__forceinline void			Set(const UInt32 id, const std::string& strVal, const bool bPropagate = true)
	{
		Value* value;
		if (value = GetValue(id); !value) value = ThisCall<Value*>(0xA01000, this, id);
		value->SetString(strVal.c_str(), bPropagate);
	}
	__forceinline void			Set(const char* id, const std::string& strVal, bool bPropagate = true) { Set(TraitNameToID(id), strVal.c_str(), bPropagate); }

	void						SetStringRecursive(UInt32, const char*, const char*);

	__forceinline void			SetGradual(UInt32 id, Float32 startVal, Float32 endVal, Float32 seconds, UInt32 changeMode = 0)
								{ ThisCall(0xA07C60, this, id, startVal, endVal, seconds, changeMode); };

	__forceinline void			SetGradual(const char* id, Float32 startVal, Float32 endVal, Float32 seconds, UInt32 changeMode = 0)
								{ SetGradual(TraitNameToID(id), startVal, endVal, seconds, changeMode); }

	__forceinline Menu*			GetParentMenu() { return ThisCall<Menu*>(0xA03C90, this); };
	TileMenu*					GetTileMenu();
	void __fastcall				PokeValue(UInt32 valueID);

	__forceinline void			HandleChange(UInt32 tilevalue) { ThisCall<void>(0xA074D0, this, tilevalue); }
	__forceinline Tile*			GetChildByID(UInt32 id) { return ThisCall<Tile*>(0xA03EB0, this, id); }; // THANKS STEWIE
	__forceinline Tile*			LookUpRectByName(const char* name);

	Tile*						AddTileFromTemplate(const char* templateName, const char* altName = nullptr);

	Tile* GetParentByID(UInt32 id)
	{
		CdeclCall(0xA044F0); // enter tile critical section

		Tile* tile = this;
		while (tile = tile->parent)
		{
			if (static_cast<UInt32>(tile->Get(kTileValue_id)) == id)
			{
				break;
			}
		}

		CdeclCall(0xA04500); // leave tile critical section
		return tile;
	}

	Tile* GetByTraitName(const char* traitName) { return CdeclCall<Tile*>(0xA08B20, this, traitName); };

	std::string GetFullPath();
};
static_assert(sizeof(Tile) == 0x38);

// 3C
class TileRect : public Tile
{
public:
	UInt32	unk38;	// 38
};
static_assert(sizeof(TileRect) == 0x3C);

// 40
class TileMenu : public TileRect
{
public:
	Menu	* menu;	// 3C - confirmed
};
static_assert(sizeof(TileMenu) == 0x40);

// 48
class TileImage : public Tile
{
public:
	Float32		flt038;		// 38
	RefNiObject unk03C;		// 3C
	RefNiObject unk040;		// 40
	UInt8		byt044;		// 44
	UInt8		fill[3];	// 45-47
};
static_assert(sizeof(TileImage) == 0x48);

// 48
class RadialTile : public TileImage
{
	enum
	{
		xOffset = kTileValue_user0,
		yOffset = kTileValue_user1,
		anglemin = kTileValue_user2,
		anglemax = kTileValue_user3,
		distanceMin = kTileValue_user4,
		distanceMax = kTileValue_user5
	};
public:

};


class TileText : public Tile
{
public:
};

class Tile3D : Tile
{
	UInt32 unk38;
	UInt32 unk3C;
	String str40;
	String maybeModelPath;
};

class TileExtra : public NiExtraData
{
public:
	TileExtra();
	~TileExtra();

	Tile* parentTile;	// 0C
	NiNode* parentNode;	// 10
};

// 1C
struct GradualSetFloat
{	//	0		From start to end in duration
	//	1		From start to end to start, in duration, perpetually
	//	2		From start to end to start, in duration, 4 times
	//	3		From start to end to start, 7 times, in duration
	//	4		From start to end in duration/6, end for duration*2/3, from end to start in duration/6
	//	5**		From start to end, in duration, perpetually (suitable for image rotation)
	enum kGradualSetFloat
	{
		StartToEnd				= 0,
		StartToEndPerpetual		= 1,
		StartToEndFourTimes		= 2,
		StartToEndSevenTimes	= 3,
		StartToEndToStart		= 4,
		StartToEndRotation		= 5
	};

	Float32		startValue;		// 00
	Float32		endValue;		// 04
	UInt32		startTimeMS;	// 08
	Float32		durationMS;		// 0C
	UInt32		valueID;		// 10
	Tile*		tile;			// 14
	UInt32		changeMode;		// 18	0-4, see 0xA081B5

	__forceinline static TList<GradualSetFloat>* QueuedGradualSetFloat() { return (TList<GradualSetFloat>*)0x11F3348; }
};