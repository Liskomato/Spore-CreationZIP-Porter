#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include <commdlg.h>
#include "DownloadCreation.h"
#include "DetourClasses.h"
#include "ZipManager.h"


DownloadCreation::DownloadCreation()
{
}


DownloadCreation::~DownloadCreation()
{
}


void DownloadCreation::ParseLine(const ArgScript::Line& line)
{
	// This method is called when your cheat is invoked.
	// Put your cheat code here.
	
	eastl::vector<eastl::string16> filePaths;

	WCHAR file[1025] = { 0 };
	file[0] = '\0';
	
	OPENFILENAMEW openedFile;
	ZeroMemory(&openedFile,sizeof(openedFile));
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
		if (eastl::find(states.begin(),states.end(),true) == false) {
			App::ConsolePrintF("No creations were added. Either the filepaths were invalid, or no files read were valid.");
		}
		else {
			App::ConsolePrintF("New creations were successfully added to the Sporepedia!"); 
		}

	}
	else {
		App::ConsolePrintF("DownloadCreation failed to open window dialog.");
	}


}

const char* DownloadCreation::GetDescription(ArgScript::DescriptionMode mode) const
{
	if (mode == ArgScript::DescriptionMode::Basic) {
		return "Downloads creations from the local file system.";
	}
	else {
		return "DownloadCreation: Opens Windows Explorer to select a creation to load into the game.";
	}
}
