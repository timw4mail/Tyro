/**
 * Main Application Frame
 */
#include "widget.h"

// Nasty globals
extern TyroMenu *Glob_menu_bar;
static TabContainer *notebook;

/**
 * Constructor
 */
MainFrame::MainFrame(wxFrame *frame, const wxString &title)
	: wxFrame(frame, -1, title)
{
	findReplaceData = new wxFindReplaceData(wxFR_DOWN);
	
	// Set the frame icon
#include "../../resources/xpm/tyro.xpm"
	wxIcon app_icon(tyro_icon);
	this->SetIcon(app_icon);
	
	// Apply the menu bar to the current frame
	this->SetMenuBar(Glob_menu_bar);
	
	this->SetupStatusBar();
	
	this->DoLayout();
	this->BindEvents();
}

/**
 * Time to clean up!
 */ 
MainFrame::~MainFrame() 
{
	wxLogDebug("Main Frame Destructor Called.");
	delete notebook;
	delete toolBar;
	manager->UnInit();
}

/**
 * Layout the widgets on the main frame
 *
 * @return void
 */ 
void MainFrame::DoLayout()
{
	// Create the tab container
	notebook = new TabContainer(this);
	
	this->manager = new wxAuiManager(this);
	this->SetupToolbar();
	
	// Setup properties for each AUI pane
	wxAuiPaneInfo toolBarPaneInfo;
	toolBarPaneInfo
		.Top()
		.ToolbarPane()
		.Gripper(false)
		.DockFixed(true)
		.Resizable(true);
	this->manager->AddPane(toolBar, toolBarPaneInfo);
	
	wxAuiPaneInfo notebookPaneInfo;
	notebookPaneInfo.CenterPane();
	this->manager->AddPane(notebook, notebookPaneInfo);
	
	// Update everything
	this->EnableEditControls(false);
}

/**
 * Create the status bar
 *
 * @return void
 */ 
void MainFrame::SetupStatusBar()
{
	CreateStatusBar(3);
}

/**
 * Create the main toolbar
 * 
 * @return void
 */
void MainFrame::SetupToolbar()
{
	// Icon files
#ifndef __WXGTK__
	#include "../../resources/xpm/32/new.xpm"
	#include "../../resources/xpm/32/open.xpm"
	#include "../../resources/xpm/32/save.xpm"
	#include "../../resources/xpm/32/cut.xpm"
	#include "../../resources/xpm/32/copy.xpm"
	#include "../../resources/xpm/32/paste.xpm"

	wxBitmap new_file_icon(new_file);
	wxBitmap open_file_icon(open);
	wxBitmap save_file_icon(save);
	wxBitmap copy_icon(copy);
	wxBitmap cut_icon(cut);
	wxBitmap paste_icon(paste);
#else
	wxBitmap new_file_icon = wxArtProvider::GetBitmap(wxART_NEW, wxART_TOOLBAR);
	wxBitmap open_file_icon = wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_TOOLBAR);
	wxBitmap save_file_icon = wxArtProvider::GetBitmap(wxART_FILE_SAVE, wxART_TOOLBAR);
	wxBitmap copy_icon = wxArtProvider::GetBitmap(wxART_COPY, wxART_TOOLBAR);
	wxBitmap cut_icon = wxArtProvider::GetBitmap(wxART_CUT, wxART_TOOLBAR);
	wxBitmap paste_icon = wxArtProvider::GetBitmap(wxART_PASTE, wxART_TOOLBAR);
#endif 
	
	toolBar = new wxAuiToolBar(this);

	toolBar->AddTool(wxID_NEW, "New", new_file_icon, "New file");
	toolBar->AddTool(wxID_OPEN, "Open", open_file_icon, "Open file");
	toolBar->AddTool(wxID_SAVE, "Save", save_file_icon, "Save file");

	toolBar->AddSeparator();
	
	toolBar->AddTool(wxID_COPY, "Copy", copy_icon, "Copy");
	toolBar->AddTool(wxID_CUT, "Cut", cut_icon, "Cut");
	toolBar->AddTool(wxID_PASTE, "Paste", paste_icon, "Paste");
	toolBar->AddStretchSpacer();
	
	toolBar->Realize();
}

