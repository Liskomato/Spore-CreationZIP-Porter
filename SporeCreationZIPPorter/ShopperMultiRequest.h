#pragma once

#include <Spore\BasicIncludes.h>
namespace Sporepedia {
	struct ShopperMultiRequest
		: public ShopperRequest
	{
	public:
		ShopperMultiRequest(IMultiShopperListener* listener = nullptr);
	};
}
