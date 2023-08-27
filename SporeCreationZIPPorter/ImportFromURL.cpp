#include "stdafx.h"
#include "ImportFromURL.h"
#include "ZipManager.h"
#include <filesystem>
#include <urlmon.h>
#pragma comment(lib, "Urlmon")

ImportFromURL::ImportFromURL()
{
}


ImportFromURL::~ImportFromURL()
{
}


void ImportFromURL::ParseLine(const ArgScript::Line& line)
{
	// This method is called when your cheat is invoked.
	// Put your cheat code here.
	auto args = line.GetArguments(1);
	eastl::wstring url;
	eastl::string16 path;
	ResourceKey key;
	path.assign_convert(args[0]);
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
			App::ConsolePrintF("ZIP file %ls has been successfully read and its contents extracted.",path.substr(path.find_last_of(u"/")+1).c_str());
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

const char* ImportFromURL::GetDescription(ArgScript::DescriptionMode mode) const
{
	if (mode == ArgScript::DescriptionMode::Basic) {
		return "Download content directly from web addresses.";
	}
	else {
		return "ImportFromURL: Downloads a creation from an URL address and adds it to the game.";
	}
}
