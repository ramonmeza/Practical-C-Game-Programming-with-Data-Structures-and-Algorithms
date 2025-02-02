#include "Demo2eUI.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "KnightUtils.h"

void InGameDialogue::Create(UIPopupManager* uim)
{
	__super::Create(uim);
}

void InGameDialogue::Update(float interval)
{


	if (IsKeyPressed(KEY_BACKSPACE)) {
		mgr->Close();
	}
	if (IsKeyPressed(KEY_S)) {
		mgr->Show(mgr->GetPopup("Settings"));
	}
}

void InGameDialogue::Draw()
{
	Rectangle r;
	if (GuiWindowBox(CenterRectangle(r, 800, 600), "In Game Menu") == 1)
	{
		mgr->Close();
		return;
	}

	if (GuiButton(CenterRectangle(r, 300, 60), "Settings"))
		mgr->Show(mgr->GetPopup("Settings"));
}

void SettingsDialogue::Create(UIPopupManager* uim)
{
	__super::Create(uim);
}

void SettingsDialogue::Update(float interval)
{
	if (IsKeyPressed(KEY_BACKSPACE)) {
		mgr->Close();
	}
	if (IsKeyPressed(KEY_V)) {
		mgr->Show(mgr->GetPopup("Volume"));
	}
}

void SettingsDialogue::Draw()
{
	Rectangle r;
	if (GuiWindowBox(CenterRectangle(r, 700, 400), "Settings") == 1)
	{
		mgr->Close();
		return;
	}
	if (GuiButton(CenterRectangle(r, 300, 60), "Sound"))
		mgr->Show(mgr->GetPopup("Volume"));
}

SoundDialogue::SoundDialogue()
{
	_toggleStatus = false;
}

void SoundDialogue::Create(UIPopupManager* uim)
{
	__super::Create(uim);
}

void SoundDialogue::Update(float interval)
{
	if (IsKeyPressed(KEY_BACKSPACE)) {
		mgr->Close();
	}
}

void SoundDialogue::Draw()
{
	Rectangle r;
	
	if (GuiWindowBox(CenterRectangle(r, 500, 300), "Mute/unmute Sound") == 1)
	{
		mgr->Close();
		return;
	}
	GuiCheckBox(CenterRectangle(r, 50, 50), "Mute", &_toggleStatus);
	CenterRectangle(r, 200, 50);
	r.y += 100;
	if (GuiButton(r, "Confirm")) 
	{
		mgr->Close();
	}
}

void Demo2eUIManager::Create()
{
	//initialize global UI attributes
	GuiSetStyle(DEFAULT, TEXT_SIZE, 24);

	Menu.Create(this);
	Settings.Create(this);
	Sound.Create(this);
}

UIPopup* Demo2eUIManager::GetPopup(const char* name)
{
	if (!_stricmp(name, "Menu"))
		return &Menu;
	else if (!_stricmp(name, "Settings"))
		return &Settings;
	else if (!_stricmp(name, "Volume"))
		return &Sound;
	return NULL;
}


