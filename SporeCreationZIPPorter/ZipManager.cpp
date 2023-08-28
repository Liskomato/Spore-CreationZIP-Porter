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
	if (libPaths[0] != AlternativePackageLocations::libDir) {
		libPaths.push_back(AlternativePackageLocations::libDir);
	}
	eastl::string16 ZIPs = u"ZIPs/", creations = Resource::Paths::GetDirFromID(Resource::PathID::Creations);

	ZIPs = creations + ZIPs;
	if (!std::filesystem::is_directory(ZIPs.c_str()) || !std::filesystem::exists(ZIPs.c_str())) { // Check if directory exists
		 std::filesystem::create_directory(ZIPs.c_str()); // create folder
	}

	libPaths.push_back(ZIPs);
	ZipExportPath = ZIPs;
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
					if (!ReadZIP(entryPath)) {
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

bool cZipManager::ReadZIP(const eastl::string16& zip) {
	if (zip != u"" && zip.substr(zip.find_last_of(u".") + 1) == u"zip") {
		
		eastl::string16 creations = Resource::Paths::GetDirFromID(Resource::PathID::Creations);
		eastl::string16 fileName = zip.substr(zip.find_last_of(u"/\\")+1);

		eastl::string16 extractedFolder = u"Extracted/";
		eastl::string16 destPath = creations.c_str() + extractedFolder + fileName.substr(0,fileName.find_last_of(u".")) + u"/";

		if (!std::filesystem::is_directory(destPath.c_str()) || !std::filesystem::exists(destPath.c_str())) { // Check if destination directory exists
			std::filesystem::create_directory(destPath.c_str()); // create folder
		}

		//ZipFile::Open(zipPath.c_str())
		std::ifstream* zipFile = new std::ifstream();
		zipFile->open(zip.c_str(), std::ios::binary);

		if (!zipFile->is_open()) {
			App::ConsolePrintF("cZipManager::ReadZIP ERROR: ZIP file could not be opened. Returning false.");
			return false;
		}

		ZipArchive::Ptr archive = ZipArchive::Create(zipFile,true);

		for (uint32_t i = 0; i < archive->GetEntriesCount(); i++) 
		{
			const auto& entry = archive->GetEntry(i);
			eastl::string16 targetFile = destPath;

			if (entry->GetName().find("50") != std::string::npos ||
				entry->GetName().find("zzz_0x") != std::string::npos ||
				entry->GetName().find("0x") != std::string::npos ||
				entry->GetName().find("creation_") != std::string::npos) {
				targetFile.append_convert(entry->GetName().c_str());
			}
			else if (entry->GetName().find("_50") != std::string::npos) {
				std::string name = entry->GetName().substr(entry->GetName().find_last_of("_"));
				targetFile.append_sprintf(u"zzz%s",name.c_str());
			}
			else if (!entry->IsDirectory()) {
				targetFile.append_sprintf(u"creation_%u_%#x.png", i, id(zip.c_str()));
			}
			else {
				targetFile.append_convert(entry->GetName().c_str());
				if (!std::filesystem::is_directory(targetFile.c_str()) || !std::filesystem::exists(targetFile.c_str())) { // Check if destination directory exists
					std::filesystem::create_directory(targetFile.c_str()); // create folder
				}
			}
			if (entry->CanExtract() && targetFile.find(u".png") != eastl::string16::npos) {
					
					ResourceKey key;

					std::ofstream destFile;
					destFile.open(targetFile.c_str(), std::ios::binary | std::ios::trunc);

					std::istream* dataStream = entry->GetDecompressionStream();

					if (dataStream == nullptr)
					{
						SporeDebugPrint("File entry %d within ZIP archive %ls was not successfully read.", i, zip.c_str());
						destFile.close();
						continue;
					}

					utils::stream::copy(*dataStream, destFile);

					destFile.flush();
					destFile.close();

					//if (entry->IsDirectory() && (std::filesystem::is_directory(targetFile.c_str()) || std::filesystem::exists(targetFile.c_str()))) {
					//	//we'll deal with this after sorting out reading.
					//	for (const auto& dirEntry : std::filesystem::directory_iterator(targetFile.c_str())) {
					//		eastl::string16 entryPath = dirEntry.path().u16string().c_str();
					//		if (entryPath.substr(entryPath.find_last_of(u".") + 1) == u"png") {
					//			ResourceKey key;
					//			bool success = CALL(Address(ModAPI::ChooseAddress(0x5fc240, 0x5fc3c0)), bool, Args(App::Thumbnail_cImportExport*, const char16_t*, ResourceKey&), Args(App::Thumbnail_cImportExport::Get(), dirEntry.path().u16string().c_str(), key));
					//			if (success) {
					//				SporeDebugPrint("%ls was successfully added to Sporepedia.\nResource key: %#u!%#u.%#u", entry.path().u16string().c_str(), key.groupID, key.instanceID, key.typeID);
					//			}
					//			else {
					//				SporeDebugPrint("%ls was not successfully added to Sporepedia.", entry.path().u16string().c_str());
					//			}
					//		}
					//	}
					//	continue;
					//}


					bool readExtracted = CALL(Address(ModAPI::ChooseAddress(0x5fc240, 0x5fc3c0)), bool, Args(App::Thumbnail_cImportExport*, const char16_t*, ResourceKey&), Args(App::Thumbnail_cImportExport::Get(), targetFile.c_str(), key));
					if (readExtracted) {
						SporeDebugPrint("File entry %d within ZIP archive %ls was successfully added to Sporepedia.\nResource key: %#x!%#x.%#x", i, zip.c_str(), key.groupID, key.instanceID, key.typeID);
					}
					else {
						SporeDebugPrint("File entry %d within ZIP archive %ls was successfully extracted but it could not be added to Sporepedia. Either something went wrong, the file was invalid or it already exists in-game.", i,zip.c_str());
					}
					
			}
			else {
				SporeDebugPrint("File entry %d within ZIP archive %ls was not successfully read.", i, zip.c_str());
			}
		}
		if (!std::filesystem::remove_all(destPath.c_str())) {
			App::ConsolePrintF("Failed to remove temporary folder. It will persist after this process has ended.");
		}
		return true;
	}
	
	else {
		return false;
	}
}


eastl::string16 cZipManager::GetPath(uint32_t index) {
	return libPaths[index];
}

eastl::string16 cZipManager::GetZIPExportPath() {
	return ZipExportPath;
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
