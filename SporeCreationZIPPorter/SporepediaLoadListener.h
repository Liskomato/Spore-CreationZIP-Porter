#pragma once

#include <Spore\BasicIncludes.h>
#include <Spore/UTFWin/cSPUIMessageBox.h>
#include <thread>

#define SporepediaLoadListenerPtr intrusive_ptr<SporepediaLoadListener>

class LoadParentClass
{
	char padding[0x10];
	UTFWin::MessageBoxCallback* pCallback;
};

class DummyClass {};

class SporepediaLoadListener 
	: public App::IMessageListener
	, public UTFWin::MessageBoxCallback
	, public DefaultRefCounted
{
public:
	static const uint32_t TYPE = id("SporepediaLoadListener");

	UTFWin::MessageBoxCallback* detouredCallback;
	LoadParentClass* detouredCallbackParent;
	App::cScenarioMode* scenario;
	ResourceKey storedAdventureKey;

	SporepediaLoadListener();
	~SporepediaLoadListener();

	int AddRef() override;
	int Release() override;
	
	void OnButtonPress(UTFWin::IWindow* pWindow, uint32_t buttonID) override;

	// This is the function you have to implement, called when a message you registered to is sent.
	bool HandleMessage(uint32_t messageID, void* message) override;

	void DownloadAssets(const ResourceKey& selection);
};

