#include "stdafx.h"
#include "ImportCreations.h"
#include "ZIPExport.h"
#include "ZipManager.h"
#include <iostream>
#include <windows.h>
#include <commdlg.h>
#include <wininet.h>
#include <filesystem>
#include <urlmon.h>
#pragma comment(lib, "Urlmon")
#pragma comment(lib,"Wininet.lib")

ImportCreations::ImportCreations()
{
	eastl::string16 creations = Resource::Paths::GetDirFromID(Resource::PathID::Creations);
	downloadFolder = u"Downloads/";
	downloadFolder = creations + downloadFolder;
}


ImportCreations::~ImportCreations()
{
}


void ImportCreations::ParseLine(const ArgScript::Line& line)
{
	// This method is called when your cheat is invoked.
	// Put your cheat code here.
	if (GameModeManager.GetActiveModeID() != kGGEMode) {
		LocalizedString string = LocalizedString(id("AutoDownloadPrompt"),0x7,u"Using the Creation ZIP Porter's cheats is not recommended while not in the main menu due to crashing risks.\n\nAre you sure you want to continue?");
		eastl::wstring localized;
		localized.assign_convert(string.GetText());
		int confirm = MessageBoxW(NULL,localized.c_str(), L"Creation ZIP Porter", MB_YESNO | MB_ICONWARNING);
		if (confirm == IDNO) {
			return;
		}
	}


	if (line.HasFlag("a") || line.HasFlag("assets")) {
		// Check if we can connect to Spore's servers
		bool bConnect = InternetCheckConnectionW(L"http://www.spore.com/", FLAG_ICC_FORCE_CONNECTION, 0);

		if (!bConnect) {
			App::ConsolePrintF("ERROR! Connection to Spore.com could not be established. Please make sure you are connected to the internet and that the Spore servers are online (account not required).");
			return;
		}

		// We are online, we can do this.
		Sporepedia::ShopperRequest request(this);
		request.shopperID = 0x67060110; // AssetBrowserConfig!ScenarioShopper.prop
		request.maxSelections = 1;
		Sporepedia::ShopperRequest::Show(request);
	}
	else {
		size_t numArgs;
		auto option = line.GetOptionRange("id",&numArgs,0,1);
		if (numArgs == 1) {
			ResourceKey key;
			if (GetKeyfromServerID(option[0], key)) {
				OnShopperAccept(key);
			}
		}
		else {
			auto args = line.GetArgumentsRange(&numArgs,0,1);
			std::thread asyncThread;
			if (numArgs == 1) {
				asyncThread = std::thread(&ImportCreations::ImportFromURL,this,args[0]);
			}
			else {
				asyncThread = std::thread(&ImportCreations::DownloadCreation,this);
			}
			asyncThread.detach();
		}
	}
}

void ImportCreations::OnShopperAccept(const ResourceKey& selection) {
	
	std::thread asyncThread(&ImportCreations::OnShopperAcceptAsync, this, selection);
	asyncThread.detach();
}

