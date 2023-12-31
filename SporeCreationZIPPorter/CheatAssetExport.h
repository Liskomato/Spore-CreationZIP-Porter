#pragma once

#include <Spore\BasicIncludes.h>

const uint32_t kSupportedAssetIDs[] = {
	TypeIDs::adventure,		// Adventures 
	TypeIDs::cll,	// Cells
	TypeIDs::crt,	// Creatures
	TypeIDs::bld,	// Buildings
	TypeIDs::vcl,	// Vehicles
	TypeIDs::ufo,	// UFOs
	TypeIDs::prop	// Fixed objects, music etc.
};

const uint32_t kBakedAssetIDs[] = {
	TypeIDs::cll,
	TypeIDs::crt,
	TypeIDs::bld,
	TypeIDs::vcl,
	TypeIDs::ufo,
};

const uint32_t kKnownUnsupportedAssetIDs[] = {
	TypeIDs::flr, // Flora // Add support only when full Flora Editor in any form releases
};

class CheatAssetExport 
	: public ArgScript::ICommand
	, public Sporepedia::IShopperListener
{
public:
	CheatAssetExport();
	~CheatAssetExport();

	bool IsSupportedAsset(ResourceKey assetKey);
	bool IfAssetNeedsBake(ResourceKey assetKey);
	bool IsKnownUnsupportedAsset(ResourceKey assetKey);
	bool AssetIsBaked(ResourceKey assetKey);
	bool ExportAsset(ResourceKey assetKey, bool noBake = false);
	void ExportResource(ResourceKey assetKey);
	void ExportAttempt(ResourceKey assetKey, bool noBake = false);
	// Called when the cheat is invoked
	void ParseLine(const ArgScript::Line& line) override;
	
	void OnShopperAccept(const ResourceKey& selection) override;
	// Returns a string containing the description. If mode != DescriptionMode::Basic, return a more elaborated description
	const char* GetDescription(ArgScript::DescriptionMode mode) const override;

};

class CheatAdventureExport
	: public CheatAssetExport
{
public:
	CheatAdventureExport();
	~CheatAdventureExport();

	// Called when the cheat is invoked
	void ParseLine(const ArgScript::Line& line) override;

	// Returns a string containing the description. If mode != DescriptionMode::Basic, return a more elaborated description
	const char* GetDescription(ArgScript::DescriptionMode mode) const override;
private:
	void AdventureExportAsync(const ArgScript::Line& line);
};