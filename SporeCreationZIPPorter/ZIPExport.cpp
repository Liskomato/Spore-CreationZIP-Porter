#include "stdafx.h"
#include "ZIPExport.h"
#include <Spore\Resource\PFRecordRead.h>
#include <Spore\Resource\IResourceManager.h>
#include <Spore\Resource\IResourceFactory.h>
#include <filesystem>

ZIPExport::ZIPExport()
{
}


ZIPExport::~ZIPExport()
{
}


void ZIPExport::ParseLine(const ArgScript::Line& line)
{
	// This method is called when your cheat is invoked.
	// Put your cheat code here.
	Sporepedia::ShopperRequest request(this);
	request.maxSelections = -1;
	Sporepedia::ShopperRequest::Show(request);
}

const char* ZIPExport::GetDescription(ArgScript::DescriptionMode mode) const
{
	if (mode == ArgScript::DescriptionMode::Basic) {
		return "Export creations to ZIP files.";
	}
	else {
		return "ZIPExport: Select creations you want to share and pack them into a ZIP file found in My Spore Creations/ZIPs.";
	}
}

void ZIPExport::OnShopperAccept(const eastl::vector<ResourceKey>& selection) {
	
	eastl::string16 tmpPath = u"tmp/";
	tmpPath = ZipManager.GetZIPExportPath() + tmpPath;

	if (!std::filesystem::is_directory(tmpPath.c_str()) || !std::filesystem::exists(tmpPath.c_str())) { // Check if directory exists
		std::filesystem::create_directory(tmpPath.c_str()); // create folder
	}

	for (const ResourceKey& key : selection) 
	{
		ResourceObjectPtr resource;
		ResourceKey png = {key.instanceID, TypeIDs::png, key.groupID};
		auto package = ResourceManager.FindRecord(png);
		
		if (package != nullptr && ResourceManager.GetResource(key, &resource)) {
			
			if (key.groupID == 0x408a0000) 
			{
				cScenarioResourcePtr scenario = object_cast<Simulator::cScenarioResource>(resource);
				
				if (!ExportAsset(png,tmpPath,package)) {
					App::ConsolePrintF("Failed to export %#x.png",png.instanceID);
					continue;
				}
				else {
					SporeDebugPrint("Successfully exported %#x.png to %ls",png.instanceID,tmpPath.c_str());
				}
				
				if (scenario == nullptr) {
					App::ConsolePrintF("Failed to fetch scenario resource. No dependency creations were fetched.");
					continue;
				}

				// Vector to store all the dependency keys.
				eastl::vector<ResourceKey> cast;

				// Pushing back avatar and crew members.
				cast.push_back(scenario->mAvatarAsset.mKey);
				SporeDebugPrint("%#x!%#x.%#x", scenario->mAvatarAsset.mKey.groupID, scenario->mAvatarAsset.mKey.instanceID, scenario->mAvatarAsset.mKey.typeID);
				if (scenario->mInitialPosseMembers.size() != 0) {
					for (const auto& posseMember : scenario->mInitialPosseMembers) {
						cast.push_back(posseMember.mAsset.mKey);
						SporeDebugPrint("%#x!%#x.%#x", posseMember.mAsset.mKey.groupID, posseMember.mAsset.mKey.instanceID, posseMember.mAsset.mKey.typeID);
					}
				}

				// pushing back all of the cast.
				if (scenario->mClasses.size() != 0) {
					int i = 1;
					for (const auto& asset : scenario->mClasses) {
						cast.push_back(asset.second.mAsset.mKey);
						cast.push_back(asset.second.mGameplayObjectGfxOverrideAsset.mKey);
						cast.push_back(asset.second.mGameplayObjectGfxOverrideAsset_Secondary.mKey);
						SporeDebugPrint("Asset %d key: %#x!%#x.%#x", i,asset.second.mAsset.mKey.groupID, asset.second.mAsset.mKey.instanceID, asset.second.mAsset.mKey.typeID);
						if (auto d = ResourceManager.FindDatabase(asset.second.mAsset.mKey)) SporeDebugPrint("File found in %ls",d->GetLocation());
						SporeDebugPrint("Override key: %#x!%#x.%#x", asset.second.mGameplayObjectGfxOverrideAsset.mKey.groupID, asset.second.mGameplayObjectGfxOverrideAsset.mKey.instanceID, asset.second.mGameplayObjectGfxOverrideAsset.mKey.typeID);
						if (auto d = ResourceManager.FindDatabase(asset.second.mGameplayObjectGfxOverrideAsset.mKey)) SporeDebugPrint("File found in %ls", d->GetLocation());
						SporeDebugPrint("Override 2 key: %#x!%#x.%#x", asset.second.mGameplayObjectGfxOverrideAsset_Secondary.mKey.groupID, asset.second.mGameplayObjectGfxOverrideAsset_Secondary.mKey.instanceID, asset.second.mGameplayObjectGfxOverrideAsset_Secondary.mKey.typeID);
						if (auto d = ResourceManager.FindDatabase(asset.second.mGameplayObjectGfxOverrideAsset_Secondary.mKey)) SporeDebugPrint("File found in %ls", d->GetLocation());
						i++;
					}
				}

				for (const ResourceKey& key : cast) {
					if (key.groupID == GroupIDs::BuildingModels ||
						key.groupID == GroupIDs::CellModels ||
						key.groupID == GroupIDs::CreatureModels ||
						key.groupID == GroupIDs::VehicleModels ||
						key.groupID == GroupIDs::UfoModels) {
						png = { key.instanceID, TypeIDs::png, key.groupID };
						SporeDebugPrint("Stored key: %#x!%#x.%#x",key.groupID,key.instanceID,key.typeID);
						if (key.instanceID == 0) {
							
						}
						auto loc = ResourceManager.FindDatabase(png);
						if (loc == nullptr || !ExportAsset(png,tmpPath,loc)) {
							App::ConsolePrintF("Failed to export %#x.png", png.instanceID);
						}
						else {
							SporeDebugPrint("Successfully exported %#x.png to %ls", png.instanceID, tmpPath.c_str());
						}
					}
					else {
						continue;
					}
				}


			}
			else if (key.groupID == GroupIDs::BuildingModels ||
					 key.groupID == GroupIDs::CellModels ||
					 key.groupID == GroupIDs::CreatureModels ||
					 key.groupID == GroupIDs::VehicleModels ||
					 key.groupID == GroupIDs::UfoModels ) 
			{
				if (!ExportAsset(png, tmpPath, package)) {
					App::ConsolePrintF("Failed to export %#x.png", png.instanceID);
				}
				else {
					SporeDebugPrint("Successfully exported %#x.png to %ls", png.instanceID, tmpPath.c_str());
				}
			}
		}
		else {
			App::ConsolePrintF("Failed to get resource for %#x",key.instanceID);
		}
	}

	// Gathering the PNGs in temp folder, creating the ZIP archive and then delete the temp folder.
	
	eastl::string16 zipName;
	cAssetMetadataPtr firstSelection;
	if (Pollinator::GetMetadata(selection[0].instanceID, selection[0].groupID, firstSelection)) {
		zipName.append_sprintf(u"%ls.zip",firstSelection->GetName());
	}
	else {
		zipName.append_sprintf(u"%#x.zip",selection[0].instanceID);
	}

	eastl::string16 zipPath = ZipManager.GetZIPExportPath() + zipName;
	eastl::string8 zipPathC;
	zipPathC.assign_convert(zipPath.c_str());

	if (std::filesystem::is_empty(tmpPath.c_str())) {
		App::ConsolePrintF("ZIPExport ERROR: Temporary folder was empty after iterating through creations. Function aborted.");
		return;
	}


	for (const auto& entry : std::filesystem::directory_iterator(tmpPath.c_str())) {
		std::string entryPath = entry.path().string();
		ZipFile::AddFile(zipPathC.c_str(),entryPath);
		SporeDebugPrint("Added entry %s to ZIP file %s",entryPath.c_str(),zipPathC.c_str());
	}

	if (!std::filesystem::remove_all(tmpPath.c_str())) {
		App::ConsolePrintF("Failed to remove temporary folder. It will persist after this process has ended.");
	}

	App::ConsolePrintF("Your creations have been added to ZIP archive %ls. It can be found in %ls",zipName.c_str(),ZipManager.GetZIPExportPath().c_str());

}
void ZIPExport::OnShopperAccept(const ResourceKey& selection) {
	eastl::vector<ResourceKey> container = {selection};
	this->OnShopperAccept(container);
}

