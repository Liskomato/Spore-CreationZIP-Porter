#include "stdafx.h"
#include "SporepediaLoadListener.h"

SporepediaLoadListener::SporepediaLoadListener()
{
}


SporepediaLoadListener::~SporepediaLoadListener()
{
}

// For internal use, do not modify.
int SporepediaLoadListener::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not modify.
int SporepediaLoadListener::Release()
{
	return DefaultRefCounted::Release();
}


// The method that receives the message. The first thing you should do is checking what ID sent this message...
bool SporepediaLoadListener::HandleMessage(uint32_t messageID, void* message)
{
	// Return true if the message has been handled. Other listeners will receive the message regardless of the return value.
	return true;
}
