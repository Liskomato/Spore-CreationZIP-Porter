#pragma once
#include "stdafx.h"

namespace PackageNameChecker {
	bool IsDatabaseVanilla(Resource::Database* database);
	static eastl::vector<eastl::string16> vanillaDatabases 
	{
		 u"Spore_Content",
		 u"CSA_Graphics",
		 u"Spore_EP1_Content_01",
		 u"Spore_EP1_Content_02"
	};
};