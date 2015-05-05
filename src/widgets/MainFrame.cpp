/**
 * Main Application Frame
 */
#include "MainFrame.h"

MainFrame::MainFrame(wxFrame *frame, const wxString &title)
	: wxFrame(frame, -1, title)
{
	#include "../../resources/xpm/tyro.xpm"

	findReplaceData = new wxFindReplaceData(wxFR_DOWN);
	
	wxIcon app_icon(tyro_icon);
	this->SetIcon(app_icon);
	
	// Create menus and bars
	this->SetupMenu();
	this->SetupStatusBar();
	this->SetupToolbar();
	
	// Create the tab container
	notebook = new TabContainer(this);

	// Set up control layout
	wxBoxSizer *base_sizer = new wxBoxSizer(wxVERTICAL);

	base_sizer->Add(notebook, 1, wxEXPAND | wxALL, 5);
	base_sizer->SetContainingWindow(this);
	base_sizer->SetMinSize(800,600);
	
	this->EnableEditControls(false);
	this->BindEvents();
	
	// Do the layout
	SetSizerAndFit(base_sizer);
}

MainFrame::~MainFrame() {}

void MainFrame::SetupStatusBar()
{
	CreateStatusBar(2);
	SetStatusText(_(""), 0);
	SetStatusText(_(""), 1);
}

/**
 * Create the main toolbar
 * 
 * @return void
 */
void MainFrame::SetupToolbar()
{
	// Icon files
#ifdef __WXMAC__
	#include "../../resources/xpm/file_add.xpm"
	#include "../../resources/xpm/folder.xpm"
	#include "../../resources/xpm/diskette.xpm"
	#include "../../resources/xpm/close.xpm"
	#include "../../resources/xpm/copy.xpm"
	#include "../../resources/xpm/scissors.xpm"
	#include "../../resources/xpm/clipboard.xpm"

	wxBitmap new_file_icon(file_add);
	wxBitmap open_file_icon(folder);
	wxBitmap save_file_icon(diskette);
	wxBitmap close_file_icon(close);
	wxBitmap copy_icon(copy);
	wxBitmap cut_icon(scissors);
	wxBitmap paste_icon(clipboard);
#else
	wxBitmap new_file_icon = wxArtProvider::GetBitmap(wxART_NEW, wxART_TOOLBAR);
	wxBitmap open_file_icon = wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_TOOLBAR);
	wxBitmap save_file_icon = wxArtProvider::GetBitmap(wxART_FILE_SAVE, wxART_TOOLBAR);
	wxBitmap close_file_icon = wxArtProvider::GetBitmap(wxART_CLOSE, wxART_TOOLBAR);
	wxBitmap copy_icon = wxArtProvider::GetBitmap(wxART_COPY, wxART_TOOLBAR);
	wxBitmap cut_icon = wxArtProvider::GetBitmap(wxART_CUT, wxART_TOOLBAR);
	wxBitmap paste_icon = wxArtProvider::GetBitmap(wxART_PASTE, wxART_TOOLBAR);
#endif 
	
	CreateToolBar(wxNO_BORDER | wxTB_FLAT | wxTB_HORIZONTAL);

	toolBar = GetToolBar();

	toolBar->AddTool(wxID_NEW, "New", new_file_icon, "New file");
	toolBar->AddTool(wxID_OPEN, "Open", open_file_icon, "Open file");
	toolBar->AddTool(wxID_SAVE, "Save", save_file_icon, "Save file");
#ifndef __WXMSW__
	toolBar->AddTool(wxID_CLOSE, "Close", close_file_icon, "Close file");
#endif
	toolBar->AddSeparator();
	toolBar->AddTool(wxID_COPY, "Copy", copy_icon, "Copy");
	toolBar->AddTool(wxID_CUT, "Cut", cut_icon, "Cut");
	toolBar->AddTool(wxID_PASTE, "Paste", paste_icon, "Paste");

	toolBar->Realize();
}

/**
 * Create the main menu
 * 
 * @return void
 */
