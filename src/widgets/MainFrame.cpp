/**
 * Main Application Frame
 */
#include "src/widgets/MainFrame.h"

// Nasty globals
extern TyroMenu *Glob_menu_bar;
extern wxStatusBar *Glob_status_bar;

// Frame icon (const static char *tyro_icon[])
#include "resources/xpm/tyro.xpm"

/**
 * Constructor
 */
MainFrame::MainFrame(wxFrame *frame, const wxString &title, const wxSize &size)
	: wxFrame(frame, -1, title, wxDefaultPosition, size)
{
	// Create the tab container
	this->notebook = new TabContainer(this);

	// Initialize other widgets
	this->fileTreePane = new FileTreePane(this);
	this->prefFrame = new PrefFrame();

	// Set the frame icon
	wxIcon app_icon(tyro_icon);
	this->SetIcon(app_icon);

	// Apply the menu bar to the current frame
	this->SetMenuBar(Glob_menu_bar);

	// Setup StatusBar
	Glob_status_bar = new wxStatusBar(this, wxID_ANY);
	Glob_status_bar->SetFieldsCount(6);

	this->MainLayout();

	this->BindEvents();
}

/**
 * Time to clean up!
 */
MainFrame::~MainFrame()
{
	wxLogDebug("Main Frame Destructor Called.");

	wxDELETE(this->findDlg);
	wxDELETE(this->findData);
	wxDELETE(this->replaceDlg);
	wxDELETE(this->findReplaceData);
	wxDELETE(this->toolBar);
	wxDELETE(this->prefFrame);
	wxDELETE(this->fileTreePane);
	this->manager->UnInit();

	wxDELETE(this->notebook);

	Glob_status_bar->Destroy();
}

/**
 * Layout the widgets on the main frame
 *
 * @return void
 */
void MainFrame::MainLayout()
{
	this->manager = new wxAuiManager(this);
	this->toolBar = this->SetupToolbar();

	// Setup properties for each AUI pane
	wxAuiPaneInfo toolBarPaneInfo;
	toolBarPaneInfo
		.Top()
		.ToolbarPane()
		.Gripper(false)
		.DockFixed(true)
		.Resizable(true);
	this->manager->AddPane(this->toolBar, toolBarPaneInfo);

	wxAuiPaneInfo filePaneInfo;
	filePaneInfo.Left()
		.MinSize(225, 550)
		.RightDockable(true)
		.LeftDockable(true)
		.Resizable(true);
	this->manager->AddPane(this->fileTreePane, filePaneInfo);

	wxAuiPaneInfo notebookPaneInfo;
	notebookPaneInfo.CenterPane();
	this->manager->AddPane(this->notebook, notebookPaneInfo);

	wxAuiPaneInfo statusPaneInfo;
	statusPaneInfo.Bottom()
		.ToolbarPane()
		.Gripper(false)
		.DockFixed(true)
		.Resizable(true);
	this->manager->AddPane(Glob_status_bar, statusPaneInfo);

	// Update everything
	this->EnableEditControls(false);
}

/**
 * Create the main toolbar
 *
 * @return void
 */
wxAuiToolBar* MainFrame::SetupToolbar()
{
	// Icon files
#ifndef __WXGTK__
	#include "resources/xpm/32/new.xpm"
	#include "resources/xpm/32/open.xpm"
	#include "resources/xpm/32/save.xpm"
	#include "resources/xpm/32/cut.xpm"
	#include "resources/xpm/32/copy.xpm"
	#include "resources/xpm/32/paste.xpm"

	wxBitmap new_file_icon(new_file);
	wxBitmap open_file_icon(open);
	wxBitmap open_folder_icon(open);
	wxBitmap save_file_icon(save);
	wxBitmap copy_icon(copy);
	wxBitmap cut_icon(cut);
	wxBitmap paste_icon(paste);
#else
	wxBitmap new_file_icon = wxArtProvider::GetBitmap(wxART_NEW, wxART_TOOLBAR);
	wxBitmap open_file_icon = wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_TOOLBAR);
	wxBitmap open_folder_icon = wxArtProvider::GetBitmap(wxART_FOLDER_OPEN, wxART_TOOLBAR);
	wxBitmap save_file_icon = wxArtProvider::GetBitmap(wxART_FILE_SAVE, wxART_TOOLBAR);
	wxBitmap copy_icon = wxArtProvider::GetBitmap(wxART_COPY, wxART_TOOLBAR);
	wxBitmap cut_icon = wxArtProvider::GetBitmap(wxART_CUT, wxART_TOOLBAR);
	wxBitmap paste_icon = wxArtProvider::GetBitmap(wxART_PASTE, wxART_TOOLBAR);
#endif

	toolBar = new wxAuiToolBar(this);

	toolBar->AddTool(wxID_NEW, "New", new_file_icon, "New file");
	toolBar->AddTool(wxID_OPEN, "Open", open_file_icon, "Open file");
#ifdef __WXGTK__
	toolBar->AddTool(myID_OPEN_DIR, "Open Dir", open_folder_icon, "Open folder");
