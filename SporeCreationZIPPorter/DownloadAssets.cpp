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
	request.shopperID = 0x67060110; // AssetBrowserConfig!ScenarioShopper.prop
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
			int i = 0;
			for each (const auto& posseMember in scenario->mInitialPosseMembers) {
				if (posseMember.mAsset.mServerId != -1 && ZIPExport::GetKeyfromServerID(posseMember.mAsset.mServerId,downloadFolder,assetKey)) {
					SporeDebugPrint("Downloaded posse member %d successfully.",i+1);
					
				}
				i++;
			}
		}
		if (scenario->mClasses.size() != 0) {
			int i = 0;
			for each (const auto& creation in scenario->mClasses) {
				if (creation.second.mAsset.mServerId != -1 && ZIPExport::GetKeyfromServerID(creation.second.mAsset.mServerId, downloadFolder, assetKey)) {
					SporeDebugPrint("Downloaded scenario class %d successfully.", i + 1);
				}
				if (creation.second.mGameplayObjectGfxOverrideAsset.mServerId != -1 && ZIPExport::GetKeyfromServerID(creation.second.mGameplayObjectGfxOverrideAsset.mServerId, downloadFolder, assetKey)) {
					SporeDebugPrint("Downloaded GFX override 1 for scenario class %d successfully.",i+1);
				}
				if (creation.second.mGameplayObjectGfxOverrideAsset_Secondary.mServerId != -1 && ZIPExport::GetKeyfromServerID(creation.second.mGameplayObjectGfxOverrideAsset_Secondary.mServerId, downloadFolder, assetKey)) {
					SporeDebugPrint("Downloaded GFX override 2 for scenario class %d successfully.", i + 1);
				}
				i++;
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