void MainFrame::SetupMenu()
{
	// create a menu bar
	this->mbar = new wxMenuBar();

	// Create Base menus
	fileMenu = new wxMenu();
	editMenu = new wxMenu();
	viewMenu = new wxMenu();
	langMenu = new wxMenu();
	helpMenu = new wxMenu();

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

	// Add the menus to the menubar
	this->mbar->Append(fileMenu, "&File");
	this->mbar->Append(editMenu, "&Edit");
	this->mbar->Append(viewMenu, "&View");
	this->mbar->Append(langMenu, "&Language");
	this->mbar->Append(helpMenu, "&Help");

#ifdef __WXMAC__
	wxMenuBar::MacSetCommonMenuBar(mbar);
#endif // __WXMAC__
	SetMenuBar(mbar);
}

/**
 * Bind event handlers
 * 
 * @return void
 */
void MainFrame::BindEvents()
{
	// File Menu Events
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnNew, this, wxID_NEW);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnOpen, this, wxID_OPEN);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnSave, this, wxID_SAVE);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnSaveAs, this, wxID_SAVEAS);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnCloseTab, this, wxID_CLOSE);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnCloseAll, this, myID_CLOSE_ALL);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnQuit, this, wxID_EXIT);
	
	// Edit Menu Events
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnEditCut, this, wxID_CUT);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnEditCopy, this, wxID_COPY);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnEditPaste, this, wxID_PASTE);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnEditSelectAll, this, wxID_SELECTALL);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnEditUndo, this, wxID_UNDO);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnEditRedo, this, wxID_REDO);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnEditFind, this, wxID_FIND);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnEditReplace, this, wxID_REPLACE);
	
	// View Menu Events
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnToggleWhitespace, this, myID_VIEW_WHITESPACE);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnToggleLineWrap, this, myID_LINE_WRAP);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnToggleLineEndings, this, myID_VIEW_LINE_ENDINGS);
	
	// Notebook Events
	Bind(wxEVT_AUINOTEBOOK_PAGE_CLOSE, &MainFrame::OnClose, this, wxID_ANY);
	Bind(wxEVT_AUINOTEBOOK_PAGE_CLOSED, &MainFrame::OnClosed, this, wxID_ANY);
	Bind(wxEVT_AUINOTEBOOK_TAB_RIGHT_DOWN, &MainFrame::OnTabContextMenu, this, wxID_ANY);
	
	// Find/Replace Events
	Bind(wxEVT_FIND, &MainFrame::OnFindDialog, this, wxID_ANY);
	Bind(wxEVT_FIND_NEXT, &MainFrame::OnFindDialog, this, wxID_ANY);
	Bind(wxEVT_FIND_REPLACE, &MainFrame::OnFindDialog, this, wxID_ANY);
	Bind(wxEVT_FIND_REPLACE_ALL, &MainFrame::OnFindDialog, this, wxID_ANY);
	Bind(wxEVT_FIND_CLOSE, &MainFrame::OnFindDialog, this, wxID_ANY);
	
	// Help Menu Events
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnAbout, this, wxID_ABOUT);
}

/**
 * Create a new document
 * 
 * @return void
 */ 
void MainFrame::OnNew(wxCommandEvent &WXUNUSED(event))
{
	this->EnableEditControls();
	notebook->AddTab();
}

/**
 * Open existing document(s)
 *
 * @return void
 */ 
void MainFrame::OnOpen(wxCommandEvent &WXUNUSED(event))
{
	wxArrayString filelist;
	int listcount;
	
	wxFileDialog dlg (this, "Open file(s)", wxEmptyString, wxEmptyString,
		TYRO_FILE_OPEN_WILDCARDS, wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_CHANGE_DIR | wxFD_MULTIPLE);

	if (dlg.ShowModal() != wxID_OK) return;
	
	dlg.GetPaths(filelist);
	listcount = filelist.GetCount();
	
	// Open a new tab for each file
	for (int i = 0; i < listcount; i++)
	{
		notebook->AddTab(filelist[i]);
	}
	
	this->EnableEditControls(true);
}

