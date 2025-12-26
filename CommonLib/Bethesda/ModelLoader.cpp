#include "ModelLoader.hpp"

ModelLoader* ModelLoader::GetSingleton() {
	return *(ModelLoader**)0x011C3B3C;
}

TESModel* ModelLoader::GetModelForBoundObject(const TESBoundObject* apObject, const TESObjectREFR* apRef) {
	return ThisCall<TESModel*>(0x446A60, this, apObject, apRef);
}