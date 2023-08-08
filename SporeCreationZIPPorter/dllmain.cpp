// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "DetourClasses.h"

void Initialize()
{
	// This method is executed when the game starts, before the user interface is shown
	// Here you can do things such as:
	//  - Add new cheats
	//  - Add new simulator classes
	//  - Add new game modes
	//  - Add new space tools
	//  - Change materials
}

member_detour(SavePNG_detour, App::Thumbnail_cImportExport, bool(Resource::ResourceObject*, RenderWare::Raster*, Resource::Database*, bool, bool)) {
	bool detoured(Resource::ResourceObject * pResource, RenderWare::Raster * pImage, Resource::Database * database,
		bool forceReplace = false, bool disableSteganography = false) {

		return original_function(this,pResource,pImage,database,forceReplace,disableSteganography);
	}
};

// ModAPI::ChooseAddress(0x563550, 0x5634b0)
member_detour(PNG_Detour01,VirtualClass,void(uint32_t*,char*)) {
	void detoured(uint32_t * p1, char* p2) {
		original_function(this,p1,p2);
	}
};

// ModAPI::ChooseAddress(0x57e870, 0x57ea30)
member_detour(PNG_Detour02, VirtualClass, void(void)) {
	void detoured() {
		original_function(this);
	}
};
/*
// ModAPI::ChooseAddress(0x580040, 0x5802f0)
// Hit when saving creation. Crashed because of function parameter mismatch.
member_detour(PNG_Detour03, VirtualClass, void(char*, int*,App::PropertyList*,int*)) {
	void detoured(char* p1, int* p2, App::PropertyList* p3, int* p4) {
		original_function(this, p1, p2, p3, p4);
	}
};
*/
// ModAPI::ChooseAddress(0x5f89c0, 0x5f8b60)
member_detour(PNG_Detour04, VirtualClass, uint32_t(int*)) {
	uint32_t detoured(int* p1) {
		return original_function(this, p1);
	}
};

// ModAPI::ChooseAddress(0x5f9830, 0x5f99b0)
member_detour(PNG_Detour05, VirtualClass, uint32_t(void*, void*,char*)) {
	uint32_t detoured(void* p1, void* p2, char* p3) {
		return original_function(this, p1, p2, p3);
	}
};

// ModAPI::ChooseAddress(0x5fa1a0, 0x5fa320)
// Hit during game launch. Perhaps instantiates the HTTP client
member_detour(PNG_Detour06, App::Thumbnail_cImportExport, char*(void)) {
	char* detoured() {
		return original_function(this);
	}
};
/*
// ModAPI::ChooseAddress(0x5fba10, 0x5fbb90)
// Called when PNGs are dragged into the game. We need to find its correct parameters.
member_detour(PNG_Detour07, VirtualClass, uint32_t(IStream*,void*,void*,void*,void*)) {
	uint32_t detoured(IStream* p1, void* p2, void* p3, void* p4, void* p5) {
		return original_function(this, p1, p2, p3, p4, p5);
	}
};
*/
// ModAPI::ChooseAddress(0x5fc240, 0x5fc5fb)
member_detour(PNG_Detour08, VirtualClass, uint32_t(const char16_t*, App::cIDGenerator*)) {
	uint32_t detoured(const char16_t* p1, App::cIDGenerator* p2) {
		return original_function(this, p1, p2);
	}
};

// ModAPI::ChooseAddress(0x615cc0, 0x6160c0)
member_detour(PNG_Detour09, VirtualClass, void()) {
	void detoured() {
		original_function(this);
	}
};

// ModAPI::ChooseAddress(0x615de0, 0x6161e0)
member_detour(PNG_Detour10, VirtualClass, uint32_t(int, int**, void**)) {
	uint32_t detoured(int p1, int** p2, void** p3) {
		return original_function(this, p1, p2, p3);
	}
};


void Dispose()
{
	// This method is called when the game is closing
}

void AttachDetours()
{
	// Call the attach() method on any detours you want to add
	// For example: cViewer_SetRenderType_detour::attach(GetAddress(cViewer, SetRenderType));
	SavePNG_detour::attach(GetAddress(App::Thumbnail_cImportExport, SavePNG));

	PNG_Detour01::attach(Address(ModAPI::ChooseAddress(0x563550, 0x5634b0)));
	PNG_Detour02::attach(Address(ModAPI::ChooseAddress(0x57e870, 0x57ea30)));
//	PNG_Detour03::attach(Address(ModAPI::ChooseAddress(0x580040, 0x5802f0)));
	PNG_Detour04::attach(Address(ModAPI::ChooseAddress(0x5f89c0, 0x5f8b60)));
	PNG_Detour05::attach(Address(ModAPI::ChooseAddress(0x5f9830, 0x5f99b0)));
	PNG_Detour06::attach(Address(ModAPI::ChooseAddress(0x5fa1a0, 0x5fa320)));
//	PNG_Detour07::attach(Address(ModAPI::ChooseAddress(0x5fba10, 0x5fbb90)));
	PNG_Detour08::attach(Address(ModAPI::ChooseAddress(0x5fc240, 0x5fc5fb)));
	PNG_Detour09::attach(Address(ModAPI::ChooseAddress(0x615cc0, 0x6160c0)));
	PNG_Detour10::attach(Address(ModAPI::ChooseAddress(0x615de0, 0x6161e0)));
}


// Generally, you don't need to touch any code here
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		ModAPI::AddPostInitFunction(Initialize);
		ModAPI::AddDisposeFunction(Dispose);

		PrepareDetours(hModule);
		AttachDetours();
		CommitDetours();
		break;

	case DLL_PROCESS_DETACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}