void ImportCreations::DownloadCreation() {
	eastl::vector<eastl::string16> filePaths;

	WCHAR file[1025] = { 0 };
	file[0] = '\0';

	OPENFILENAMEW openedFile;
	ZeroMemory(&openedFile, sizeof(openedFile));
	openedFile.lStructSize = sizeof(openedFile);
	openedFile.lpstrFilter = L"Spore Creations (*.png)\0*.png\0ZIP files (*.zip)\0*.zip\0All (*.*)\0*.*\0\0";
	openedFile.nFileOffset = 1;
	openedFile.lpstrFile = file;
	openedFile.lpstrFile[0] = '\0';
	openedFile.nMaxFile = 1025;
	openedFile.lpstrTitle = L"Load a Spore creation into the game.";
	openedFile.Flags = OFN_ALLOWMULTISELECT | OFN_EXPLORER | OFN_FILEMUSTEXIST;

	bool check = GetOpenFileNameW(&openedFile);

	if (check) {
		ResourceKey key;
		eastl::vector<ResourceKey> keys;
		bool state = 0;
		eastl::vector<bool> states;
		char16_t* str = (char16_t*)openedFile.lpstrFile;
		eastl::string16 dir = str;
		char16_t* dir_c = str;
		str += (dir.length() + 1);
		if (*str == 0) {
			if (dir.substr(dir.find_last_of(u".") + 1) == u"zip") {
				state = ZipManager.ReadZIP(dir);
			}
			else {
				state = CALL(Address(ModAPI::ChooseAddress(0x5fc240, 0x5fc3c0)), bool, Args(App::Thumbnail_cImportExport*, const char16_t*, ResourceKey&), Args(App::Thumbnail_cImportExport::Get(), dir_c, key));
			}

			states.emplace_back(state);
		}
		else {
			while (*str) {
				eastl::string16 filename = str;
				str += (filename.length() + 1);
				filePaths.emplace_back(filename);
			}
			for (eastl::string16 file : filePaths) {
				eastl::string16 fullPath = dir + u"\\" + file;
				ResourceKey fileKey;
				if (file.substr(file.find_last_of(u".") + 1) == u"zip") {
					state = ZipManager.ReadZIP(fullPath);
				}
				else {
					state = CALL(Address(ModAPI::ChooseAddress(0x5fc240, 0x5fc3c0)), bool, Args(App::Thumbnail_cImportExport*, const char16_t*, ResourceKey&), Args(App::Thumbnail_cImportExport::Get(), fullPath.c_str(), fileKey));
				}
				keys.emplace_back(fileKey);
				states.emplace_back(state);
			}
		}
		if (eastl::find(states.begin(), states.end(), true) == false) {
			App::ConsolePrintF("No creations were added. Either the filepaths were invalid, or no files read were valid.");
		}
		else {
			App::ConsolePrintF("New creations were successfully added to the Sporepedia!");
			HintManager.ShowHint(id("import-downloadcomplete"));
			Audio::PlayAudio(id("ui_attention_positive"));
		}

	}
	else {
		SporeDebugPrint("DownloadCreation failed to open window dialog.");
	}
}

void ImportCreations::OnShopperAcceptAsync(const ResourceKey& selection) {


	ResourceObjectPtr resource;
	if (selection.typeID == TypeIDs::adventure && ResourceManager.GetResource(selection, &resource)) {
		cScenarioResourcePtr scenario = object_cast<Simulator::cScenarioResource>(resource);
		ResourceKey assetKey;
		App::ConsolePrintF("Beginning download...");
		// Check avatar
		if (scenario->mAvatarAsset.mServerId != -1 && ZIPExport::GetKeyfromServerID(scenario->mAvatarAsset.mServerId, downloadFolder, assetKey)) {
			App::ConsolePrintF("Downloaded avatar successfully.");
		}
		if (scenario->mInitialPosseMembers.size() != 0) {
			int i = 1;
			for each (const auto & posseMember in scenario->mInitialPosseMembers) {
				if (posseMember.mAsset.mServerId != -1 && ZIPExport::GetKeyfromServerID(posseMember.mAsset.mServerId, downloadFolder, assetKey)) {
					App::ConsolePrintF("Downloaded posse member %d successfully.", i);

				}
				i++;
			}
		}
		if (scenario->mClasses.size() != 0) {
			int i = 1;
			for each (const auto & creation in scenario->mClasses) {
				if (creation.second.mAsset.mServerId != -1 && ZIPExport::GetKeyfromServerID(creation.second.mAsset.mServerId, downloadFolder, assetKey)) {
					App::ConsolePrintF("Downloaded scenario class %d successfully.", i);
				}
				if (creation.second.mGameplayObjectGfxOverrideAsset.mServerId != -1 && ZIPExport::GetKeyfromServerID(creation.second.mGameplayObjectGfxOverrideAsset.mServerId, downloadFolder, assetKey)) {
					App::ConsolePrintF("Downloaded GFX override 1 for scenario class %d successfully.", i);
				}
				if (creation.second.mGameplayObjectGfxOverrideAsset_Secondary.mServerId != -1 && ZIPExport::GetKeyfromServerID(creation.second.mGameplayObjectGfxOverrideAsset_Secondary.mServerId, downloadFolder, assetKey)) {
					App::ConsolePrintF("Downloaded GFX override 2 for scenario class %d successfully.", i);
				}
				i++;
			}
		}

		HintManager.ShowHint(id("import-downloadcomplete"));
		Audio::PlayAudio(id("ui_attention_positive"));

	}
}

