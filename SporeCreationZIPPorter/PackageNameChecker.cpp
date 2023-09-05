#include "stdafx.h"
#include "PackageNameChecker.h"


bool PackageNameChecker::IsDatabaseVanilla(Resource::Database* database) {
	eastl::string16 databaseLocation = database->GetLocation();
	for each (const auto & packageName in vanillaDatabases) {
		if (databaseLocation.find(packageName) != eastl::string16::npos) {
			return true;
		}
	}
	return false;
}

eastl::string16 PackageNameChecker::IdToString(uint32_t id) {
	for each (const auto& key in idMap) {
		if (key.first == id) {
			return key.second;
		}
	}
	eastl::string16 idString = u"";
	idString.append_sprintf(u"0x%8X",id);
	return idString;
}