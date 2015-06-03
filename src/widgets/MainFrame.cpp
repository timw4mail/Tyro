/**
 * Main Application Frame
 */
#include "widget.h"

// Nasty globals
extern TyroMenu *Glob_menu_bar;
extern PrefPane *Glob_pref_pane;
static TabContainer *notebook;

// Frame icon
#include "../../resources/xpm/tyro.xpm"

/**
 * Constructor
 */
MainFrame::MainFrame(wxFrame *frame, const wxString &title)
	: wxFrame(frame, -1, title, wxDefaultPosition, wxSize(800,600))
{
	// Create the tab container
	notebook = new TabContainer(this);
	
	// Set the frame icon
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
	this->Bind(wxEVT_MENU, [=](wxCommandEvent& event) {
		EditPane *editor = notebook->GetCurrentEditor();
		
		switch(event.GetId())
		{
			case wxID_CUT:
				editor->Cut();
			break;
			
			case wxID_COPY:
				editor->Copy();
			break;
			
			case wxID_PASTE:
				if (editor->CanPaste()) editor->Paste();
			break;
			
			case wxID_SELECTALL:
				editor->SelectAll();
			break;
			
			case wxID_UNDO:
				if (editor->CanUndo()) editor->Undo();
			break;
			
			case wxID_REDO:
				if (editor->CanRedo()) editor->Redo();
			break;
			
			case wxID_PREFERENCES:
				Glob_pref_pane->Show(this);
			break;
			
			case wxID_FIND:
				this->OnEditFind(event);
			break;
			
			case wxID_REPLACE:
				this->OnEditReplace(event);
			break;
			
			default:
				event.Skip(true);
			break;
		}
	});
	
	// View Menu Events
	Bind(wxEVT_MENU, &MainFrame::OnToggleWhitespace, this, myID_VIEW_WHITESPACE);
	Bind(wxEVT_MENU, &MainFrame::OnToggleLineWrap, this, myID_LINE_WRAP);
	Bind(wxEVT_MENU, &MainFrame::OnToggleLineEndings, this, myID_VIEW_LINE_ENDINGS);

	// Find/Replace Events
	Bind(wxEVT_FIND, &MainFrame::OnFindDialog, this, wxID_ANY);
	Bind(wxEVT_FIND_NEXT, &MainFrame::OnFindDialog, this, wxID_ANY);
	Bind(wxEVT_FIND_REPLACE, &MainFrame::OnFindDialog, this, wxID_ANY);
	Bind(wxEVT_FIND_REPLACE_ALL, &MainFrame::OnFindDialog, this, wxID_ANY);
	this->Bind(wxEVT_FIND_CLOSE, [=](wxFindDialogEvent &event) {
		wxLogDebug("wxEVT_FIND_CLOSE");
		event.GetDialog()->Hide();
	});
	
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
 * Display a file open dialog, and open the selected files
 *
 * @return void
 */ 
void MainFrame::OnOpen(wxCommandEvent &WXUNUSED(event))
{
	wxArrayString filelist;
	
	wxFileDialog dlg(this, "Open file(s)", wxEmptyString, wxEmptyString,
		TYRO_FILE_OPEN_WILDCARDS, wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_CHANGE_DIR | wxFD_MULTIPLE);

	if (dlg.ShowModal() != wxID_OK) return;
	
	dlg.GetPaths(filelist);
	
	this->OpenFiles(filelist);
}

/**
 * Open tabs containing the files passed
 * 
 * @param wxArrayString& filelist
 * @return void
 */
void MainFrame::OpenFiles(wxArrayString filelist)
{
	int listcount = filelist.GetCount();
	
	if (listcount < 1) return;
	
	// Open a new tab for each file
	notebook->Freeze();
	for (int i = 0; i < listcount; i++)
	{
		notebook->AddTab(filelist[i]);
	}
	notebook->Thaw();
	
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

	notebook->Freeze();
	notebook->DeletePage(current_tab);
	
	if (notebook->GetPageCount() == 0)
	{
		this->EnableEditControls(false);
	}
	
	notebook->Thaw();
	this->manager->Update();
}

/**
 * Close all the open tabs
 *
 * @return void
 */
void MainFrame::OnCloseAll(wxCommandEvent &WXUNUSED(event))
{
	notebook->Freeze();
	notebook->DeleteAllPages();
	this->EnableEditControls(false);
	notebook->Thaw();
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
	this->Destroy();
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
	if (this->findDlg != nullptr)
	{
		wxDELETE(this->findDlg);
		wxDELETE(this->findData);
	}
	else
	{
		this->findData = new wxFindReplaceData(wxFR_DOWN);
		this->findDlg = new wxFindReplaceDialog(this, this->findData, "Find");
		this->findDlg->Show(true);
	}
}

/**
 * Show the find/replace dialog
 * 
 * @return void
 */ 
void MainFrame::OnEditReplace(wxCommandEvent &WXUNUSED(event)) 
{
	if (this->replaceDlg != nullptr)
	{
		wxDELETE(this->replaceDlg);
		wxDELETE(this->findReplaceData);
	}
	else
	{
		this->findReplaceData = new wxFindReplaceData(wxFR_DOWN);
		this->replaceDlg = new wxFindReplaceDialog(this, this->findReplaceData, 
			"Find and Replace", wxFR_REPLACEDIALOG);
		
		this->replaceDlg->Show(true);
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
	
	// Send find flags to editor control
	editor->SetSearchFlags(stc_flags);

	if (type == wxEVT_FIND)
	{
		wxLogDebug("wxEVT_FIND");
		
		if (editor->GetCurrentPos() < 0 || editor->GetCurrentPos() > editor->GetLastPosition())
		{
			editor->GotoPos(1);
		}
		
		editor->SearchAnchor();
	}
	
	if (type == wxEVT_FIND_NEXT || type == wxEVT_FIND)
	{
		if (type == wxEVT_FIND_NEXT)
		{
			wxLogDebug("wxEVT_FIND_NEXT");
		}
		
		new_pos = ((fr_flags & wxFR_DOWN) != 0)
			? editor->SearchNext(stc_flags, event.GetFindString())
			: editor->SearchPrev(stc_flags, event.GetFindString());
		
		int sel_start = editor->GetSelectionStart();
		int sel_end = editor->GetSelectionEnd();
		
		if (new_pos > 0)
		{
			if ((fr_flags & wxFR_DOWN) != 0)
			{
				editor->GotoPos(sel_end + 1);
			}
			else
			{
				((sel_start - 1) > 0) 
					? editor->GotoPos(sel_start - 1)
					: editor->GotoPos(sel_start);
			}
			
			editor->SearchAnchor();
			editor->SetSelectionStart(sel_start);
			editor->SetSelectionEnd(sel_end);
		}
		
		return;
	}
	else if (type == wxEVT_FIND_REPLACE)
	{
		wxLogDebug("wxEVT_FIND_REPLACE");
		editor->ReplaceSelection(event.GetReplaceString());
	}
	else if (type == wxEVT_FIND_REPLACE_ALL)
	{
		wxLogDebug("wxEVT_FIND_REPLACE_ALL");
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

/**
 * Applies settings when prefs are changed
 *
 * @return void
 */
void MainFrame::OnPrefsChanged(wxCommandEvent &WXUNUSED(event))
{	
	EditPane *editor;
	
	notebook->Freeze();
	for(size_t i = 0; i < notebook->GetPageCount(); i++)
	{
		editor = notebook->GetEditor(i);
		editor->ReApplyTheme();
	}
	notebook->Thaw();
}