/**
 * Event handler for file saving checks on tab close
 *
 * @param wxAuiNotebookEvent& event
 * @return void
 */ 
void MainFrame::OnClose(wxAuiNotebookEvent &event)
{
	int current_tab = notebook->GetSelection();
	EditPane *editor = notebook->GetCurrentEditor();
	
	// Sanity check
	if (current_tab == -1) return;
	
	if (editor->IsModified())
	{
		int Msgbox_Choice = wxMessageBox(
			"File has not been saved, save file before closing?", 
			"Modified File",
			wxYES_NO | wxCANCEL | wxICON_QUESTION,
			this
		);
		
		if (Msgbox_Choice == wxCANCEL)
		{
			return event.Veto();
		}
		else if (Msgbox_Choice == wxYES)
		{
			editor->SaveFile();
			if (editor->IsModified())
			{
				wxMessageBox(TYRO_SAVE_ERROR, TYRO_SAVE_ERROR_CAPTION, wxOK | wxICON_EXCLAMATION);
				return event.Veto();
			}
		}
	};
}

/**
 * Event handler triggered after a tab is closed
 * 
 * @param WXUNUSED
 * @return void
 */
void MainFrame::OnClosed(wxAuiNotebookEvent &WXUNUSED(event))
{
	if (notebook->GetPageCount() == 0)
	{
		this->EnableEditControls(false);
	}
}

/**
 * Close the current tab via the toolbar or menu
 * 
 * @return void
 */ 
void MainFrame::OnCloseTab(wxCommandEvent &WXUNUSED(event))
{
	int current_tab = notebook->GetSelection();

	notebook->DeletePage(current_tab);
	
	if (notebook->GetPageCount() == 0)
	{
		this->EnableEditControls(false);
	}
}

/**
 * Close all the open tabs
 *
 * @return void
 */
void MainFrame::OnCloseAll(wxCommandEvent &WXUNUSED(event))
{
	notebook->DeleteAllPages();
	this->EnableEditControls(false);
}

/**
 * Save the current document
 *
 * @param wxCommandEvent& event
 * @return void
 */
void MainFrame::OnSave(wxCommandEvent &event)
{
	EditPane *editor = notebook->GetCurrentEditor();
	
	// Check if the filename is set for the current file
	if ( ! editor->fileName.IsOk())
	{
		this->OnSaveAs(event);
		return;
	}
	
	editor->SaveFile();
}

/**
 * Save the current document with a new name
 *
 * @return void
 */
void MainFrame::OnSaveAs(wxCommandEvent &WXUNUSED(event))
{
	EditPane *editor = notebook->GetCurrentEditor();

	// If the file hasn't been changed, just return
	if ( ! editor->IsModified()) return;

	wxFileDialog dlg(
		this, 
		"Save as...", 
		wxEmptyString, 
		wxEmptyString,
		TYRO_FILE_SAVE_WILDCARDS,
		wxFD_SAVE | wxFD_OVERWRITE_PROMPT
	);

	// Return if the file isn't to be saved
	if (dlg.ShowModal() != wxID_OK) return;

	wxString filePath = dlg.GetPath();

	// Save the file
	if(editor->SaveFile(filePath))
	{
		wxFileName fileName(filePath);
		const wxString fullPath = filePath;
		const wxString caption= fileName.GetFullName();
		
		// Update the name of the tab
		notebook->SetPageToolTip(notebook->GetSelection(), fullPath);
		notebook->SetPageText(notebook->GetSelection(), caption);

		// Update the editor highlighting
		editor->Highlight(filePath);
	}
}	

void MainFrame::OnQuit(wxCommandEvent &WXUNUSED(event))
{
	Destroy();
}

void MainFrame::OnEditCut(wxCommandEvent &WXUNUSED(event))
{
	notebook->GetCurrentEditor()->Cut();
}

void MainFrame::OnEditCopy(wxCommandEvent &WXUNUSED(event))
{
	notebook->GetCurrentEditor()->Copy();
}

