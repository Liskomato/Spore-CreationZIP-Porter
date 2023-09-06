#include "stdafx.h"
#include "CheatAssetExport.h"
#include "ZIPExport.h"
#include "PackageNameChecker.h"
#include <Spore/App/ScenarioMode.h>
#include <Spore/Simulator/cScenarioResource.h>

// Derived from GMDLExport

CheatAssetExport::CheatAssetExport()
{
}


CheatAssetExport::~CheatAssetExport()
{
}

// Unify methods?
bool CheatAssetExport::IsSupportedAsset(ResourceKey assetKey)
{
	for each (uint32_t ID in kSupportedAssetIDs)
	{
		if (ID == assetKey.typeID) {
			return true;
		}
	}
	return false;
}

bool CheatAssetExport::IfAssetNeedsBake(ResourceKey assetKey)
{
	for each (uint32_t ID in kBakedAssetIDs)
	{
		if (ID == assetKey.typeID) {
			return AssetIsBaked(assetKey);
		}
	}
	return true;
}

bool CheatAssetExport::IsKnownUnsupportedAsset(ResourceKey assetKey)
{
	for each (uint32_t ID in kKnownUnsupportedAssetIDs)
	{
		if (ID == assetKey.typeID) {
			return true;
		}
	}
	return false;
}

bool CheatAssetExport::AssetIsBaked(ResourceKey assetKey)
{
	PropertyListPtr propList;
	if (!PropManager.GetPropertyList(assetKey.instanceID, assetKey.groupID, propList))
	{
		// No model property file
		return false;
	}

	ResourceKey gmdlFileKey;
	App::Property::GetKey(propList.get(), 0x00F9EFBB, gmdlFileKey);

	Resource::IRecord* pRecord;
	auto dbpf = ResourceManager.FindDatabase(gmdlFileKey);
	if (!dbpf || !dbpf->OpenRecord(gmdlFileKey, &pRecord) || PackageNameChecker::IsDatabaseVanilla(dbpf)) {
		// No model file, or we're dealing with a vanilla resource.
		return false;
	}
	return true;
}

bool CheatAssetExport::ExportAsset(ResourceKey assetKey, bool noBake)
{
	ResourceKey* keys{};
	size_t size{};
	PropertyListPtr propList;
	if (noBake) {
		if (!PropManager.GetPropertyList(assetKey.typeID, 0x4d881742, propList)) {
			PropManager.GetPropertyList(0xcecf5fdf, 0x4d881742, propList);
		}
	}
	else if (!PropManager.GetPropertyList(assetKey.typeID, 0xda77fdac, propList)) {
		PropManager.GetPropertyList(0xcecf5fdf, 0xda77fdac, propList);
	}
	App::Property::GetArrayKey(propList.get(), 0x405F6744, size, keys);
	try {
		for (size_t i = 0; i < size; i++) {
			ExportResource({ assetKey.instanceID, keys[i].typeID, keys[i].groupID });
		}
	}
	catch (const std::exception& exception) {
		App::ConsolePrintF("Couldn't export asset: %s", exception.what());
		return false;
	}
	return true;
}

void CheatAssetExport::ExportResource(ResourceKey assetKey)
{
	Resource::IRecord* pRecord;
	auto dbpf = ResourceManager.FindDatabase(assetKey);
	if (!dbpf || !dbpf->OpenRecord(assetKey, &pRecord) || PackageNameChecker::IsDatabaseVanilla(dbpf)) {
		return;
	}
	pRecord->GetStream()->SetPosition(0);
	auto size = pRecord->GetStream()->GetSize();
	char* buffer = new char[size];
	pRecord->GetStream()->Read(buffer, size);
	pRecord->RecordClose();

	eastl::string16 path = u"";
	eastl::string16 archivePath = u"";
	App::Thumbnail_cImportExport::Get()->FolderPathFromLocale(0x06244EB0, archivePath);
	eastl::wstring directoryPath = L"";

	eastl::string16 group = PackageNameChecker::IdToString(assetKey.groupID);

	directoryPath.sprintf(L"%ls%ls/", archivePath.c_str(), group.c_str());
	CreateDirectory(directoryPath.data(), NULL);
	path.sprintf(u"%ls%ls/0x%X.%ls", archivePath.c_str(), group.c_str(), assetKey.instanceID, PackageNameChecker::IdToString(assetKey.typeID).c_str());
	FileStreamPtr outputStream = new IO::FileStream(path.c_str());
	outputStream->Open(IO::AccessFlags::ReadWrite, IO::CD::CreateAlways);
	outputStream->Write(buffer, size);
	outputStream->Close();
	delete[] buffer;
}

void CheatAssetExport::ExportAttempt(ResourceKey assetKey, bool noBake)
{
	// Localize these messages? Cheats aren't localized by the Game...
	if (IsSupportedAsset(assetKey)) {
		App::ConsolePrintF("Attempting export of 0x%X...", assetKey.instanceID);
		if (!IfAssetNeedsBake(assetKey)) {
			App::ConsolePrintF("Error: Creation isn't baked. Preview it to bake asset.");
			return;
		}

		if (ExportAsset(assetKey, noBake)) {
			App::ConsolePrintF("Success.");
		};
		return;
	}
	else if (IsKnownUnsupportedAsset(assetKey)) {
		App::ConsolePrintF("This asset type hasn't received support yet.");
		return;
	}

	App::ConsolePrintF("Error: Unknown asset.");
}


void CheatAssetExport::ParseLine(const ArgScript::Line& line)
{
	Sporepedia::ShopperRequest request(this);
	Sporepedia::ShopperRequest::Show(request);
}

