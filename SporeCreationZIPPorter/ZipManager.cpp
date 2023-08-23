#include "stdafx.h"
#include "ZipManager.h"
#include "AlternativePackageLocations.h"
#include "libzippp.h"
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

void cZipManager::AddFilepath(string16 path) {
	libPaths.emplace_back(path);
}

void cZipManager::CheckFilepaths() {
	for (string16 path : libPaths) {
		std::u16string std_path = path.c_str();
		for (const auto& entry : std::filesystem::directory_iterator(std_path)) {
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
				libzippp::ZipArchive* archive = new libzippp::ZipArchive(entry.path().string());
				if (!ReadZIP(archive, std_path)) {
					SporeDebugPrint("%ls was not successfully read.", entry.path().u16string().c_str());
				}
				delete archive;
			}
		}
	}
}

bool cZipManager::ReadZIP(libzippp::ZipArchive* zip, std::u16string parentDirectory) {
	if (parentDirectory != u"" && zip != nullptr && zip->open()) {

		for (const auto& entry : zip->getEntries()) {
			if (entry.isFile() && entry.getName().find_last_of(".png")) {
				std::ofstream output;
				ResourceKey key;
				std::u16string targetDir = parentDirectory + u"\\Extracted\\" + (char16_t*)entry.getName().c_str();
				output.open(targetDir, std::ofstream::out);
				if (entry.readContent(output) != LIBZIPPP_OK) {
					SporeDebugPrint("File entry %ls within ZIP archive %ls was not successfully read.", entry.getName().c_str(), zip->getPath().c_str());
					output.close();
				}
				else {
					output.close();
					bool readExtracted = CALL(Address(ModAPI::ChooseAddress(0x5fc240, 0x5fc3c0)), bool, Args(App::Thumbnail_cImportExport*, const char16_t*, ResourceKey&), Args(App::Thumbnail_cImportExport::Get(), targetDir.c_str(), key));
					if (readExtracted) {
						SporeDebugPrint("File entry %ls within ZIP archive %s was successfully added to Sporepedia.\nResource key: %#u!%#u.%#u",entry.getName().c_str(), zip->getPath().c_str(), key.groupID, key.instanceID, key.typeID);
					}
					else {
						SporeDebugPrint("File entry %ls was successfully extracted but it could not be added to Sporepedia",entry.getName().c_str());
					}
				}

			}
		}
		zip->close();
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
