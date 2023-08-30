#include "stdafx.h"
#include "ZIPExport.h"
#include "DownloadAssets.h"
#include <wininet.h>
#pragma comment(lib,"Wininet.lib")

DownloadAssets::DownloadAssets()
{
	eastl::string16 creations = Resource::Paths::GetDirFromID(Resource::PathID::Creations);
	downloadFolder = u"Downloads/";
	downloadFolder = creations + downloadFolder;
}


DownloadAssets::~DownloadAssets()
{
}


void DownloadAssets::ParseLine(const ArgScript::Line& line)
{
	// This method is called when your cheat is invoked.
	// Put your cheat code here.

	// Check if we can connect to Spore's servers
	bool bConnect = InternetCheckConnectionW(L"http://www.spore.com/", FLAG_ICC_FORCE_CONNECTION, 0);

	if (!bConnect) {
		App::ConsolePrintF("ERROR! Connection to Spore.com could not be established. Please make sure you are connected to the internet and that the Spore servers are online (account not required).");
		return;
	}

	// We are online, we can do this.
	Sporepedia::ShopperRequest request(this);
	request.shopperID = 0x14e497ed; // AssetBrowserFilter!ScenarioPlayable.prop
	request.maxSelections = 1;
	Sporepedia::ShopperRequest::Show(request);
}

void DownloadAssets::OnShopperAccept(const ResourceKey& selection) {


	ResourceObjectPtr resource;
	if (selection.typeID == TypeIDs::adventure && ResourceManager.GetResource(selection, &resource)) {
		cScenarioResourcePtr scenario = object_cast<Simulator::cScenarioResource>(resource);
		ResourceKey assetKey;
		// Check avatar
		if (scenario->mAvatarAsset.mServerId != -1 && ZIPExport::GetKeyfromServerID(scenario->mAvatarAsset.mServerId,downloadFolder,assetKey)) {
			SporeDebugPrint("Downloaded avatar successfully.");
		}
		if (scenario->mInitialPosseMembers.size() != 0) {
			for (uint32_t i = 0; i < scenario->mInitialPosseMembers.size(); i++) {
				if (scenario->mInitialPosseMembers[i].mAsset.mServerId != -1 && ZIPExport::GetKeyfromServerID(scenario->mInitialPosseMembers[i].mAsset.mServerId,downloadFolder,assetKey)) {
					SporeDebugPrint("Downloaded posse member %d successfully.",i+1);
				}
			}
		}
		if (scenario->mClasses.size() != 0) {
			for (uint32_t i = 0; i < scenario->mClasses.size(); i++) {
				if (scenario->mClasses[i].mAsset.mServerId != -1 && ZIPExport::GetKeyfromServerID(scenario->mClasses[i].mAsset.mServerId, downloadFolder, assetKey)) {
					SporeDebugPrint("Downloaded scenario class %d successfully.", i + 1);
				}
				if (scenario->mClasses[i].mGameplayObjectGfxOverrideAsset.mServerId != -1 && ZIPExport::GetKeyfromServerID(scenario->mClasses[i].mGameplayObjectGfxOverrideAsset.mServerId, downloadFolder, assetKey)) {
					SporeDebugPrint("Downloaded GFX override 1 for scenario class %d successfully.",i+1);
				}
				if (scenario->mClasses[i].mGameplayObjectGfxOverrideAsset_Secondary.mServerId != -1 && ZIPExport::GetKeyfromServerID(scenario->mClasses[i].mGameplayObjectGfxOverrideAsset_Secondary.mServerId, downloadFolder, assetKey)) {
					SporeDebugPrint("Downloaded GFX override 2 for scenario class %d successfully.", i + 1);
				}
			}
		}


	}
}

const char* DownloadAssets::GetDescription(ArgScript::DescriptionMode mode) const
{
	if (mode == ArgScript::DescriptionMode::Basic) {
		return "This cheat does something.";
	}
	else {
		return "DownloadAssets: Elaborate description of what this cheat does.";
	}
}
