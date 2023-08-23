#pragma once

#include "stdafx.h"
#include "ZipLib\ZipArchive.h"
#include "AlternativePackageLocations.h"

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
		void AddFilepath(const eastl::string16& path);
		void CheckFilepaths();
		bool ReadZIP(const eastl::string16& zip, const eastl::string16& parentDirectory);

		cZipManager(const cZipManager& obj) = delete;
		~cZipManager();

		int AddRef() override;
		int Release() override;
		void* Cast(uint32_t type) const override;
	private:
		cZipManager();
		inline static cZipManager* ptr;
		eastl::vector<eastl::string16> libPaths = {AlternativePackageLocations::libDir};

	};