/**
 * Bind event handlers
 * 
 * @return void
 */
void MainFrame::BindEvents()
{
	// File Menu Events
	Bind(wxEVT_MENU, &MainFrame::OnNew, this, wxID_NEW);
	Bind(wxEVT_MENU, &MainFrame::OnOpen, this, wxID_OPEN);
	Bind(wxEVT_MENU, &MainFrame::OnSave, this, wxID_SAVE);
	Bind(wxEVT_MENU, &MainFrame::OnSaveAs, this, wxID_SAVEAS);
	Bind(wxEVT_MENU, &MainFrame::OnCloseTab, this, wxID_CLOSE);
	Bind(wxEVT_MENU, &TabContainer::OnCloseAll, notebook, myID_CLOSE_ALL);
	Bind(wxEVT_MENU, &MainFrame::OnQuit, this, wxID_EXIT);
	
	// Edit Menu Events
	Bind(wxEVT_MENU, &MainFrame::OnEditCut, this, wxID_CUT);
	Bind(wxEVT_MENU, &MainFrame::OnEditCopy, this, wxID_COPY);
	Bind(wxEVT_MENU, &MainFrame::OnEditPaste, this, wxID_PASTE);
	Bind(wxEVT_MENU, &MainFrame::OnEditSelectAll, this, wxID_SELECTALL);
	Bind(wxEVT_MENU, &MainFrame::OnEditUndo, this, wxID_UNDO);
	Bind(wxEVT_MENU, &MainFrame::OnEditRedo, this, wxID_REDO);
	Bind(wxEVT_MENU, &MainFrame::OnEditFind, this, wxID_FIND);
	Bind(wxEVT_MENU, &MainFrame::OnEditReplace, this, wxID_REPLACE);
	
	// View Menu Events
	Bind(wxEVT_MENU, &MainFrame::OnToggleWhitespace, this, myID_VIEW_WHITESPACE);
	Bind(wxEVT_MENU, &MainFrame::OnToggleLineWrap, this, myID_LINE_WRAP);
	Bind(wxEVT_MENU, &MainFrame::OnToggleLineEndings, this, myID_VIEW_LINE_ENDINGS);

	// Find/Replace Events
	Bind(wxEVT_FIND, &MainFrame::OnFindDialog, this, wxID_ANY);
	Bind(wxEVT_FIND_NEXT, &MainFrame::OnFindDialog, this, wxID_ANY);
	Bind(wxEVT_FIND_REPLACE, &MainFrame::OnFindDialog, this, wxID_ANY);
	Bind(wxEVT_FIND_REPLACE_ALL, &MainFrame::OnFindDialog, this, wxID_ANY);
	Bind(wxEVT_FIND_CLOSE, &MainFrame::OnFindDialog, this, wxID_ANY);
	
	// Language Selection
	Bind(wxEVT_MENU, &MainFrame::OnLangSelect, this, wxID_ANY);
	
	// Help Menu Events
	Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
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
	
	// Make sure the layout is updated immediately
	this->manager->Update();
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
	notebook->Hide();
	for (int i = 0; i < listcount; i++)
	{
		notebook->AddTab(filelist[i]);
	}
	notebook->Show();
	
	this->EnableEditControls(true);
}

/**
 * Close the current tab via the toolbar or menu
 * 
 * @return void
 */ 
void MainFrame::OnCloseTab(wxCommandEvent &WXUNUSED(event))
{
	int current_tab = notebook->GetSelection();

	notebook->Hide();
	notebook->DeletePage(current_tab);
	
	if (notebook->GetPageCount() == 0)
	{
		this->EnableEditControls(false);
	}
	
	notebook->Show();
	this->manager->Update();
}