#endif
	toolBar->AddTool(wxID_SAVE, "Save", save_file_icon, "Save file");

	toolBar->AddSeparator();

	toolBar->AddTool(wxID_COPY, "Copy", copy_icon, "Copy");
	toolBar->AddTool(wxID_CUT, "Cut", cut_icon, "Cut");
	toolBar->AddTool(wxID_PASTE, "Paste", paste_icon, "Paste");
	toolBar->AddStretchSpacer();

	toolBar->Realize();

	return toolBar;
}

/**
 * Bind event handlers
 *
 * @return void
 */
void MainFrame::BindEvents()
{
	// File Menu Events
	this->Bind(wxEVT_MENU, &MainFrame::OnNew, this, wxID_NEW);
	this->Bind(wxEVT_MENU, &MainFrame::OnOpen, this, wxID_OPEN);
	this->Bind(wxEVT_MENU, &MainFrame::OnOpenFolder, this, myID_OPEN_DIR);
	this->Bind(wxEVT_MENU, &MainFrame::OnSave, this, wxID_SAVE);
	this->Bind(wxEVT_MENU, &MainFrame::OnSaveAs, this, wxID_SAVEAS);
	this->Bind(wxEVT_MENU, &MainFrame::OnCloseTab, this, wxID_CLOSE);
	this->Bind(wxEVT_MENU, &TabContainer::OnCloseAllButThis, this->notebook, myID_CLOSE_ALL_BUT_THIS);
	this->Bind(wxEVT_MENU, &TabContainer::OnCloseAll, this->notebook, myID_CLOSE_ALL);
	this->Bind(wxEVT_MENU, &MainFrame::OnQuit, this, wxID_EXIT);

	// Edit Menu Events
	this->Bind(wxEVT_MENU, [=](wxCommandEvent& event) {
		EditorPane *editor = this->notebook->GetCurrentEditor();

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
				this->prefFrame->Show(this);
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
	this->Bind(wxEVT_MENU, &MainFrame::OnToggleWhitespace, this, myID_VIEW_WHITESPACE);
	this->Bind(wxEVT_MENU, &MainFrame::OnToggleLineWrap, this, myID_LINE_WRAP);
	this->Bind(wxEVT_MENU, &MainFrame::OnToggleLineEndings, this, myID_VIEW_LINE_ENDINGS);

	// Find/Replace Events
	this->Bind(wxEVT_FIND, &MainFrame::OnFindDialog, this, wxID_ANY);
	this->Bind(wxEVT_FIND_NEXT, &MainFrame::OnFindDialog, this, wxID_ANY);
	this->Bind(wxEVT_FIND_REPLACE, &MainFrame::OnFindDialog, this, wxID_ANY);
	this->Bind(wxEVT_FIND_REPLACE_ALL, &MainFrame::OnFindDialog, this, wxID_ANY);
	this->Bind(wxEVT_FIND_CLOSE, [=](wxFindDialogEvent &event) {
		wxLogDebug("wxEVT_FIND_CLOSE");
		event.GetDialog()->Hide();
	});

	// Language Selection
	this->Bind(wxEVT_MENU, &MainFrame::OnLangSelect, this, wxID_ANY);

	// Help Menu Events
	this->Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
}

/**
 * Create a new document
 *
 * @return void
 */
void MainFrame::OnNew(wxCommandEvent &WXUNUSED(event))
{
	this->EnableEditControls();
	this->notebook->AddTab();

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

void MainFrame::OnOpenFolder(wxCommandEvent &event)
{
	wxDirDialog dlg(this, "Select Project Dir", wxEmptyString, wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST | wxDD_CHANGE_DIR);

	if (dlg.ShowModal() != wxID_OK) return;

	auto path = dlg.GetPath();

	this->fileTreePane->CreateTree(path);
}

/**
 * Open tabs containing the files passed
 *
 * @param wxArrayString& filelist
 * @return void
 */
void MainFrame::OpenFiles(wxArrayString filelist)
{
	// @TODO skip duplicated files
	int listcount = filelist.GetCount();

	if (listcount < 1) return;

	// Open a new tab for each file
	this->notebook->Freeze();
	for (int i = 0; i < listcount; i++)
	{
		this->notebook->AddTab(filelist[i]);
	}
	this->notebook->Thaw();

	this->EnableEditControls(true);
}

/**
 * Close the current tab via the toolbar or menu
 *
 * @return void
 */
void MainFrame::OnCloseTab(wxCommandEvent &WXUNUSED(event))
{
	int current_tab = this->notebook->GetSelection();

	this->notebook->Freeze();
	this->notebook->DeletePage(current_tab);

	if (this->notebook->GetPageCount() == 0)
	{
		this->EnableEditControls(false);
	}

	this->notebook->Thaw();
	this->manager->Update();
}

/**
 * Close all the open tabs
 *
 * @return void
 */
void MainFrame::OnCloseAll(wxCommandEvent &WXUNUSED(event))
{
	this->notebook->Freeze();
	this->notebook->DeleteAllPages();
	this->EnableEditControls(false);
	this->notebook->Thaw();
}

/**
 * Save the current document
 *
 * @param wxCommandEvent& event
 * @return void
 */
void MainFrame::OnSave(wxCommandEvent &event)
{
	EditorPane *editor = this->notebook->GetCurrentEditor();

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
	EditorPane *editor = this->notebook->GetCurrentEditor();

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
		const wxString caption = fileName.GetFullName();

		// Update the name of the tab
		this->notebook->SetPageToolTip(this->notebook->GetSelection(), filePath);
		this->notebook->SetPageText(this->notebook->GetSelection(), caption);

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
	wxPlatformInfo plat_info;

	info.SetName(APP_NAME);
	info.SetVersion(APP_VERSION, APP_VERSION_MORE);

	info.AddDeveloper("Timothy J. Warren");
	info.AddArtist("Brian Smith: Main icon");

#ifndef __WXGTK__
	info.AddArtist("http://dryicons.com: Other icons");
#endif

	wxString desc = "Tyro, a text editor for all development\n\n";

	desc += wxString::Format("%s %i.%i (%s)\n",
		wxPlatformInfo::GetOperatingSystemIdName(plat_info.GetOperatingSystemId()),
		plat_info.GetOSMajorVersion(),
		plat_info.GetOSMinorVersion(),
		wxPlatformInfo::GetArchName(plat_info.GetArchitecture()));

	desc += wxString::Format("%s %i.%i.%i\n",
		plat_info.GetPortIdName(),
		wxMAJOR_VERSION,
		wxMINOR_VERSION,
		wxRELEASE_NUMBER
	);

#ifdef __WXGTK__
	wxLinuxDistributionInfo dist_info = wxGetLinuxDistributionInfo();
	wxString desk = plat_info.GetDesktopEnvironment();
	if (desk != "")
	{
		desc += wxString::Format("%s\n", desk);
	}

	desc += dist_info.Description;

	if (dist_info.CodeName != "")
	{
		desc += " (" + dist_info.CodeName + ")\n";
	}
#endif

	if ( ! info.HasIcon())
	{
		wxIcon appIcon = wxIcon(tyro_icon);
		info.SetIcon(appIcon);
	}

	info.SetDescription(desc);
	info.SetLicence(APP_LICENSE);

	info.SetCopyright("(C) 2015-2019");

	wxAboutBox(info, this);
}

/**
 * Toggle display of invisibles
 *
 * @param wxCommandEvent& event
 * @return void
 */
void MainFrame::OnToggleWhitespace(wxCommandEvent& event)
{
	EditorPane *editor = this->notebook->GetCurrentEditor();
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
	if (this->findDlg == nullptr)
	{
		this->findData = new wxFindReplaceData(wxFR_DOWN);
		this->findDlg = new wxFindReplaceDialog(this, this->findData, "Find");
	}

	this->findDlg->Show(true);
}

/**
 * Show the find/replace dialog
 *
 * @return void
 */
void MainFrame::OnEditReplace(wxCommandEvent &WXUNUSED(event))
{
	if (this->replaceDlg == nullptr)
	{
		this->findReplaceData = new wxFindReplaceData(wxFR_DOWN);
		this->replaceDlg = new wxFindReplaceDialog(this, this->findReplaceData,
			"Find and Replace", wxFR_REPLACEDIALOG);
	}

	this->replaceDlg->Show(true);
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
	EditorPane *editor = this->notebook->GetCurrentEditor();

	// Parse flags
	uint stc_flags = 0;
	uint fr_flags = event.GetFlags();

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

		// Freeze editor drawing until replacement is finished
		editor->Freeze();

		editor->GotoPos(0); // Go to the start of the document
		editor->SearchAnchor();

		editor->BeginUndoAction();

		while (editor->SearchNext(stc_flags, event.GetFindString()) != -1)
		{
			editor->ReplaceSelection(event.GetReplaceString());
		}

		editor->EndUndoAction();

		editor->ScrollToEnd();

		editor->Thaw();
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
	EditorPane *editor = this->notebook->GetCurrentEditor();
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
	this->notebook->GetCurrentEditor()->SetViewEOL(event.IsChecked());
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
	auto menu_bar = (TyroMenu *) this->GetMenuBar();
	menu_bar->EnableEditControls(enable);

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
	auto *selectedMenu = (wxMenu *) event.GetEventObject();
	auto *langMenu = Glob_menu_bar->GetMenu(myLANG_MENU);
	if (langMenu == nullptr)
	{
		wxLogDebug("Couldn't get lang menu");
		// Go to the more specific event handlers
		event.Skip(true);
		return;
	}

	if (selectedMenu == langMenu)
	{
		wxMenuItem *sel_item = langMenu->FindChildItem(event.GetId());
		wxString itemLabel = sel_item->GetItemLabelText();

		this->notebook->GetCurrentEditor()->SetCurrentLang(itemLabel.ToStdString());
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
	this->notebook->Freeze();
	for(size_t i = 0; i < this->notebook->GetPageCount(); i++)
	{
		this->notebook->GetEditor(i)->ReApplyTheme();
	}
	this->notebook->Thaw();
}
