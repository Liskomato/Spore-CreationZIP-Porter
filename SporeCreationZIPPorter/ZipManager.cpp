#include "stdafx.h"
#include "ZipManager.h"
#include "AlternativePackageLocations.h"
#include "ZipLib.h"
#include <filesystem>
#include <fstream>


cZipManager::cZipManager()
{
}


cZipManager::~cZipManager()
{
}

void cZipManager::Initialize() {
	
}
void cZipManager::Dispose() {
	ptr = nullptr;
}

void cZipManager::AddFilepath(eastl::string16 path) {
	libPaths.emplace_back(path);
}

void cZipManager::CheckFilepaths() {
	for (eastl::string16 path : libPaths) {
		for (const auto& entry : std::filesystem::directory_iterator(path.c_str())) {
			if (entry.path().u16string().find_last_of(u".png") || entry.path().u16string().find_last_of(u".PNG")) {
				ResourceKey key;
				bool success = CALL(Address(ModAPI::ChooseAddress(0x5fc240, 0x5fc3c0)), bool, Args(App::Thumbnail_cImportExport*, const char16_t*, ResourceKey&), Args(App::Thumbnail_cImportExport::Get(), entry.path().u16string().c_str(), key));
				if (success) {
					SporeDebugPrint("%ls was successfully added to Sporepedia.\nResource key: %#u!%#u.%#u", entry.path().u16string().c_str(),key.groupID,key.instanceID,key.typeID);
				}
				else {
					SporeDebugPrint("%ls was not successfully added to Sporepedia.", entry.path().u16string().c_str());
				}
			}
			else if (entry.path().u16string().find_last_of(u".zip") || entry.path().u16string().find_last_of(u".ZIP")) {
				if (!ReadZIP(entry.path().u16string().c_str(), path)) {
					SporeDebugPrint("%ls was not successfully read.", entry.path().u16string().c_str());
				}
			}
		}
	}
}

bool cZipManager::ReadZIP(eastl::string16 zip, eastl::string16 parentDirectory) {
	if (parentDirectory != u"" && zip != u"") {
		eastl::string8 zipPath;
		zipPath.assign_convert(zip.c_str());
		ZipArchive::Ptr archive = ZipFile::Open(zipPath.c_str());
		for (uint32_t i = 0; i < archive->GetEntriesCount(); i++) {
			const auto& entry = archive->GetEntry(i);
			if (entry->GetName().find_last_of(".png")) {
				
				ResourceKey key;
				std::string extractedFolder = "\\Extracted\\";
				std::string targetDir = (char*)parentDirectory.c_str() + extractedFolder + entry->GetName();
				if (entry->CanExtract()) {
					ZipFile::ExtractFile(zipPath.c_str(),entry->GetName(),targetDir.c_str());
					bool readExtracted = CALL(Address(ModAPI::ChooseAddress(0x5fc240, 0x5fc3c0)), bool, Args(App::Thumbnail_cImportExport*, const char16_t*, ResourceKey&), Args(App::Thumbnail_cImportExport::Get(), (char16_t*)targetDir.c_str(), key));
					if (readExtracted) {
						SporeDebugPrint("File entry %s within ZIP archive %s was successfully added to Sporepedia.\nResource key: %#u!%#u.%#u", entry->GetName().c_str(), zipPath.c_str(), key.groupID, key.instanceID, key.typeID);
					}
					else {
						SporeDebugPrint("File entry %ls was successfully extracted but it could not be added to Sporepedia", entry->GetName().c_str());
					}
					
				}
				else {
					SporeDebugPrint("File entry %s within ZIP archive %ls was not successfully read.", entry->GetName().c_str(), zip);
				}

			}
		}
		ZipFile::SaveAndClose(archive,zipPath.c_str());
		return true;
	}
	else {
		return false;
	}
}

cZipManager* cZipManager::Get() {
	if (ptr == nullptr) {
		ptr = new cZipManager();
		return ptr;
	}
	else {
		return ptr;
	}
}

// For internal use, do not modify.
int cZipManager::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not modify.
int cZipManager::Release()
{
	return DefaultRefCounted::Release();
}

// You can extend this function to return any other types your class implements.
void* cZipManager::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(cZipManager);
	return nullptr;
}
