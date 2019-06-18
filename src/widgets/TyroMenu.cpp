#include "src/widgets/TyroMenu.h"

/**
 * Constructor
 */ 
TyroMenu::TyroMenu()
{
	this->fileMenu = new wxMenu();
	this->editMenu = new wxMenu();
	this->viewMenu = new wxMenu();
	this->langMenu = new wxMenu();
	this->helpMenu = new wxMenu();

	this->lang_config = new LangConfig();

	this->SetupMainMenus();
	this->SetupLangMenu();
	
	// Add the menus to the menubar
	this->Insert(myFILE_MENU, fileMenu, "&File");
	this->Insert(myEDIT_MENU, editMenu, "&Edit");
	this->Insert(myVIEW_MENU, viewMenu, "&View");
	this->Insert(myLANG_MENU, langMenu, "&Language");
	this->Insert(myHELP_MENU, helpMenu, "&Help");
}

/**
 * Destructor
 */ 
TyroMenu::~TyroMenu()
{
	wxLogDebug("TyroMenu Destructor Called.");
	delete this->lang_config;
}

/**
 * Setup basic menu items
 * 
 * @return void
 */ 
void TyroMenu::SetupMainMenus()
{
	// Add items to top-level menus
	this->fileMenu->Append(wxID_NEW, "&New\tCtrl+N", "Create a new file");
	this->fileMenu->AppendSeparator();
	this->fileMenu->Append(wxID_OPEN, "&Open\tCtrl+O", "Opens an existing file");
	this->fileMenu->Append(myID_OPEN_DIR, "&Open Dir\tShift+Ctrl+O", "Opens the selected folder in the sidebar");
	this->fileMenu->AppendSeparator();
	this->fileMenu->Append(wxID_SAVE, "&Save\tCtrl+S", "Save the content");
	this->fileMenu->Append(wxID_SAVEAS, "Save &As...\tShift+Ctrl+S", "Save current file as...");
	this->fileMenu->AppendSeparator();
	this->fileMenu->Append(wxID_CLOSE, "&Close\tCtrl+W", "Close the current document");
	this->fileMenu->Append(myID_CLOSE_ALL, "C&lose All\tShift+Ctrl+W", "Close all open documents.");
	this->fileMenu->Append(wxID_EXIT, "&Quit\tCtrl+Q", "Quit the application");
	
	this->editMenu->Append(wxID_UNDO, "&Undo\tCtrl+Z", "Undo last action");
	this->editMenu->Append(wxID_REDO, "&Redo\tCtrl+Y", "Redo last action");
	this->editMenu->AppendSeparator();
	this->editMenu->Append(wxID_CUT, "Cu&t\tCtrl+X", "Cut selected text");
	this->editMenu->Append(wxID_COPY, "&Copy\tCtrl+C", "Copy selected text");
	this->editMenu->Append(wxID_PASTE, "&Paste\tCtrl+V", "Paste contents of clipboard");
	//editMenu->Append(wxID_DELETE, "&Delete\tDel");
	this->editMenu->AppendSeparator();
	this->editMenu->Append(wxID_FIND, "&Find\tCtrl+F");
	this->editMenu->Append(wxID_REPLACE, "&Replace\tCtrl+R");
	this->editMenu->AppendSeparator();
	this->editMenu->Append(wxID_SELECTALL, "Select All\tCtrl+A", "Select all the text in the current document");
	this->editMenu->AppendSeparator();
	this->editMenu->Append(wxID_PREFERENCES, "&Preferences\tCtrl+P");

	this->viewMenu->AppendCheckItem(myID_VIEW_WHITESPACE, "Show Invisible Characters\tCtrl+Shift+I", "Toggle visibility of white space characters");
	this->viewMenu->AppendCheckItem(myID_VIEW_LINE_ENDINGS, "Show line endings", "Toggle visibility of line ending characters");
	this->viewMenu->AppendCheckItem(myID_LINE_WRAP, "Word Wrap", "Toggle wrapping of long lines");
	
	this->helpMenu->Append(wxID_ABOUT, "About Tyro", "Show info about this application");
}

/**
 * Create the language selection menu
 *
 * @return void
 */ 
void TyroMenu::SetupLangMenu()
{
	StringMap languages = lang_config->GetLangList();
	
	for (const auto lang: languages)
	{
		langMenu->Append(wxID_ANY, lang.second, "Highlight file as " + lang.second, wxITEM_RADIO);
	}
}

/**
 * Enable/disable controls that require a file to be open
 *
 * @param bool enable 
 * @return void
 */ 
void TyroMenu::EnableEditControls(bool enable)
{
	this->fileMenu->Enable(wxID_SAVE, enable);
	this->fileMenu->Enable(wxID_SAVEAS, enable);
	this->fileMenu->Enable(wxID_CLOSE, enable);
	this->fileMenu->Enable(myID_CLOSE_ALL, enable);
	
	this->editMenu->Enable(wxID_UNDO, enable);
	this->editMenu->Enable(wxID_REDO, enable);
	this->editMenu->Enable(wxID_CUT, enable);
	this->editMenu->Enable(wxID_COPY, enable);
	this->editMenu->Enable(wxID_PASTE, enable);
	this->editMenu->Enable(wxID_SELECTALL, enable);
	this->editMenu->Enable(wxID_FIND, enable);
	this->editMenu->Enable(wxID_REPLACE, enable);
	
	// Enable/disable top level menus
	this->EnableEntireMenu(myVIEW_MENU, this->viewMenu, enable);
	this->EnableEntireMenu(myLANG_MENU, this->langMenu, enable);
}

/**
 * Check the menu item associated with the specified id
 * 
 * @param int id
 * @param bool checked
 * @return void
 */
void TyroMenu::SetIdChecked(int id, bool checked)
{
	this->FindItem(id)->Check(checked);
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

	for(auto item: list)
	{
		item->Enable(enable);
		
		// Uncheck all the items
		if (item->IsCheckable())
		{
			item->Check(false);
		}
	}
}

/**
 * Change the language used for highlighting
 * 
 * @param string lang
 * @return void
 */
void TyroMenu::SetCurrentLanguage(string lang)
{
	if (lang.empty())
	{
		lang = "Plain Text";
	}
	
	this->Check(this->FindMenuItem("&Language", lang), true);
}
