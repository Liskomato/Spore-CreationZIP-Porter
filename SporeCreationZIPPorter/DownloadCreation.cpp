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
	
	OPENFILENAMEA openedFile;
	ZeroMemory(&openedFile,sizeof(openedFile));
	openedFile.lStructSize = sizeof(LPOPENFILENAMEA);
	openedFile.lpstrFilter = "Spore Creations\0*.png\0All\0*.*\0\0";
	openedFile.nFileOffset = 1;
	openedFile.lpstrFile[0] = '\0';
	openedFile.nMaxFile = 2048;
	openedFile.lpstrTitle = "Load a Spore creation into the game.";
	openedFile.Flags = OFN_ALLOWMULTISELECT | OFN_EXPLORER | OFN_FILEMUSTEXIST;
	if (GetOpenFileNameA(&openedFile)) {
		for (int i = 0; i < openedFile.nMaxFile; i++) {
			if (openedFile.lpstrFile[i] != '\0') {
				filePaths.emplace_back(openedFile.lpstrFile[i]);
			}
		}
		OpenFileMessagePtr msg = new OpenFileMessage();
		msg->files = filePaths;
		MessageManager.MessagePost(0x24CE123,msg.get());
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
