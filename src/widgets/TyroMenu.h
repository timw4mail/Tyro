/* 
 * File:   TyroMenu.h
 * Author: twarren
 *
 * Created on May 7, 2015, 3:10 PM
 */

#ifndef TYRO_MENU_H
#define	TYRO_MENU_H

class TyroMenu : public wxMenuBar {
public:
	TyroMenu();
	~TyroMenu();
	void EnableEditControls(bool enable);
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