void CheatAssetExport::OnShopperAccept(const ResourceKey& selection)
{
	ExportAttempt(selection);
}

const char* CheatAssetExport::GetDescription(ArgScript::DescriptionMode mode) const
{
	if (mode == ArgScript::DescriptionMode::Basic) {
		return "Exports single creation asset.";
	}
	else {
		return "Opens sporepedia and allows you to export files related to the creation asset.";
	}
}

CheatAdventureExport::CheatAdventureExport()
{
}

CheatAdventureExport::~CheatAdventureExport()
{
}

void CheatAdventureExport::ParseLine(const ArgScript::Line& line)
{
	if (!Simulator::IsScenarioMode()) {
		App::ConsolePrintF("You must be in an adventure to use this cheat.");
		return;
	}
	if (ScenarioMode.mMode != App::cScenarioMode::Mode::EditMode) {
		App::ConsolePrintF("Switch to Edit Mode to use this cheat.");
		return;
	}

	bool noBake = false;
	if (line.HasFlag("noBake")) {
		noBake = true;
	}

	cScenarioResourcePtr scenarioResource = ScenarioMode.GetResource();

	ResourceKey adventureAsset = scenarioResource->GetResourceKey();

	Resource::IRecord* pRecord;
	auto dbpf = ResourceManager.FindDatabase(adventureAsset);
	if (!dbpf || !dbpf->OpenRecord(adventureAsset, &pRecord)) {
		App::ConsolePrintF("Save adventure first before using this cheat.");
		return;
	}

	ScenarioMode.GetData()->StartHistoryEntry();

	// Instance IDs in shared adventure assets are zeroed out so we want to restore them in the package adventure
	ResourceKey replacementKey;

	if (scenarioResource->mAvatarAsset.mServerId != -1 && ZIPExport::GetKeyfromServerID(scenarioResource->mAvatarAsset.mServerId,replacementKey)) {
		scenarioResource->mAvatarAsset.mKey = replacementKey;
		scenarioResource->mAvatarAsset.mServerId = -1;
	}
	ExportAttempt(scenarioResource->mAvatarAsset.mKey,noBake); // Export avatar

	for each (auto posseMember in scenarioResource->mInitialPosseMembers) // Export team members
	{
		if (posseMember.mAsset.mServerId != -1 && ZIPExport::GetKeyfromServerID(posseMember.mAsset.mServerId,replacementKey)) {
			posseMember.mAsset.mKey = replacementKey;
			posseMember.mAsset.mServerId = -1;
		}
		ExportAttempt(posseMember.mAsset.mKey,noBake);
	}

	for each (auto paletteAsset in scenarioResource->mClasses) // Export other assets in palette
	{
		if (paletteAsset.second.mAsset.mServerId != -1 && ZIPExport::GetKeyfromServerID(paletteAsset.second.mAsset.mServerId,replacementKey)) {
			paletteAsset.second.mAsset.mKey = replacementKey;
			paletteAsset.second.mAsset.mServerId = -1;
		}
		ExportAttempt(paletteAsset.second.mAsset.mKey,noBake);

		if (paletteAsset.second.mGameplayObjectGfxOverrideAsset.mServerId != -1 && ZIPExport::GetKeyfromServerID(paletteAsset.second.mGameplayObjectGfxOverrideAsset.mServerId,replacementKey)) {
			paletteAsset.second.mGameplayObjectGfxOverrideAsset.mKey = replacementKey;
			paletteAsset.second.mGameplayObjectGfxOverrideAsset.mServerId = -1;
		}
		ExportAttempt(paletteAsset.second.mGameplayObjectGfxOverrideAsset.mKey,noBake);

		if (paletteAsset.second.mGameplayObjectGfxOverrideAsset_Secondary.mServerId != -1 && ZIPExport::GetKeyfromServerID(paletteAsset.second.mGameplayObjectGfxOverrideAsset_Secondary.mServerId,replacementKey)) {
			paletteAsset.second.mGameplayObjectGfxOverrideAsset_Secondary.mKey = replacementKey;
			paletteAsset.second.mGameplayObjectGfxOverrideAsset_Secondary.mServerId = -1;
		}
		ExportAttempt(paletteAsset.second.mGameplayObjectGfxOverrideAsset_Secondary.mKey,noBake);
	}
	ScenarioMode.GetData()->CommitHistoryEntry();

	ExportAttempt(adventureAsset,noBake);

	eastl::string16 path = u"";
	eastl::string16 archivePath = u"";
	App::Thumbnail_cImportExport::Get()->FolderPathFromLocale(0x06244EB0, archivePath);
	eastl::wstring directoryPath = L"";

	eastl::string16 group = PackageNameChecker::IdToString(adventureAsset.groupID);

	directoryPath.sprintf(L"%ls%ls/", archivePath.c_str(), group.c_str());
	CreateDirectory(directoryPath.data(), NULL);
	path.sprintf(u"%ls%ls/0x%X.0x%X", archivePath.c_str(), group.c_str(), adventureAsset.instanceID, adventureAsset.typeID);
	FileStreamPtr outputStream = new IO::FileStream(path.c_str());
	outputStream->Open(IO::AccessFlags::ReadWrite, IO::CD::CreateAlways);
	scenarioResource->Write(outputStream.get());
	outputStream->Close();

	
}

const char* CheatAdventureExport::GetDescription(ArgScript::DescriptionMode mode) const
{
	if (mode == ArgScript::DescriptionMode::Basic) {
		return "Exports creations used in adventure.";
	}
	else {
		return "Exports creations used in adventure. You need to be in an adventure to use this command.\n"
			   "-noBake: Exports the adventure without baked asset files.";
	}
}
