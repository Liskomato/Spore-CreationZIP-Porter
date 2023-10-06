#pragma once

#include <Spore\BasicIncludes.h>

class ImportCreations 
	: public ArgScript::ICommand
	, public Sporepedia::IShopperListener
{
public:
	ImportCreations();
	~ImportCreations();

	// Called when the cheat is invoked
	void ParseLine(const ArgScript::Line& line) override;
	void OnShopperAccept(const ResourceKey& selection) override;
	void DownloadCreation();
	void ImportFromURL(const char* address);
	bool GetKeyfromServerID(const char* id, ResourceKey& key);
	
	// Returns a string containing the description. If mode != DescriptionMode::Basic, return a more elaborated description
	const char* GetDescription(ArgScript::DescriptionMode mode) const override;

	eastl::string16 downloadFolder;
private:
	void OnShopperAcceptAsync(const ResourceKey& selection);
};

