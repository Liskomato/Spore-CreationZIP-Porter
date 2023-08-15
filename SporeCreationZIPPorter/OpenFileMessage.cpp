#include "stdafx.h"
#include "OpenFileMessage.h"

OpenFileMessage::OpenFileMessage()
{
}


OpenFileMessage::~OpenFileMessage()
{
}

// For internal use, do not modify.
int OpenFileMessage::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not modify.
int OpenFileMessage::Release()
{
	return DefaultRefCounted::Release();
}

// You can extend this function to return any other types your class implements.
void* OpenFileMessage::Cast(uint32_t type) const
{
	CLASS_CAST(Object);
	CLASS_CAST(OpenFileMessage);
	return nullptr;
}
