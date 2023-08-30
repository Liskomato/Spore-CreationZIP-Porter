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