void ImportCreations::ImportFromURL(const char* address) {
	eastl::wstring url;
	eastl::string16 path;
	ResourceKey key;
	path.assign_convert(address);
	url.assign_convert(path.c_str());

	if (path.substr(path.find_last_of(u".") + 1) == u"sporemod") {
		App::ConsolePrintF("Please install .sporemod files ONLY with Spore ModAPI Easy Installer!");
		return;
	}
	else if (path.substr(path.find_last_of(u".") + 1) == u"package") {
		App::ConsolePrintF("Please install .package files with Spore ModAPI Easy Installer. Direct loading of package files like this isn't possible.");
		return;
	}

	if (path.substr(0, 4) == u"http") {
		eastl::wstring wpath;
		wpath.assign_convert(ZipManager.GetZIPExportPath().c_str());
		if (path.substr(path.find_last_of(u".") + 1) == u"png" ||
			path.substr(path.find_last_of(u".") + 1) == u"zip") {
			wpath.append_convert(path.substr(path.find_last_of(u"/") + 1).c_str());
		}
		else {
			App::ConsolePrintF("Warning: This cheat only supports downloading either ZIP or PNG files. Other types of files are not permitted.");
			return;
		}
		HRESULT result = URLDownloadToFileW(NULL, url.c_str(), wpath.c_str(), BINDF_GETNEWESTVERSION, NULL);
		if (result != S_OK) {
			App::ConsolePrintF("WARNING: Failed to download file. Are you connected to the internet?");
			return;
		}
		path.assign_convert(wpath.c_str());
	}
	if (path.substr(path.find_last_of(u".") + 1) == u"zip") {
		if (ZipManager.ReadZIP(path)) {
			App::ConsolePrintF("ZIP file %ls has been successfully read and its contents extracted.", path.substr(path.find_last_of(u"/") + 1).c_str());
		}
		else {
			App::ConsolePrintF("ZIP file %ls was downloaded but it wasn't extracted.", path.substr(path.find_last_of(u"/") + 1).c_str());
		}
	}
	else {
		bool value = CALL(Address(ModAPI::ChooseAddress(0x5fc240, 0x5fc3c0)), bool, Args(App::Thumbnail_cImportExport*, const char16_t*, ResourceKey&), Args(App::Thumbnail_cImportExport::Get(), path.c_str(), key));
		App::ConsolePrintF("%s. Key: %#x!%#x.%#x", value ? "Success" : "Failed", key.groupID, key.instanceID, key.typeID);
	}
}

bool ImportCreations::GetKeyfromServerID(const char* id, ResourceKey& key) {
	eastl::string16 idString, webAddress;
	idString.assign_convert(id);
	eastl::string16 s1 = idString.substr(0, 3), s2 = idString.substr(3, 3), s3 = idString.substr(6, 3);
	webAddress.append_sprintf(u"http://static.spore.com/static/thumb/%ls/%ls/%ls/%ls.png", s1.c_str(), s2.c_str(), s3.c_str(), idString.c_str());
	eastl::wstring address, path;
	address.assign_convert(webAddress.c_str());

	eastl::string16 dst = downloadFolder + webAddress.substr(webAddress.find_last_of(u"/")+1);
	path.assign_convert(dst.c_str());

	HRESULT result = URLDownloadToFileW(NULL, address.c_str(), path.c_str(), BINDF_GETNEWESTVERSION, NULL);
	if (result != S_OK) {
		App::ConsolePrintF("WARNING: Failed to download file from Spore.com. Are you connected to the internet?");
		return false;
	}
	bool newItem = CALL(Address(ModAPI::ChooseAddress(0x5fc240, 0x5fc3c0)), bool, Args(App::Thumbnail_cImportExport*, const char16_t*, ResourceKey&), Args(App::Thumbnail_cImportExport::Get(),dst.c_str(), key));

	if (key != ResourceKey() && newItem) {
		App::ConsolePrintF("New creation was added to Sporepedia. Resource key: %#x!%#x.%#x",key.groupID,key.instanceID,key.typeID);
	}
	SporeDebugPrint("%ls was downloaded to %ls", webAddress.substr(webAddress.find_last_of(u"/")+1).c_str(), dst.substr(0, dst.find_last_of(u"/")));
	return true;
}

const char* ImportCreations::GetDescription(ArgScript::DescriptionMode mode) const
{
	if (mode == ArgScript::DescriptionMode::Basic) {
		return "ImportCreations creations to the game.";
	}
	else {
		return "ImportCreations: Download creations into the game\n"
			   "How to use:\n"
			   "importCreations -id <id>: Download creations from Spore.com based on the server ID you inputted. If the downloaded creation is an adventure, download its assets as well.\n"
			   "importCreations -assets: Download assets of a selected adventure in the Sporepedia.\n"
			   "importCreations <url/filepath>: Download a ZIP or PNG file from inputted web address/file path. ZIP file will be extracted and its creations imported to the game as well.\n"
			   "importCreations: Open Windows Explorer to select a creation to add.";
	}
}
