/**
 * Main Menu class
 */
#ifndef TYRO_MENU_H
#define	TYRO_MENU_H

#include "widget.h"

class TyroMenu : public wxMenuBar {
public:
	TyroMenu();
	~TyroMenu();
	void EnableEditControls(bool enable);
	void SetIdChecked(int id, bool checked);
	void SetCurrentLanguage(string lang);
private:
	wxMenu *fileMenu;
	wxMenu *editMenu;
	wxMenu *viewMenu;
	wxMenu *langMenu;
	wxMenu *helpMenu;
	void SetupMainMenus();
	void SetupLangMenu();
	void EnableEntireMenu(size_t menuId, wxMenu *menu, bool enable);
};

#endif	/* TYRO_MENU_H */

