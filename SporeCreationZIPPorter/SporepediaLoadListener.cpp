#include "stdafx.h"
#include "SporepediaLoadListener.h"
#include "ZipManager.h"

SporepediaLoadListener::SporepediaLoadListener()
{
	detouredCallback = nullptr;
	detouredCallbackParent = nullptr;
	UIPointer = new UTFWin::UILayout();
}


SporepediaLoadListener::~SporepediaLoadListener()
{
	detouredCallback = nullptr;
	detouredCallbackParent = nullptr;
	UIPointer = nullptr;
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

void SporepediaLoadListener::OnButtonPress(UTFWin::IWindow* pWindow, uint32_t buttonID) {
	if (buttonID == UTFWin::cSPUIMessageBox::ControlIDs::kButton4 && detouredCallbackParent != nullptr) {
		UIPointer->Load(ResourceKey(0x2D3D4971, 0x250FE9A2, 0x40464200), false, 0x5b598f7);
		asyncThread = std::thread(&SporepediaLoadListener::DownloadAssets,this);
		asyncThread.detach();
	}
}
// The method that receives the message. The first thing you should do is checking what ID sent this message...
bool SporepediaLoadListener::HandleMessage(uint32_t messageID, void* message)
{
	// Return true if the message has been handled. Other listeners will receive the message regardless of the return value.
	return true;
}

void SporepediaLoadListener::DownloadAssets() {
	if (detouredCallback != nullptr) {
		if (ScenarioMode.GetData() == nullptr) {
			UTFWin::cSPUIMessageBox::ShowDialog(detouredCallback, ResourceKey(0x219A97F6,TypeIDs::prop, 0xf5f6fde8));
		}
	}
}