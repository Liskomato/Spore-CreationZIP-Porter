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
	/// 
	/// Saves the given resource key into My Spore Creations/ZIPs/tmp
	/// @param key Resource key of the file.
	/// @param targetDir Directory where the file will be written.
	/// @param database Database where the resource is found.
	/// @returns True if success, false if failed
	static bool ExportAsset(const ResourceKey& key, eastl::string16 targetDir, Resource::Database* database);
	
	/// 
	/// Downloads the creation of the given server ID from the server to My Spore Creations/ZIPs/tmp
	/// @param id Server ID of the creation.
	/// @param dst Directory where the file will be downloaded.
	/// @returns True if success, false if failed
	static bool DownloadfromServerID(uint64_t id,eastl::string16 dst);
	/// 
	/// Assigns a resource key to a given server ID.
	/// @param id Server ID of the creation.
	/// @param dst Directory where the local file is located.
	/// @param key Resource key of the file that will be written.
	/// @returns True if success, false if failed
	static bool GetKeyfromServerID(uint64_t id,eastl::string16 dst, ResourceKey& key);
	static bool GetKeyfromServerID(uint64_t id, ResourceKey& key); // Unused function, will be looked at more once we know more about address 0x54e460 (March 2017)

	// Returns a string containing the description. If mode != DescriptionMode::Basic, return a more elaborated description
	const char* GetDescription(ArgScript::DescriptionMode mode) const override;
};

