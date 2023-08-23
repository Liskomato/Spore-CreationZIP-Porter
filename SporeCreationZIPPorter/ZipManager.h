#pragma once

#include <Spore\BasicIncludes.h>
//#include "libzippp.h"

#define ZipManagerPtr intrusive_ptr<cZipManager>
#define ZipManager (*cZipManager::Get())

	class cZipManager
		: public Object
		, public DefaultRefCounted
	{
	public:
		static const uint32_t TYPE = id("cZipManager");
		static cZipManager* Get();
		void Initialize();
		void Dispose();
		void AddFilepath(string16 path);
		//void CheckFilepaths();
		//bool ReadZIP(libzippp::ZipArchive* zip, std::u16string parentDirectory);

		cZipManager(const cZipManager& obj) = delete;
		~cZipManager();

		int AddRef() override;
		int Release() override;
		void* Cast(uint32_t type) const override;
	private:
		cZipManager();
		inline static cZipManager* ptr;
		vector<string16> libPaths;

	};

