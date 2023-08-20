#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include <commdlg.h>
#include "DownloadCreation.h"
#include "OpenFileMessage.h"
#include "DetourClasses.h"


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
	
	vector<string16> filePaths;

	WCHAR file[1025] = { 0 };
	file[0] = '\0';
	
	OPENFILENAMEW openedFile;
	ZeroMemory(&openedFile,sizeof(openedFile));
	openedFile.lStructSize = sizeof(openedFile);
	openedFile.lpstrFilter = L"Spore Creations (*.png)\0*.png\0All (*.*)\0*.*\0\0";
	openedFile.nFileOffset = 1;
	openedFile.lpstrFile = file;
	openedFile.lpstrFile[0] = '\0';
	openedFile.nMaxFile = 1025;
	openedFile.lpstrTitle = L"Load a Spore creation into the game.";
	openedFile.Flags = OFN_ALLOWMULTISELECT | OFN_EXPLORER | OFN_FILEMUSTEXIST;
	
	bool check = GetOpenFileNameW(&openedFile);
	
	if (check) {
		ResourceKey key;
		vector<ResourceKey> keys;
		bool state = 0;
		vector<bool> states;
		char16_t* str = (char16_t*)openedFile.lpstrFile;
		string16 dir = str;
		char16_t* dir_c = str;
		str += (dir.length() + 1);
		if (*str == 0) {
			state = CALL(Address(ModAPI::ChooseAddress(0x5fc240, 0x5fc3c0)), bool, Args(App::Thumbnail_cImportExport*, const char16_t*, ResourceKey&), Args(App::Thumbnail_cImportExport::Get(), dir_c, key));
			states.emplace_back(state);
		}
		else {
			while (*str) {
				string16 filename = str;
				str += (filename.length() + 1);
				filePaths.emplace_back(filename);
			}
			for (string16 file : filePaths) {
				string16 fullPath = dir + u"\\" + file;
				ResourceKey fileKey;
				state = CALL(Address(ModAPI::ChooseAddress(0x5fc240, 0x5fc3c0)), bool, Args(App::Thumbnail_cImportExport*,const char16_t*, ResourceKey&), Args(App::Thumbnail_cImportExport::Get(), fullPath.c_str(), fileKey));
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
		return "This cheat does something.";
	}
	else {
		return "DownloadCreation: Elaborate description of what this cheat does.";
	}
}