bool ZIPExport::ExportAsset(const ResourceKey& key, eastl::string16 targetDir, Resource::Database* database) {
	Resource::IRecord* record;
	if (database->OpenRecord(key, &record)) {
		record->GetStream()->SetPosition(0);
		auto size = record->GetStream()->GetSize();
		char* buffer = new char[size];
		record->GetStream()->Read(buffer, size);
		record->RecordClose();

		//cAssetMetadataPtr metadata;
		eastl::string16 fName;

		if (key == ResourceKey(key.instanceID, TypeIDs::png, 0x408a0000)) {
			fName.append_sprintf(u"zzz_0x%x.png", key.instanceID); // Making adventures load last in imports, hopefully.
		}
		else if (key.typeID == TypeIDs::png) {
			fName.append_sprintf(u"0x%x.png", key.instanceID);
		}
		//else if (Pollinator::GetMetadata(key.instanceID, key.groupID, metadata)) {
		//	fName.append_sprintf(u"%ls.%ls", metadata->GetName().c_str(),ResourceManager.GetTypenameFromType(key.typeID));
		//}
		else {
			fName.append_sprintf(u"0x%x.0x%x", key.instanceID, key.typeID);
		}
		eastl::string16 fPath = targetDir + fName;

		FileStreamPtr fstream = new IO::FileStream(fPath.c_str());

		if (fstream->Open(IO::AccessFlags::ReadWrite, IO::CD::CreateAlways)) {
			fstream->Write(buffer, size);
			fstream->Close();
			return true;
		}
		else {
			return false;
		}

	}
	else {
		return false;
	}
}