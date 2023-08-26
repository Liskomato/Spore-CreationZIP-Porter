#pragma once

#include <Spore\BasicIncludes.h>
#include "ZipLib.h"
#include "ZipManager.h"

class ZIPExport 
	: public ArgScript::ICommand
	, public Sporepedia::IShopperListener
	, public Sporepedia::IMultiShopperListener
{
public:
	ZIPExport();
	~ZIPExport();

	// Called when the cheat is invoked
	void ParseLine(const ArgScript::Line& line) override;
	
	void OnShopperAccept(const ResourceKey& selection) override;
	void OnShopperAccept(const eastl::vector<ResourceKey>& selection) override;
	static bool ExportAsset(const ResourceKey& key, eastl::string16 targetDir, Resource::Database* database);

	// Returns a string containing the description. If mode != DescriptionMode::Basic, return a more elaborated description
	const char* GetDescription(ArgScript::DescriptionMode mode) const override;
};

