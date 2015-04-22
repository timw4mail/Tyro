/**
 * Main Application Frame
 */
#include "MainFrame.h"

MainFrame::MainFrame(wxFrame *frame, const wxString &title)
	: wxFrame(frame, -1, title)
{
	#include "../../resources/xpm/document.xpm"
	
	wxIcon app_icon(document);
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
	mbar = new wxMenuBar();

	// Create Base menus
	fileMenu = new wxMenu("");
	editMenu = new wxMenu("");
	viewMenu = new wxMenu("");
	langMenu = new wxMenu("");
	helpMenu = new wxMenu("");

	// Add items to top-level menus
	fileMenu->Append(wxID_NEW, "&New\tCtrl+N", "Create a new file");
	fileMenu->AppendSeparator();
	fileMenu->Append(wxID_OPEN, "&Open\tCtrl+O", "Opens an existing file");
	fileMenu->Append(wxID_SAVE, "&Save\tCtrl+S", "Save the content");
	fileMenu->Append(wxID_SAVEAS, "Save &As...\tShift+Ctrl+S", "Save current file as...");
	fileMenu->AppendSeparator();
	fileMenu->Append(wxID_CLOSE, "&Close\tCtrl+W", "Close the current document");
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
	editMenu->Append(wxID_SELECTALL, "Select All\tCtrl+A", "Select all the text in the current document");	
	
	helpMenu->Append(wxID_ABOUT, "&About...\tF1", "Show info about this application");

	// Add the menus to the menubar
	mbar->Append(fileMenu, "&File");
	mbar->Append(editMenu, "&Edit");
	//mbar->Append(viewMenu, "&View");
	//mbar->Append(langMenu, "&Language");
	mbar->Append(helpMenu, "&Help");

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
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnNew, this, wxID_NEW);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnOpen, this, wxID_OPEN);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnSave, this, wxID_SAVE);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnSaveAs, this, wxID_SAVEAS);
	Bind(wxEVT_AUINOTEBOOK_PAGE_CLOSE, &MainFrame::OnClose, this, wxID_ANY);
	Bind(wxEVT_AUINOTEBOOK_PAGE_CLOSED, &MainFrame::OnClosed, this, wxID_ANY);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnCloseTab, this, wxID_CLOSE);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnAbout, this, wxID_ABOUT);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnQuit, this, wxID_EXIT);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnEditCut, this, wxID_CUT);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnEditCopy, this, wxID_COPY);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnEditPaste, this, wxID_PASTE);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnEditSelectAll, this, wxID_SELECTALL);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnEditUndo, this, wxID_UNDO);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnEditRedo, this, wxID_REDO);
}

void MainFrame::OnNew(wxCommandEvent &WXUNUSED(event))
{
	this->EnableEditControls();
	notebook->AddTab();
}

void MainFrame::OnOpen(wxCommandEvent &WXUNUSED(event))
{
	wxString filename;
	
	wxFileDialog dlg (this, _T("Open file"), wxEmptyString, wxEmptyString,
		TYRO_FILE_OPEN_WILDCARDS, wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_CHANGE_DIR);

	if (dlg.ShowModal() != wxID_OK) return;
	
	filename = dlg.GetPath();
	
	this->EnableEditControls();
	
	notebook->AddTab(filename);
}

void MainFrame::OnClose(wxAuiNotebookEvent &event)
{
	int current_tab = notebook->GetSelection();
	EditPane *editor = notebook->GetCurrentEditor();
	
	// Sanity check
	if (current_tab == -1) return;
	
	if (editor->IsModified())
	{
		int Msgbox_Choice = wxMessageBox(
			_T("File has not been saved, save file before closing?"), 
			_T("Modified File"),
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
				wxMessageBox(_("File could not be saved"), _("Error"), wxOK | wxICON_EXCLAMATION);
				return event.Veto();
			}
		}
	};
}

void MainFrame::OnClosed(wxAuiNotebookEvent &WXUNUSED(event))
{
	if (notebook->GetPageCount() == 0)
	{
		this->EnableEditControls(false);
	}
}

void MainFrame::OnCloseTab(wxCommandEvent &WXUNUSED(event))
{
	int current_tab = notebook->GetSelection();

	notebook->DeletePage(current_tab);
	
	if (notebook->GetPageCount() == 0)
	{
		this->EnableEditControls(false);
	}
}

void MainFrame::OnSave(wxCommandEvent &WXUNUSED(event))
{
	EditPane *editor = notebook->GetCurrentEditor();
	editor->SaveFile();
}

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
	info.SetVersion(APP_VERSION, "Prerelease");
	
	info.AddDeveloper("Tim Warren, Programmer");
	
	info.SetDescription("Tyro, a text editor for all development");
	info.SetCopyright(" (C) 2015, Timothy J Warren");
	
	wxAboutBox(info);
}

/**
 * Toggle enable/disable of document-specific controls
 * 
 * @param bool enable
 * @return void
 */
void MainFrame::EnableEditControls(bool enable)
{
	fileMenu->Enable(wxID_SAVE, enable);
	fileMenu->Enable(wxID_SAVEAS, enable);
	fileMenu->Enable(wxID_CLOSE, enable);
	
	editMenu->Enable(wxID_CUT, enable);
	editMenu->Enable(wxID_COPY, enable);
	editMenu->Enable(wxID_PASTE, enable);
	editMenu->Enable(wxID_CLEAR, enable);
	//editMenu->Enable(wxID_FIND, enable);
	editMenu->Enable(wxID_SELECTALL, enable);
	
	toolBar->EnableTool(wxID_SAVE, enable);
	toolBar->EnableTool(wxID_CLOSE, enable);
	toolBar->EnableTool(wxID_COPY, enable);
	toolBar->EnableTool(wxID_CUT, enable);
	toolBar->EnableTool(wxID_PASTE, enable);
}
