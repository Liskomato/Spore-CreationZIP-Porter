#include "stdafx.h"
#include "ShopperMultiRequest.h"
namespace Sporepedia {
	ShopperMultiRequest::ShopperMultiRequest(IMultiShopperListener* request) 
		: ShopperRequest()
	{
		pMultiListener = request;
	}
}
