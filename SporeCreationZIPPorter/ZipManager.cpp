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

void cZipManager::AddFilepath(const eastl::string16& path) {
	libPaths.push_back(path);
}

void cZipManager::CheckFilepaths() {
	for (eastl::string16 path : libPaths) {
		if (path != u"") {
			for (const auto& entry : std::filesystem::directory_iterator(path.c_str())) {
				eastl::string16 entryPath = entry.path().u16string().c_str();
				if (entryPath.substr(entryPath.find_last_of(u".") + 1) == u"png") {
					ResourceKey key;
					bool success = CALL(Address(ModAPI::ChooseAddress(0x5fc240, 0x5fc3c0)), bool, Args(App::Thumbnail_cImportExport*, const char16_t*, ResourceKey&), Args(App::Thumbnail_cImportExport::Get(), entry.path().u16string().c_str(), key));
					if (success) {
						SporeDebugPrint("%ls was successfully added to Sporepedia.\nResource key: %#u!%#u.%#u", entry.path().u16string().c_str(), key.groupID, key.instanceID, key.typeID);
					}
					else {
						SporeDebugPrint("%ls was not successfully added to Sporepedia.", entry.path().u16string().c_str());
					}
				}
				else if (entryPath.substr(entryPath.find_last_of(u".") + 1) == u"zip") {
					if (!ReadZIP(entryPath, path)) {
						SporeDebugPrint("%ls was not successfully read.", entry.path().u16string().c_str());
					}
				}
			}
		}
		else {
			SporeDebugPrint("Creation ZIP Porter: Failed to read filepath.");
		}
	}
}

bool cZipManager::ReadZIP(const eastl::string16& zip, const eastl::string16& parentDirectory) {
	if (parentDirectory != u"" && zip != u"") {
		eastl::string8 zipPath;
		zipPath.assign_convert(zip.c_str());

		eastl::string8 parent;
		parent.assign_convert(parentDirectory.c_str());

		eastl::string8 creations;
		creations.assign_convert(Resource::Paths::GetDirFromID(Resource::PathID::Creations));


		std::string extractedFolder = "Extracted/";
		std::string destPath = creations.c_str() + extractedFolder;

		if (!std::filesystem::is_directory(destPath) || !std::filesystem::exists(destPath)) { // Check if destination directory exists
			std::filesystem::create_directory(destPath); // create folder
		}

		ZipArchive::Ptr archive = ZipFile::Open(zipPath.c_str());
		for (uint32_t i = 0; i < archive->GetEntriesCount(); i++) {
			const auto& entry = archive->GetEntry(i);
			if (entry->GetName().substr(entry->GetName().find_last_of(".") + 1) == "png") {
				
				ResourceKey key;
				std::string targetFile = destPath + entry->GetName();
				
				if (entry->CanExtract()) {
					
					std::ofstream destFile;
					destFile.open(targetFile, std::ios::binary | std::ios::trunc);

					std::istream* dataStream = entry->GetDecompressionStream();

					if (dataStream == nullptr)
					{
						SporeDebugPrint("File entry %s within ZIP archive %ls was not successfully read.", entry->GetName().c_str(), zip);
						destFile.close();
						continue;
					}

					utils::stream::copy(*dataStream, destFile);

					destFile.flush();
					destFile.close();

					eastl::string16 file;
					file.assign_convert(targetFile);

					bool readExtracted = CALL(Address(ModAPI::ChooseAddress(0x5fc240, 0x5fc3c0)), bool, Args(App::Thumbnail_cImportExport*, const char16_t*, ResourceKey&), Args(App::Thumbnail_cImportExport::Get(), file.c_str(), key));
					if (readExtracted) {
						SporeDebugPrint("File entry %s within ZIP archive %s was successfully added to Sporepedia.\nResource key: %#x!%#x.%#x", entry->GetName().c_str(), zipPath.c_str(), key.groupID, key.instanceID, key.typeID);
					}
					else {
						SporeDebugPrint("File entry %s was successfully extracted but it could not be added to Sporepedia. Either something went wrong, the file was invalid or it already exists in-game.", entry->GetName().c_str());
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
