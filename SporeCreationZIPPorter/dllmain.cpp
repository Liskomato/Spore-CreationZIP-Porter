// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "CheckForZIPs.h"
#include "ZIPExport.h"
#include "ImportCreations.h"
#include "CheatAssetExport.h"
#include "DetourClasses.h"
#include "AlternativePackageLocations.h"
#include "ZipManager.h"
#include "ZipLib.h"
#include <Spore/UTFWin/cSPUIMessageBox.h>
#include "SporepediaLoadListener.h"



void Initialize()
{
	// This method is executed when the game starts, before the user interface is shown
	// Here you can do things such as:
	//  - Add new cheats
	//  - Add new simulator classes
	//  - Add new game modes
	//  - Add new space tools
	//  - Change materials


	// ZIP cheats
	CheatManager.AddCheat("CheckForZIPs", new CheckForZIPs());
	CheatManager.AddCheat("ZIPExport", new ZIPExport());

	// Import cheats
	CheatManager.AddCheat("importCreations",new ImportCreations());

	// Raw export cheats
	CheatManager.AddCheat("assetExport",new CheatAssetExport());
	CheatManager.AddCheat("adventureExport", new CheatAdventureExport());

	MessageManager.AddListener(new SporepediaLoadListener(), 0xcc0bf724);

	// Initialize ZipManager
	ZipManager.Initialize();
	ZipManager.CheckFilepaths();
}
/// Detour for App::Thumbnail_cImportExport::SavePNG
member_detour(SavePNG_detour, App::Thumbnail_cImportExport, bool(Resource::ResourceObject*, RenderWare::Raster*, Resource::Database*, bool, bool)) {
	bool detoured(Resource::ResourceObject * pResource, RenderWare::Raster * pImage, Resource::Database * database,
		bool forceReplace = false, bool disableSteganography = false) {

		return original_function(this,pResource,pImage,database,forceReplace,disableSteganography);
	}
};

// ModAPI::ChooseAddress(0x5fba10, 0x5fbb90)
// Called when PNGs are dragged into the game. We need to find its correct parameters.
member_detour(PNG_Detour07, VirtualClass, uint32_t(IStream*, void*, void*)) {
	uint32_t detoured(IStream * p1, void* p2, void* p3) {
		return original_function(this, p1, p2, p3);
	}
};
/// ModAPI::ChooseAddress(0x5fc240, 0x5fc3c0)
/// Detour for App::Thumbnail_cImportExport::ImportPNG
member_detour(ImportPNG_dtour, App::Thumbnail_cImportExport, bool(const char16_t*, ResourceKey&)) {
	bool detoured(const char16_t* path, ResourceKey & key) {
		eastl::string16 sPath = path;

		if (sPath.substr(sPath.find_last_of(u".") + 1) == u"zip") {
			bool zipRead = ZipManager.ReadZIP(sPath);
			if (!zipRead) {
				App::ConsolePrintF("Creation ZIP Porter: Failed to read dropped ZIP archive in %ls",path);
			}
		}
		else if (sPath.substr(sPath.find_last_of(u".") + 1) == u"sporemod") {
			App::ConsolePrintF("Please install .sporemod files ONLY with Spore ModAPI Easy Installer!");
		}
		else if (sPath.substr(sPath.find_last_of(u".") + 1) == u"package") {
			App::ConsolePrintF("Dragging .package files directly into the game like this isn't possible. Please use the Spore ModAPI Easy Installer.");
		}

		return original_function(this, path, key);
	}
};


member_detour(cScenarioData_init_dtr, Simulator::cScenarioData, void(bool)) {

	void detoured(bool boolean) {
		
		original_function(this,boolean);
	}
};

static_detour(dialogbox_detour,bool(UTFWin::MessageBoxCallback*,const ResourceKey&)) {
	
	bool detoured(UTFWin::MessageBoxCallback* pCallback, const ResourceKey & name) {
		
		if (name.instanceID == 0x7a555cd8 || name.instanceID == 0x65d634a3) {
			SporeDebugPrint("Message box ID: %#x",name.instanceID);
		}
		return original_function(pCallback, name);
	}
};

member_detour(LoadScenarioResource_detour,Simulator::cScenarioData,Simulator::cScenarioResource*(const ResourceKey&)) {
	Simulator::cScenarioResource* detoured(const ResourceKey& key) {
		
		Simulator::cScenarioResource* scenario = original_function(this,key);
		/*ResourceKey k;
		if (scenario->mAvatarAsset.mServerId != -1 && ZIPExport::GetKeyfromServerID(scenario->mAvatarAsset.mServerId, k)) {
			SporeDebugPrint("Downloaded avatar.");
		}
		if (scenario->mInitialPosseMembers.size() > 0 && scenario->mInitialPosseMembers.size() <= 3) {
			for each (const auto & posseMember in scenario->mInitialPosseMembers) {
				if (posseMember.mAsset.mServerId != -1 && ZIPExport::GetKeyfromServerID(posseMember.mAsset.mServerId, k)) {
					SporeDebugPrint("Downloaded posse member.");
				}
			}
		}
		if (scenario->mClasses.size() > 0) {
			for each (const auto & paletteAsset in scenario->mClasses) {
				if (paletteAsset.second.mAsset.mServerId != -1 && ZIPExport::GetKeyfromServerID(paletteAsset.second.mAsset.mServerId, k))
				{
					SporeDebugPrint("Downloaded cast member.");
				}
				if (paletteAsset.second.mGameplayObjectGfxOverrideAsset.mServerId != -1 && ZIPExport::GetKeyfromServerID(paletteAsset.second.mGameplayObjectGfxOverrideAsset.mServerId, k))
				{
					SporeDebugPrint("Downloaded cast member graphics override asset #1.");
				}
				if (paletteAsset.second.mGameplayObjectGfxOverrideAsset_Secondary.mServerId != -1 && ZIPExport::GetKeyfromServerID(paletteAsset.second.mGameplayObjectGfxOverrideAsset_Secondary.mServerId, k))
				{
					SporeDebugPrint("Downloaded cast member graphics override asset #2.");
				}
			}
		}*/
		
		return scenario;
	}
};