void MainFrame::OnEditPaste(wxCommandEvent &WXUNUSED(event))
{
	if (notebook->GetCurrentEditor()->CanPaste())
	{
		notebook->GetCurrentEditor()->Paste();
	}
}

void MainFrame::OnEditSelectAll(wxCommandEvent &WXUNUSED(event))
{
	notebook->GetCurrentEditor()->SelectAll();
}

void MainFrame::OnEditUndo(wxCommandEvent &WXUNUSED(event))
{
	if (notebook->GetCurrentEditor()->CanUndo())
	{
		notebook->GetCurrentEditor()->Undo();
	}
}

void MainFrame::OnEditRedo(wxCommandEvent &WXUNUSED(event))
{
	if (notebook->GetCurrentEditor()->CanRedo())
	{
		notebook->GetCurrentEditor()->Redo();
	}
}

void MainFrame::OnAbout(wxCommandEvent &WXUNUSED(event))
{
	wxAboutDialogInfo info;
	
	info.SetName(APP_NAME);
	info.SetVersion(APP_VERSION, APP_VERSION_MORE);
	
	info.AddDeveloper("Tim Warren, Programmer");
	info.AddArtist("Brian Smith, Icon");
	
	info.SetDescription("Tyro, a text editor for all development");
	info.SetCopyright(" (C) 2015");
	
	wxAboutBox(info);
}

/**
 * Toggle display of invisibles
 * 
 * @param wxCommandEvent& event
 * @return void
 */
void MainFrame::OnToggleWhitespace(wxCommandEvent& event)
{
	EditPane *editor = notebook->GetCurrentEditor();
	int flag = (event.IsChecked()) 
		? wxSTC_WS_VISIBLEALWAYS 
		: wxSTC_WS_INVISIBLE;
	
	editor->SetViewWhiteSpace(flag);
}

/**
 * Show the find dialog
 * 
 * @return void
 */ 
void MainFrame::OnEditFind(wxCommandEvent &WXUNUSED(event)) 
{
	if (findDlg)
	{
		wxDELETE(findDlg);
	}
	else
	{
		findDlg = new wxFindReplaceDialog(this, findReplaceData, "Find");
		findDlg->Show(true);
	}
}

/**
 * Show the find/replace dialog
 * 
 * @return void
 */ 
void MainFrame::OnEditReplace(wxCommandEvent &WXUNUSED(event)) 
{
	if (replaceDlg)
	{
		wxDELETE(replaceDlg);
	}
	else
	{
		replaceDlg = new wxFindReplaceDialog(this, findReplaceData, 
			"Find and Replace", wxFR_REPLACEDIALOG);
		
		replaceDlg->Show(true);
	}
}

/**
 * Handles events coming from find dialog
 * 
 * @param wxFindDialogEvent& event
 * @return void 
 */
