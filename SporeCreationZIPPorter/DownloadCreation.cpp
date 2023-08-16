#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include <commdlg.h>
#include "DownloadCreation.h"
#include "OpenFileMessage.h"


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
		
		char16_t* str = (char16_t*)openedFile.lpstrFile;
		string16 dir = str;
		str += (dir.length() + 1);
		if (*str == 0) {
			filePaths.emplace_back(dir);
		}
		else while (*str) {
			string16 filename = str;
			str += (filename.length() + 1);
			filePaths.emplace_back(filename);
		}

		OpenFileMessagePtr msg = new OpenFileMessage();
		msg->files = filePaths;
		MessageManager.MessageSend(0x24CE123,msg.get());
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