//// ModAPI::ChooseAddress(0x563550, 0x5634b0)
//member_detour(PNG_Detour01,VirtualClass,void(uint32_t*,char*)) {
//	void detoured(uint32_t * p1, char* p2) {
//		original_function(this,p1,p2);
//	}
//};
//
//// ModAPI::ChooseAddress(0x57e870, 0x57ea30)
//member_detour(PNG_Detour02, VirtualClass, void(void)) {
//	void detoured() {
//		original_function(this);
//	}
//};
///*
//// ModAPI::ChooseAddress(0x580040, 0x5802f0)
//// Hit when saving creation. Crashed because of function parameter mismatch.
//member_detour(PNG_Detour03, VirtualClass, void(char*, int*,App::PropertyList*,int*)) {
//	void detoured(char* p1, int* p2, App::PropertyList* p3, int* p4) {
//		original_function(this, p1, p2, p3, p4);
//	}
//};
//*/
//// ModAPI::ChooseAddress(0x5f89c0, 0x5f8b60)
//member_detour(PNG_Detour04, VirtualClass, uint32_t(int*)) {
//	uint32_t detoured(int* p1) {
//		return original_function(this, p1);
//	}
//};
//
//// ModAPI::ChooseAddress(0x5f9830, 0x5f99b0)
//member_detour(PNG_Detour05, VirtualClass, uint32_t(void*, void*,char*)) {
//	uint32_t detoured(void* p1, void* p2, char* p3) {
//		return original_function(this, p1, p2, p3);
//	}
//};
//
//// ModAPI::ChooseAddress(0x5fa1a0, 0x5fa320)
//// Hit during game launch. Perhaps instantiates the HTTP client
//member_detour(PNG_Detour06, App::Thumbnail_cImportExport, char*(void)) {
//	char* detoured() {
//		return original_function(this);
//	}
//};




//
//// ModAPI::ChooseAddress(0x615cc0, 0x6160c0)
//member_detour(PNG_Detour09, VirtualClass, void()) {
//	void detoured() {
//		original_function(this);
//	}
//};
//
//// ModAPI::ChooseAddress(0x615de0, 0x6161e0)
//member_detour(PNG_Detour10, VirtualClass, uint32_t(int, int**, void**)) {
//	uint32_t detoured(int p1, int** p2, void** p3) {
//		return original_function(this, p1, p2, p3);
//	}
//};


void Dispose()
{
	// This method is called when the game is closing
	ZipManager.Dispose();
}

void AttachDetours()
{
	
	// Call the attach() method on any detours you want to add
	// For example: cViewer_SetRenderType_detour::attach(GetAddress(cViewer, SetRenderType));
	SavePNG_detour::attach(GetAddress(App::Thumbnail_cImportExport, SavePNG));
	PNG_Detour07::attach(Address(ModAPI::ChooseAddress(0x5fba10, 0x5fbb90)));
	ImportPNG_dtour::attach(Address(ModAPI::ChooseAddress(0x5fc240, 0x5fc3c0)));
	cScenarioData_init_dtr::attach(GetAddress(Simulator::cScenarioData, Initialize));
	dialogbox_detour::attach(GetAddress(UTFWin::cSPUIMessageBox,ShowDialog));
	LoadScenarioResource_detour::attach(Address(ModAPI::ChooseAddress(0xf44ec0,0xf44a90)));

	AlternativePackageLocations::AttachDetour();

	//PNG_Detour01::attach(Address(ModAPI::ChooseAddress(0x563550, 0x5634b0)));
	//PNG_Detour02::attach(Address(ModAPI::ChooseAddress(0x57e870, 0x57ea30)));
	//PNG_Detour03::attach(Address(ModAPI::ChooseAddress(0x580040, 0x5802f0)));
	//PNG_Detour04::attach(Address(ModAPI::ChooseAddress(0x5f89c0, 0x5f8b60)));
	//PNG_Detour05::attach(Address(ModAPI::ChooseAddress(0x5f9830, 0x5f99b0)));
	//PNG_Detour06::attach(Address(ModAPI::ChooseAddress(0x5fa1a0, 0x5fa320)));
	
	//PNG_Detour09::attach(Address(ModAPI::ChooseAddress(0x615cc0, 0x6160c0)));
	//PNG_Detour10::attach(Address(ModAPI::ChooseAddress(0x615de0, 0x6161e0)));

	
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
	//	ManualBreakpoint();
		AlternativePackageLocations::RecordModule(hModule);

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