/**
 * Close all the open tabs
 *
 * @return void
 */
void MainFrame::OnCloseAll(wxCommandEvent &WXUNUSED(event))
{
	notebook->Hide();
	notebook->DeleteAllPages();
	this->EnableEditControls(false);
	notebook->Show();
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
	
	// Update the main view
	this->manager->Update();
}	

/**
 * Close Tyro
 *
 * @return void
 */ 
void MainFrame::OnQuit(wxCommandEvent &WXUNUSED(event))
{
	Destroy();
}

/**
 * Cut to the clipboard
 * 
 * @return void
 */ 
void MainFrame::OnEditCut(wxCommandEvent &WXUNUSED(event))
{
	notebook->GetCurrentEditor()->Cut();
}

/**
 * Copy to the clipboard
 * 
 * @return void
 */ 
void MainFrame::OnEditCopy(wxCommandEvent &WXUNUSED(event))
{
	notebook->GetCurrentEditor()->Copy();
}

/**
 * Paste from the clipboard
 * 
 * @return void
 */ 
void MainFrame::OnEditPaste(wxCommandEvent &WXUNUSED(event))
{
	if (notebook->GetCurrentEditor()->CanPaste())
	{
		notebook->GetCurrentEditor()->Paste();
	}
}

/**
 * Select all the text in the current document
 *
 * @return void
 */ 
void MainFrame::OnEditSelectAll(wxCommandEvent &WXUNUSED(event))
{
	notebook->GetCurrentEditor()->SelectAll();
}

/**
 * Undo recent change(s)
 * 
 * @return void
 */ 
void MainFrame::OnEditUndo(wxCommandEvent &WXUNUSED(event))
{
	if (notebook->GetCurrentEditor()->CanUndo())
	{
		notebook->GetCurrentEditor()->Undo();
	}
}

/**
 * Redo recent change(s)
 *
 * @return void
 */ 
void MainFrame::OnEditRedo(wxCommandEvent &WXUNUSED(event))
{
	if (notebook->GetCurrentEditor()->CanRedo())
	{
		notebook->GetCurrentEditor()->Redo();
	}
}

/**
 * Create and show about dialog
 *
 * @return void
 */ 
void MainFrame::OnAbout(wxCommandEvent &WXUNUSED(event))
{
	wxAboutDialogInfo info;
	
	info.SetName(APP_NAME);
	info.SetVersion(APP_VERSION, APP_VERSION_MORE);
	
	info.AddDeveloper("Tim Warren");
	info.AddArtist("Brian Smith: Main icon");
	info.AddArtist("http://dryicons.com: Other icons");
	
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
	// Update menu items
	Glob_menu_bar->EnableEditControls(enable);
	
	// Toggle toolbar items
	this->toolBar->EnableTool(wxID_SAVE, enable);
	this->toolBar->EnableTool(wxID_CLOSE, enable);
	this->toolBar->EnableTool(wxID_COPY, enable);
	this->toolBar->EnableTool(wxID_CUT, enable);
	this->toolBar->EnableTool(wxID_PASTE, enable);
	
	// Make sure the toolbar is refreshed instantly
	this->manager->Update();
}

/**
 * Handle selection of highlighting language
 * 
 * @param wxCommandEvent& event
 * @return void
 */
void MainFrame::OnLangSelect(wxCommandEvent &event)
{
	wxMenu *selectedMenu = (wxMenu *) event.GetEventObject();
	wxMenu *langMenu = Glob_menu_bar->GetMenu(myLANG_MENU);
	if (langMenu == NULL) wxLogDebug("Couldn't get lang menu");

	if (selectedMenu == langMenu)
	{
		wxMenuItem *sel_item = langMenu->FindChildItem(event.GetId());
		wxString itemLabel = sel_item->GetItemLabelText();

		notebook->GetCurrentEditor()->SetCurrentLang(itemLabel.ToStdString());
	}
	else
	{
		// Go to the more specific event handlers
		event.Skip(true);
	}
}
