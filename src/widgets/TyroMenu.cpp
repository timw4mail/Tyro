#include "widget.h"
#include "../settings/LangConfig.h"

static LangConfig *lang_config;

TyroMenu::TyroMenu()
{
	fileMenu = new wxMenu();
	editMenu = new wxMenu();
	viewMenu = new wxMenu();
	langMenu = new wxMenu();
	helpMenu = new wxMenu();

	lang_config = new LangConfig();

	this->SetupMainMenus();
	this->SetupLangMenu();
	
	// Add the menus to the menubar
	this->Insert(myFILE_MENU, fileMenu, "&File");
	this->Insert(myEDIT_MENU, editMenu, "&Edit");
	this->Insert(myVIEW_MENU, viewMenu, "&View");
	this->Insert(myLANG_MENU, langMenu, "&Language");
	this->Insert(myHELP_MENU, helpMenu, "&Help");
}

TyroMenu::~TyroMenu()
{
	wxLogDebug("TyroMenu Destructor Called.");
}

void TyroMenu::SetupMainMenus()
{
	// Add items to top-level menus
	fileMenu->Append(wxID_NEW, "&New\tCtrl+N", "Create a new file");
	fileMenu->AppendSeparator();
	fileMenu->Append(wxID_OPEN, "&Open\tCtrl+O", "Opens an existing file");
	fileMenu->Append(wxID_SAVE, "&Save\tCtrl+S", "Save the content");
	fileMenu->Append(wxID_SAVEAS, "Save &As...\tShift+Ctrl+S", "Save current file as...");
	fileMenu->AppendSeparator();
	fileMenu->Append(wxID_CLOSE, "&Close\tCtrl+W", "Close the current document");
	fileMenu->Append(myID_CLOSE_ALL, "C&lose All\tShift+Ctrl+W", "Close all open documents.");
	fileMenu->Append(wxID_EXIT, "&Quit\tCtrl+Q", "Quit the application");
	
	editMenu->Append(wxID_UNDO, "&Undo\tCtrl+Z", "Undo last action");
	editMenu->Append(wxID_REDO, "&Redo\tCtrl+Y", "Redo last action");
	editMenu->AppendSeparator();
	editMenu->Append(wxID_CUT, "Cu&t\tCtrl+X", "Cut selected text");
	editMenu->Append(wxID_COPY, "&Copy\tCtrl+C", "Copy selected text");
	editMenu->Append(wxID_PASTE, "&Paste\tCtrl+V", "Paste contents of clipboard");
	editMenu->Append(wxID_CLEAR, "&Delete\tDel");
	editMenu->AppendSeparator();
	//editMenu->Append(wxID_FIND, "&Find\tCtrl+F");
	//editMenu->Append(wxID_REPLACE, "&Replace\tCtrl+R");
	//editMenu->AppendSeparator();
	//editMenu->Append(wxID_PREFERENCES, "&Preferences\tCtrl+P");
	//editMenu->AppendSeparator();
	editMenu->Append(wxID_SELECTALL, "Select All\tCtrl+A", "Select all the text in the current document");

	viewMenu->AppendCheckItem(myID_VIEW_WHITESPACE, "Show Invisible Characters\tCtrl+Shift+I", "Toggle visibility of white space characters");
	viewMenu->AppendCheckItem(myID_VIEW_LINE_ENDINGS, "Show line endings", "Toggle visibility of line ending characters");
	viewMenu->AppendCheckItem(myID_LINE_WRAP, "Word Wrap", "Toggle wrapping of long lines");
	
	helpMenu->Append(wxID_ABOUT, "&About...\tF1", "Show info about this application");
}

void TyroMenu::SetupLangMenu()
{
	StringMap langs = lang_config->GetLangList();
	StringMap::iterator it;
	
	StringMap::iterator last = langs.end();
	
	for (it = langs.begin(); it != last; ++it)
	{
		langMenu->Append(wxID_ANY, it->second, "Hightlight file as " + it->second, wxITEM_CHECK);
	}
}

void TyroMenu::EnableEditControls(bool enable)
{
	this->fileMenu->Enable(wxID_SAVE, enable);
	this->fileMenu->Enable(wxID_SAVEAS, enable);
	this->fileMenu->Enable(wxID_CLOSE, enable);
	this->fileMenu->Enable(myID_CLOSE_ALL, enable);
	
	// Enable/disable top level menus
	this->EnableEntireMenu(myEDIT_MENU, this->editMenu, enable);
	this->EnableEntireMenu(myVIEW_MENU, this->viewMenu, enable);
	this->EnableEntireMenu(myLANG_MENU, this->langMenu, enable);
}

/**
 * Enable/disable all the items in the menu, for environments
 * that don't properly support disabling the menu by the parent label (like Ubuntu's Unity)
 *
 * @param size_t menuId
 * @param wxMenu* menu
 * @param bool enable
 * @return void
 */ 
void TyroMenu::EnableEntireMenu(size_t menuId, wxMenu *menu, bool enable)
{
	// Toggle the top of the menu
	this->EnableTop(menuId, enable);

	// Toggle the rest of the items in the menu
	wxMenuItemList list = menu->GetMenuItems();
	wxMenuItemList::iterator iter;

	for(iter = list.begin(); iter != list.end(); ++iter)
	{
		wxMenuItem *current = *iter;
		current->Enable(enable);
	}
}