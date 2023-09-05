#pragma once
#include "stdafx.h"

namespace PackageNameChecker {
	bool IsDatabaseVanilla(Resource::Database* database);
	eastl::string16 IdToString(uint32_t id);
	static eastl::vector<eastl::string16> vanillaDatabases 
	{
		 u"Spore_Content",
		 u"CSA_Graphics",
		 u"Spore_EP1_Content_01",
		 u"Spore_EP1_Content_02",
		 u"Spore_EP1_Data"
	};
	static eastl::hash_map<uint32_t, eastl::string16> idMap {
		// Type IDs
		{TypeIDs::bem,u"bem"},
		{TypeIDs::bld,u"bld"},
		{TypeIDs::cll,u"cll"},
		{TypeIDs::crt,u"crt"},
		{TypeIDs::dds,u"dds"},
		{TypeIDs::effdir,u"effdir"},
		{TypeIDs::flr,u"flr"},
		{TypeIDs::gif,u"gif"},
		{TypeIDs::gmdl,u"gmdl"},
		{TypeIDs::png,u"png"},
		{TypeIDs::pollen_metadata,u"pollen_metadata"},
		{TypeIDs::prop,u"prop"},
		{TypeIDs::raster,u"raster"},
		{TypeIDs::rw4,u"rw4"},
		{TypeIDs::smt,u"smt"},
		{TypeIDs::tga,u"tga"},
		{TypeIDs::ufo,u"ufo"},
		{TypeIDs::vcl,u"vcl"},
		{0x01A527DB,u"snr"},
		{0x02B9F662,u"soundProp"},
		{0x02D5C9AF,u"summary"},
		{0x02D5C9B0,u"summary_pill"},
		// Group IDs
		{0x00000000,u"animations~"},
		{0x021407EE,u"audio~"},
		{0x65928944,u"PlannerThumbnails"},
		{0xD87454E6,u"PaletteItems"},
		{0x02E9C426,u"definitions~"},
		{0x9430ADD7,u"CivicObjects"},
		{0x441BBD49,u"ep1_adventuremodels~"},
		{0x408A0000,u"adventureImages_1~"},
		{0x408A0001,u"adventureImages_2~"},
		{0x408A0002,u"adventureImages_3~"},
		{0x408A0003,u"adventureImages_4~"},
		{0x408A0004,u"adventureImages_5~"},
		{0x40636200,u"buildingModelsBLD~"},
		{0x40636201,u"buildingModels_LOD1~"},
		{0x40636202,u"buildingModels_LOD2~"},
		{0x40616200,u"cellModelsCLL~"},
		{0x41626200,u"creature+baby_editorModel~"},
		{0x40627101,u"creatureModels_LOD1~"},
		{0x40626202,u"creatureModels_LOD2~"},
		{0x40626203,u"creatureModels_LOD3~"},
		{0x40627100,u"creature_editorModelHi~"},
		{0x40626200,u"creature_editorModel~"},
		{0x40666200,u"floraModelsFLR~"},
		{0x011AC19C,u"summary_pills~"},
		{0x40656200,u"ufoModelsUFO~"},
		{0x40656201,u"ufoModels_LOD1~"},
		{0x40656202,u"ufoModels_LOD2~"},
		{0x40656203,u"ufoModels_LOD3~"},
		{0x40646200,u"vehiclesModelsVCL~"},
		{0x40646201,u"vehicleModels_LOD1~"},
		{0x40646202,u"vehicleModels_LOD2~"},
		{0x40646203,u"vehicleModels_LOD3~"}
	};
};