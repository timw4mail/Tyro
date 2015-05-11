/**
 * Main Application Frame
 */
#include "widget.h"

extern TyroMenu *mbar;
static TabContainer *notebook;

MainFrame::MainFrame(wxFrame *frame, const wxString &title)
	: wxFrame(frame, -1, title)
{
	#include "../../resources/xpm/tyro.xpm"

	findReplaceData = new wxFindReplaceData(wxFR_DOWN);
	
	wxIcon app_icon(tyro_icon);
	this->SetIcon(app_icon);
	
	// Create menus and bars
#ifdef __WXMAC__
	wxMenuBar::MacSetCommonMenuBar(mbar);
#endif // __WXMAC__
	SetMenuBar(mbar);
	
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

MainFrame::~MainFrame() 
{
	wxLogDebug("Main Frame Destructor Called.");
	delete notebook;
}

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
	#include "../../resources/xpm/128/new.xpm"
	#include "../../resources/xpm/128/open.xpm"
	#include "../../resources/xpm/128/save.xpm"
	#include "../../resources/xpm/128/cut.xpm"
	#include "../../resources/xpm/128/copy.xpm"
	#include "../../resources/xpm/128/paste.xpm"

	wxBitmap new_file_icon(new_file);
	wxBitmap open_file_icon(open);
	wxBitmap save_file_icon(save);
	wxBitmap copy_icon(copy);
	wxBitmap cut_icon(cut);
	wxBitmap paste_icon(paste);
#endif
#ifdef __WXWIN__
	#include "../../resources/xpm/48/new.xpm"
	#include "../../resources/xpm/48/open.xpm"
	#include "../../resources/xpm/48/save.xpm"
	#include "../../resources/xpm/48/cut.xpm"
	#include "../../resources/xpm/48/copy.xpm"
	#include "../../resources/xpm/48/paste.xpm"

	wxBitmap new_file_icon(new_file);
	wxBitmap open_file_icon(open);
	wxBitmap save_file_icon(save);
	wxBitmap copy_icon(copy);
	wxBitmap cut_icon(cut);
	wxBitmap paste_icon(paste);
#endif
#ifdef __WXGTK__
	wxBitmap new_file_icon = wxArtProvider::GetBitmap(wxART_NEW, wxART_TOOLBAR);
	wxBitmap open_file_icon = wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_TOOLBAR);
	wxBitmap save_file_icon = wxArtProvider::GetBitmap(wxART_FILE_SAVE, wxART_TOOLBAR);
	wxBitmap copy_icon = wxArtProvider::GetBitmap(wxART_COPY, wxART_TOOLBAR);
	wxBitmap cut_icon = wxArtProvider::GetBitmap(wxART_CUT, wxART_TOOLBAR);
	wxBitmap paste_icon = wxArtProvider::GetBitmap(wxART_PASTE, wxART_TOOLBAR);
#endif 
	
	CreateToolBar(wxNO_BORDER | wxTB_FLAT | wxTB_HORIZONTAL);

	toolBar = GetToolBar();

	toolBar->AddTool(wxID_NEW, "New", new_file_icon, "New file");
	toolBar->AddTool(wxID_OPEN, "Open", open_file_icon, "Open file");
	toolBar->AddTool(wxID_SAVE, "Save", save_file_icon, "Save file");
	toolBar->AddSeparator();
	toolBar->AddTool(wxID_COPY, "Copy", copy_icon, "Copy");
	toolBar->AddTool(wxID_CUT, "Cut", cut_icon, "Cut");
	toolBar->AddTool(wxID_PASTE, "Paste", paste_icon, "Paste");

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
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnNew, this, wxID_NEW);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnOpen, this, wxID_OPEN);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnSave, this, wxID_SAVE);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnSaveAs, this, wxID_SAVEAS);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnCloseTab, this, wxID_CLOSE);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &TabContainer::OnCloseAll, notebook, myID_CLOSE_ALL);
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
	mbar->EnableEditControls(enable);
	this->toolBar->EnableTool(wxID_SAVE, enable);
	this->toolBar->EnableTool(wxID_CLOSE, enable);
	this->toolBar->EnableTool(wxID_COPY, enable);
	this->toolBar->EnableTool(wxID_CUT, enable);
	this->toolBar->EnableTool(wxID_PASTE, enable);
}
