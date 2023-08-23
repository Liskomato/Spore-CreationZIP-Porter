#include "stdafx.h"
#include "AlternativePackageLocations.h"

namespace AlternativePackageLocations {
	

	/// We call this after receiving the DLL file's path so we can get the directory it's in.
	void SplitFileName() {
		if (libDir != u"") {

			size_t found;
			found = libDir.find_last_of(u"/\\");
			libDir = libDir.substr(0, found);
		}

	}

	void AttachDetour() {
		SetDataDirs_detour::attach(Address(ModAPI::ChooseAddress(0x687e20, 0x687bd0)));
	}
	
	
	void SetDataDirs_detour::detoured()
	{

		original_function();

		auto data_dirs = reinterpret_cast<vector<DataDirObject> *>(Address(ModAPI::ChooseAddress(0x01602c88,0x015fea00)));
		auto data_dirs_size = reinterpret_cast<int32_t*>(Address(ModAPI::ChooseAddress(0x0152f348,0x0152b348)));

		data_dirs->emplace_back(0x1, 0x0, libDir, u"");
		++(*data_dirs_size);
	}
	
	void RecordModule(HMODULE hModule) {
		module = hModule;
	}

	void Initialize() {

		WCHAR dllFilePath[512 + 1] = { 0 };

		GetModuleFileNameW(module, dllFilePath, 512);

		libDir = (char16_t*)dllFilePath;
		SplitFileName();
	}

}