void MainFrame::OnFindDialog(wxFindDialogEvent &event)
{
	wxEventType type = event.GetEventType();
	EditPane *editor = notebook->GetCurrentEditor();
	
	// Parse flags
	int stc_flags = 0;
	int fr_flags = event.GetFlags();
	
	if (fr_flags & wxFR_WHOLEWORD) stc_flags |= wxSTC_FIND_WHOLEWORD;
	if (fr_flags & wxFR_MATCHCASE) stc_flags |= wxSTC_FIND_MATCHCASE;
	
	// Position after search
	int new_pos = 0;
	int new_line = 1;
	
	// Send find flags to editor control
	editor->SetSearchFlags(stc_flags);
	  
	if (type == wxEVT_FIND)
	{
		wxLogDebug("wxEVT_FIND");
		
		editor->SearchAnchor();
		
		if ((fr_flags & wxFR_DOWN) != 0)
		{
			new_pos = editor->SearchNext(stc_flags, event.GetFindString());
		}
		else
		{
			new_pos = editor->SearchPrev(stc_flags, event.GetFindString());
		}
		
		if (new_pos >= 0)
		{
			new_line = editor->LineFromPosition(new_pos);
			editor->ScrollToLine(new_line);
		}
	}
	else if (type == wxEVT_FIND_NEXT)
	{
		wxLogDebug("wxEVT_FIND_NEXT");
		
		if ((fr_flags & wxFR_DOWN) != 0)
		{
			new_pos = editor->SearchNext(stc_flags, event.GetFindString());
		}
		else
		{
			new_pos = editor->SearchPrev(stc_flags, event.GetFindString());
		}
		
		if (new_pos >= 0)
		{	
			new_line = editor->LineFromPosition(new_pos);
			editor->ScrollToLine(new_line);
		}
	}
	else if (type == wxEVT_FIND_REPLACE)
	{
		wxLogDebug("wxEVT_FIND_REPLACE");
	}
	else if (type == wxEVT_FIND_REPLACE_ALL)
	{
		wxLogDebug("wxEVT_FIND_REPLACE_ALL");
	}
	else if (type == wxEVT_FIND_CLOSE)
	{
		wxLogDebug("wxEVT_FIND_CLOSE");
		event.GetDialog()->Destroy();
	}
}

/**
 * Toggle line wrap
 * 
 * @param wxCommandEvent& event
 * @return void
 */ 
void MainFrame::OnToggleLineWrap(wxCommandEvent &event)
{
	EditPane *editor = notebook->GetCurrentEditor();
	int flag = (event.IsChecked())
		? wxSTC_WRAP_WORD
		: wxSTC_WRAP_NONE;
	
	editor->SetWrapMode(flag);
}

/**
 * Toggle display of line ending characters
 * 
 * @param wxCommandEvent& event
 * @return void
 */
void MainFrame::OnToggleLineEndings(wxCommandEvent &event)
{
	notebook->GetCurrentEditor()->SetViewEOL(event.IsChecked());
}

/**
 * Toggle enable/disable of document-specific controls
 * 
 * @param bool enable
 * @return void
 */
void MainFrame::EnableEditControls(bool enable)
{
	this->fileMenu->Enable(wxID_SAVE, enable);
	this->fileMenu->Enable(wxID_SAVEAS, enable);
	this->fileMenu->Enable(wxID_CLOSE, enable);
	this->fileMenu->Enable(myID_CLOSE_ALL, enable);
	
	// Enable/disable top level menus
	this->EnableEntireMenu(myEDIT_MENU, this->editMenu, enable);
	this->EnableEntireMenu(myVIEW_MENU, this->viewMenu, enable);
	this->EnableEntireMenu(myLANG_MENU, this->langMenu, enable);
	
	this->toolBar->EnableTool(wxID_SAVE, enable);
	this->toolBar->EnableTool(wxID_CLOSE, enable);
	this->toolBar->EnableTool(wxID_COPY, enable);
	this->toolBar->EnableTool(wxID_CUT, enable);
	this->toolBar->EnableTool(wxID_PASTE, enable);
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
void MainFrame::EnableEntireMenu(size_t menuId, wxMenu *menu, bool enable)
{
	// Toggle the top of the menu
	this->mbar->EnableTop(menuId, enable);
	
	// Toggle the rest of the items in the menu
	wxMenuItemList list = menu->GetMenuItems();
	wxMenuItemList::iterator iter;
	
	for(iter = list.begin(); iter != list.end(); ++iter)
	{
		wxMenuItem *current = *iter;
		current->Enable(enable);
	}
}

/**
 * Displays a context menu on the current tab
 * 
 * @return void
 */ 
void MainFrame::OnTabContextMenu(wxAuiNotebookEvent &WXUNUSED(event))
{
	// Create Menu
	wxMenu *contextMenu = new wxMenu();
	contextMenu->Append(wxID_CLOSE, "&Close\tCtrl+W", "Close the current tab");
	contextMenu->Append(myID_CLOSE_ALL, "C&lose All\tShift+Ctrl+W", "Close all open documents.");

	this->PopupMenu(contextMenu);
}
