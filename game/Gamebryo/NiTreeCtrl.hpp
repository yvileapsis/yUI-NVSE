#pragma once

#include <CommCtrl.h>
#include "NiAVObject.hpp"
#include "NiTMap.hpp"

struct NiTreeCtrl {
	HINSTANCE__* m_hInstance;
	HWND__* m_hParentWnd;
	HWND__* m_hTreeWnd;
	HWND__* m_hTreeView;
	HWND__* m_hCommands;
	_IMAGELIST* m_hImageList;
	NiTMap<NiAVObject*, _TREEITEM*> TreeItemMap;
	NiAVObject* m_pObject;
	const char* m_pName;

	static NiTreeCtrl* Create(NiAVObject* pObject, const char* pName);

	static void SetCallBack(void* apFunc);

	static BOOL __cdecl TreeCtrlCallback(NiAVObject* apObj, int iMessage, unsigned int uiParam1, unsigned int uiParam2);
};

ASSERT_SIZE(NiTreeCtrl, 0x30);