/**
 * Main Menu class
 */
#pragma once

#include "src/widgets/widget.h"
#include "src/settings/LangConfig.h"

class TyroMenu : public wxMenuBar {
public:
	TyroMenu();
	~TyroMenu();
	void EnableEditControls(bool enable);
	void SetIdChecked(int id, bool checked);
	void SetCurrentLanguage(string lang);
private:
	LangConfig *lang_config = nullptr;
	wxMenu *fileMenu = nullptr;
	wxMenu *editMenu = nullptr;
	wxMenu *viewMenu = nullptr;
	wxMenu *langMenu = nullptr;
	wxMenu *helpMenu = nullptr;
	void SetupMainMenus();
	void SetupLangMenu();
	void EnableEntireMenu(size_t menuId, wxMenu *menu, bool enable);
};
