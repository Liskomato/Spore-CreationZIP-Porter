#pragma once

#include <Spore\BasicIncludes.h>

#define OpenFileMessagePtr intrusive_ptr<OpenFileMessage>

class OpenFileMessage 
	: public Object
	, public App::FileDropMessage
	, public App::IMessageRC
	, public DefaultRefCounted
{
public:
	static const uint32_t TYPE = id("OpenFileMessage");
	
	OpenFileMessage();
	~OpenFileMessage();

	int AddRef() override;
	int Release() override;
	void* Cast(uint32_t type) const override;
};
