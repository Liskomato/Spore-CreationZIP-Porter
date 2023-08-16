#pragma once

#include <Spore\BasicIncludes.h>
#include <Spore\App\FileDrop.h>

#define OpenFileMessagePtr intrusive_ptr<OpenFileMessage>

class OpenFileMessage 
	: public Object
	, public DefaultRefCounted
{
public:
	static const uint32_t TYPE = id("OpenFileMessage");
	
	OpenFileMessage();
	~OpenFileMessage();

	int AddRef() override;
	int Release() override;
	void* Cast(uint32_t type) const override;

public:
	/* 04h */	eastl::vector<eastl::string16> files;
	/* 18h */	int field_18;
	/* 1Ch */	int field_1C;
};
