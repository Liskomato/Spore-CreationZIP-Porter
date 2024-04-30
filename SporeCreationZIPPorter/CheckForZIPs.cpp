#include "stdafx.h"
#include "ZipManager.h"
#include "CheckForZIPs.h"

CheckForZIPs::CheckForZIPs()
{
}


CheckForZIPs::~CheckForZIPs()
{
}


void CheckForZIPs::ParseLine(const ArgScript::Line& line)
{
	// This method is called when your cheat is invoked.
	// Put your cheat code here.
	if (Simulator::GetGameModeID() != kGGEMode) {
		LocalizedString string = LocalizedString(id("AutoDownloadPrompt"), 0x7, u"Using the Creation ZIP Porter's cheats is not recommended while not in the main menu due to crashing risks.\n\nAre you sure you want to continue?");
		eastl::wstring localized;
		localized.assign_convert(string.GetText());
		int confirm = MessageBoxW(NULL, localized.c_str(), L"Creation ZIP Porter", MB_YESNO | MB_ICONWARNING);
		if (confirm == IDNO) {
			return;
		}
	}
	ZipManager.CheckFilepaths();
}

const char* CheckForZIPs::GetDescription(ArgScript::DescriptionMode mode) const
{
	if (mode == ArgScript::DescriptionMode::Basic) {
		return "Scan directories for new files.";
	}
	else {
		return "CheckForZIPs: Check if there have been new files added to directories known by Creation ZIP Porter.";
	}
}
