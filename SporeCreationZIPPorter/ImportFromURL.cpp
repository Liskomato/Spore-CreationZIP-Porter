#include "stdafx.h"
#include "ImportFromURL.h"

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
	eastl::string16 url;
	url.assign_convert(args[0]);
	ResourceKey key;
	bool value = CALL(Address(ModAPI::ChooseAddress(0x5fc240, 0x5fc3c0)), bool, Args(App::Thumbnail_cImportExport*, const char16_t*, ResourceKey&), Args(App::Thumbnail_cImportExport::Get(), url.c_str(), key));
	App::ConsolePrintF("%s. Key: %#x!%#x.%#x",value ? "Success" : "Failed",key.groupID,key.instanceID,key.typeID);
}

const char* ImportFromURL::GetDescription(ArgScript::DescriptionMode mode) const
{
	if (mode == ArgScript::DescriptionMode::Basic) {
		return "This cheat does something.";
	}
	else {
		return "ImportFromURL: Elaborate description of what this cheat does.";
	}
}
