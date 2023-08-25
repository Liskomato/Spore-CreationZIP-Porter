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
	
	DatabaseDirectoryFilesPtr exportDatabase = ZipManager.GetZIPExportFolder();
	eastl::string16 tmpPath = u"tmp/";
	tmpPath = ZipManager.GetZIPExportPath() + tmpPath;

	if (!std::filesystem::is_directory(tmpPath.c_str()) || !std::filesystem::exists(tmpPath.c_str())) { // Check if directory exists
		std::filesystem::create_directory(tmpPath.c_str()); // create folder
	}
	DatabaseDirectoryFilesPtr tmpDatabase = new Resource::DatabaseDirectoryFiles(tmpPath.c_str());
	tmpDatabase->AddExtensionMapping(u".png",TypeIDs::png);
	ZipArchive::Ptr archive = ZipArchive::Create();
	IResourceFactoryPtr pngFactory = ResourceManager.FindFactory(TypeIDs::png);

	for (const ResourceKey& key : selection) {
		ResourceObjectPtr resource;
		ResourceKey png = {key.instanceID, TypeIDs::png, key.groupID};
		auto package = ResourceManager.FindRecord(png);
		Resource::IRecord* record;
		if (package != nullptr && ResourceManager.GetResource(key, &resource)) {
			
			if (key.typeID == TypeIDs::adventure) {
				cScenarioResourcePtr scenario = object_cast<Simulator::cScenarioResource>(resource);
				
				png = {png.instanceID,TypeIDs::png,id("adventureImages_1~")};

				if (package->OpenRecord(png,&record)) {
					record->GetStream()->SetPosition(0);
					auto size = record->GetStream()->GetSize();
					char* buffer = new char[size];
					record->GetStream()->Read(buffer,size);
					record->RecordClose();
				}

				ResourceKey avatar = scenario->mAvatarAsset.mKey,
					posse1 = scenario->mInitialPosseMembers[0].mAsset.mKey,
					posse2 = scenario->mInitialPosseMembers[1].mAsset.mKey,
					posse3 = scenario->mInitialPosseMembers[2].mAsset.mKey;
				vector<ResourceKey&> cast;
				for (auto& asset : scenario->mClasses) {
					cast.push_back(asset.second.mAsset.mKey);
					cast.push_back(asset.second.mGameplayObjectGfxOverrideAsset.mKey);
					cast.push_back(asset.second.mGameplayObjectGfxOverrideAsset_Secondary.mKey);
				}

				ResourceKey avatarPNG = { avatar.instanceID,TypeIDs::png,avatar.groupID };


			}
		}
		else {
			SporeDebugPrint("Failed to get resource for %#x",key.instanceID);
		}
	}
}
void ZIPExport::OnShopperAccept(const ResourceKey& selection) {

}