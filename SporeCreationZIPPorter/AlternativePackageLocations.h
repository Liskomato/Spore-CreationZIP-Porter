#pragma once
#include "stdafx.h"
namespace AlternativePackageLocations 
{

	/// Directory of this solution's DLL file.
	static string16 libDir = u"";
	static HMODULE module;

	struct DataDirObject
	{
		DataDirObject(const uint32_t _1, const uint32_t _2, string16 _3, string16 _4)
			: enabled(_1)
			, packid(_2)
			, datadir(std::move(_3))
			, productkey(std::move(_4))
		{
		}
		uint32_t enabled;
		uint32_t packid;
		string16 datadir;
		string16 productkey;
	};

	void AttachDetour();
	void SplitFileName();

	void RecordModule(HMODULE hModule);
	void Initialize();

	static_detour(SetDataDirs_detour, void()) {
		void detoured();
	};
}