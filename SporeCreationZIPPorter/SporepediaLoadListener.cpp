#include "stdafx.h"
#include "SporepediaLoadListener.h"
#include "ZIPExport.h"

SporepediaLoadListener::SporepediaLoadListener()
{
	detouredCallback = nullptr;
	detouredCallbackParent = nullptr;
	eastl::string16 creations = Resource::Paths::GetDirFromID(Resource::PathID::Creations);
	downloadFolder = u"Downloads/";
	downloadFolder = creations + downloadFolder;
}


SporepediaLoadListener::~SporepediaLoadListener()
{
	detouredCallback = nullptr;
	detouredCallbackParent = nullptr;
}

// For internal use, do not modify.
int SporepediaLoadListener::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not modify.
int SporepediaLoadListener::Release()
{
	return DefaultRefCounted::Release();
}

void SporepediaLoadListener::OnButtonPress(UTFWin::IWindow* pWindow, uint32_t buttonID) {
	if (detouredCallback != nullptr) 
	{
		if (buttonID == UTFWin::cSPUIMessageBox::ControlIDs::kButtonOK)
		{
			detouredCallback->OnButtonPress(pWindow, UTFWin::cSPUIMessageBox::ControlIDs::kButton1);
		}
		else if (buttonID == UTFWin::cSPUIMessageBox::ControlIDs::kButton4 && detouredCallbackParent != nullptr && storedAdventureKey != ResourceKey())
		{
			std::thread asyncThread(&SporepediaLoadListener::DownloadAssets, this, storedAdventureKey);
			asyncThread.detach();
			detouredCallback->OnButtonPress(pWindow, UTFWin::cSPUIMessageBox::ControlIDs::kButton1);
			UTFWin::cSPUIMessageBox::ShowDialog(this, ResourceKey(id("DownloadStarted"), TypeIDs::prop, 0xf5f6fde8));

		}
		else
		{
			detouredCallback->OnButtonPress(pWindow, buttonID);
		}
	}
}
// The method that receives the message. The first thing you should do is checking what ID sent this message...
bool SporepediaLoadListener::HandleMessage(uint32_t messageID, void* message)
{
	// Return true if the message has been handled. Other listeners will receive the message regardless of the return value.
	return true;
}
/// 
/// Code for this was almost one to one copied from ImportCreations, with small differences.
void SporepediaLoadListener::DownloadAssets(const ResourceKey& selection) {
	ResourceObjectPtr resource;
	if (selection.typeID == TypeIDs::adventure && ResourceManager.GetResource(selection, &resource))
	{
		cScenarioResourcePtr scenario = object_cast<Simulator::cScenarioResource>(resource);
		ResourceKey assetKey;
		App::ConsolePrintF("Beginning download...");
		// Check avatar
		if (scenario->mAvatarAsset.mServerId != -1 && ZIPExport::GetKeyfromServerID(scenario->mAvatarAsset.mServerId, downloadFolder, assetKey)) {
			App::ConsolePrintF("Downloaded avatar successfully.");
		}
		if (scenario->mInitialPosseMembers.size() != 0) {
			int i = 1;
			for each (const auto & posseMember in scenario->mInitialPosseMembers) {
				if (posseMember.mAsset.mServerId != -1 && ZIPExport::GetKeyfromServerID(posseMember.mAsset.mServerId, downloadFolder, assetKey)) {
					App::ConsolePrintF("Downloaded posse member %d successfully.", i);

				}
				i++;
			}
		}
		if (scenario->mClasses.size() != 0) {
			int i = 1;
			for each (const auto & creation in scenario->mClasses) {
				if (creation.second.mAsset.mServerId != -1 && ZIPExport::GetKeyfromServerID(creation.second.mAsset.mServerId, downloadFolder, assetKey)) {
					App::ConsolePrintF("Downloaded scenario class %d successfully.", i);
				}
				if (creation.second.mGameplayObjectGfxOverrideAsset.mServerId != -1 && ZIPExport::GetKeyfromServerID(creation.second.mGameplayObjectGfxOverrideAsset.mServerId, downloadFolder, assetKey)) {
					App::ConsolePrintF("Downloaded GFX override 1 for scenario class %d successfully.", i);
				}
				if (creation.second.mGameplayObjectGfxOverrideAsset_Secondary.mServerId != -1 && ZIPExport::GetKeyfromServerID(creation.second.mGameplayObjectGfxOverrideAsset_Secondary.mServerId, downloadFolder, assetKey)) {
					App::ConsolePrintF("Downloaded GFX override 2 for scenario class %d successfully.", i);
				}
				i++;
			}
		}
		if (detouredCallback != nullptr) {
			UTFWin::cSPUIMessageBox::ShowDialog(this, ResourceKey(id("DownloadFinished"), TypeIDs::prop, 0xf5f6fde8));
		}
		else {
			HintManager.ShowHint(id("import-downloadcomplete"));
		}
		Audio::PlayAudio(id("ui_attention_positive"));
		
	}
	else {
		if (detouredCallback != nullptr) {
			UTFWin::cSPUIMessageBox::ShowDialog(this, ResourceKey(0x219A97F6, TypeIDs::prop, 0xf5f6fde8));
		}
		Audio::PlayAudio(id("ui_attention_negative"));
	}
}
