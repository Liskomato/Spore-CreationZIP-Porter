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
	
	vector<char16_t*> filePaths;

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
		VirtualClass vclass;
		ResourceKey key;
		vector<ResourceKey> keys;
		uint32_t state = 0;
		char16_t* str = (char16_t*)openedFile.lpstrFile;
		string16 dir = str;
		char16_t* dir_c = str;
		str += (dir.length() + 1);
		if (*str == 0) {
			state = CALL(Address(ModAPI::ChooseAddress(0x5fc240, 0x5fc3c0)), uint32_t, Args(VirtualClass, const char16_t*, ResourceKey&), Args(vclass,dir_c, key));
		}
		else {
			while (*str) {
				string16 filename = str;
				char16_t* filename_c = str;
				str += (filename.length() + 1);
				filePaths.emplace_back(filename_c);
			}
			for (const char16_t* file : filePaths) {
				char16_t* fullPath = dir_c;
				fullPath += *file;
				ResourceKey fileKey;
				state = CALL(Address(ModAPI::ChooseAddress(0x5fc240, 0x5fc3c0)), uint32_t, Args(VirtualClass,const char16_t*, ResourceKey&), Args(vclass,fullPath,fileKey));
				keys.emplace_back(fileKey);
			}
		}
		if (keys.size() != 0 || key != ResourceKey()) { 
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
