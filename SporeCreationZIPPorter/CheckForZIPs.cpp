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
