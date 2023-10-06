#include "stdafx.h"
#include "PackageNameChecker.h"


bool PackageNameChecker::IsDatabaseVanilla(Resource::Database* database) {
	eastl::string16 databaseLocation = database->GetLocation();
	PropertyListPtr resource;
	size_t size = 512;
	eastl::string16* databases = new eastl::string16[size];
	if (PropManager.GetPropertyList(0xe1547e20, 0x863ef776, resource) &&
		App::Property().GetArrayString16(resource.get(), 0xe1547e20,size,databases)) {
		for (int i = 0; i < size; i++) {
			if (databaseLocation.find(databases[i]) != eastl::string16::npos) {
				return true;
			}
		}

	}
	for each (const auto & packageName in vanillaDatabases) {
		if (databaseLocation.find(packageName) != eastl::string16::npos) {
			return true;
		}
	}
	return false;
}

eastl::string16 PackageNameChecker::IdToString(uint32_t id) {
	PropertyListPtr resource;
	eastl::string16 idString = u"";
	if (PropManager.GetPropertyList(0xbcafbdd4, 0x863ef776,resource) &&
		App::Property().GetString16(resource.get(),id,idString)) {
		return idString;
	}
	
	for each (const auto& key in idMap) {
		if (key.first == id) {
			return key.second;
		}
	}
	
	idString.append_sprintf(u"0x%X",id);
	return idString;
}