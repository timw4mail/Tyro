/**
 * Main Application Frame
 */
#include "MainFrame.h"

MainFrame::MainFrame(wxFrame *frame, const wxString& title)
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
	
	this->DisableEditControls();
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
#ifndef __WXWIN__
	toolBar->AddTool(wxID_CLOSE, "Close", close_file_icon, "Close file");
#endif
	toolBar->AddSeparator();
	toolBar->AddTool(wxID_COPY, "Copy", copy_icon, "Copy");
	toolBar->AddTool(wxID_CUT, "Cut", cut_icon, "Cut");
	toolBar->AddTool(wxID_PASTE, "Paste", paste_icon, "Paste");

	toolBar->Realize();
}

void MainFrame::SetupMenu()
{
	// create a menu bar
	mbar = new wxMenuBar();

	// Create Base menus
	fileMenu = new wxMenu(_T(""));
	editMenu = new wxMenu(_T(""));
	viewMenu = new wxMenu(_T(""));
	helpMenu = new wxMenu(_T(""));

	// Add items to top-level menus
	fileMenu->Append(wxID_NEW, _T("&New\tCtrl+N"), _T("Create a new file"));
	fileMenu->AppendSeparator();
	fileMenu->Append(wxID_OPEN, _T("&Open\tCtrl+O"), _T("Opens an existing file"));
	fileMenu->Append(wxID_SAVE, _T("&Save\tCtrl+S"), _T("Save the content"));
	fileMenu->Append(wxID_SAVEAS, _T("Save &As...\tShift+Ctrl+S"), _T("Save current file as..."));
	fileMenu->AppendSeparator();
	fileMenu->Append(wxID_CLOSE, _T("&Close\tCtrl+W"), _T("Close the current document"));
	fileMenu->Append(wxID_EXIT, _T("&Quit\tCtrl+Q"), _T("Quit the application"));
	
	editMenu->Append(wxID_UNDO, _T("&Undo\tCtrl+Z"), _T("Undo last action"));
	editMenu->Append(wxID_REDO, _T("&Redo\tCtrl+Y"), _T("Redo last action"));
	editMenu->AppendSeparator();
	editMenu->Append(wxID_CUT, _T("Cu&t\tCtrl+X"), _T("Cut selected text"));
	editMenu->Append(wxID_COPY, _T("&Copy\tCtrl+C"), _T("Copy selected text"));
	editMenu->Append(wxID_PASTE, _T("&Paste\tCtrl+V"), _T("Paste contents of clipboard"));
	editMenu->Append(wxID_CLEAR, _T("&Delete\tDel"));
	editMenu->AppendSeparator();
	editMenu->Append (wxID_FIND, _("&Find\tCtrl+F"));
	editMenu->AppendSeparator();
	editMenu->Append(wxID_SELECTALL, _T("Select All\tCtrl+A"), _T("Select all the text in the current document"));	
	
	helpMenu->Append(wxID_ABOUT, _T("&About...\tF1"), _T("Show info about this application"));

	// Add the menus to the menubar
	mbar->Append(fileMenu, _T("&File"));
	mbar->Append(editMenu, _T("&Edit"));
	mbar->Append(viewMenu, _T("&View"));
	mbar->Append(helpMenu, _T("&Help"));

#ifdef __WXMAC__
	wxMenuBar::MacSetCommonMenuBar(mbar);
#endif // __WXMAC__
	SetMenuBar(mbar);
}

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
			event.Veto();
		}
		else if (Msgbox_Choice == wxYES)
		{
			editor->SaveFile();
			if (editor->IsModified())
			{
				wxMessageBox(_("File could not be saved"), _("Error"), wxOK | wxICON_EXCLAMATION);
				event.Veto();
			}
		}
	}
}

void MainFrame::OnClosed(wxAuiNotebookEvent &WXUNUSED(event))
{
	if (notebook->GetPageCount() == 0)
	{
		this->DisableEditControls();
	}
}

void MainFrame::OnCloseTab(wxCommandEvent &WXUNUSED(event))
{
	int current_tab = notebook->GetSelection();

	notebook->DeletePage(current_tab);
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
	info.SetCopyright(_T(" (C) 2015, Timothy J Warren"));
	
	wxAboutBox(info);
}

/**
 * Enable file-specific controls
 */
void MainFrame::EnableEditControls()
{
	fileMenu->Enable(wxID_SAVE, true);
	fileMenu->Enable(wxID_SAVEAS, true);
	
	editMenu->Enable(wxID_CUT, true);
	editMenu->Enable(wxID_COPY, true);
	editMenu->Enable(wxID_PASTE, true);
	editMenu->Enable(wxID_CLEAR, true);
	editMenu->Enable(wxID_FIND, true);
	editMenu->Enable(wxID_SELECTALL, true);
			
	toolBar->EnableTool(wxID_SAVE, true);
	toolBar->EnableTool(wxID_CLOSE, true);
	toolBar->EnableTool(wxID_COPY, true);
	toolBar->EnableTool(wxID_CUT, true);
	toolBar->EnableTool(wxID_PASTE, true);
}

/**
 * Disables file-specific controls
 */
void MainFrame::DisableEditControls()
{
	fileMenu->Enable(wxID_SAVE, false);
	fileMenu->Enable(wxID_SAVEAS, false);
	
	editMenu->Enable(wxID_CUT, false);
	editMenu->Enable(wxID_COPY, false);
	editMenu->Enable(wxID_PASTE, false);
	editMenu->Enable(wxID_CLEAR, false);
	editMenu->Enable(wxID_FIND, false);
	editMenu->Enable(wxID_SELECTALL, false);
	
	toolBar->EnableTool(wxID_SAVE, false);
	toolBar->EnableTool(wxID_CLOSE, false);
	toolBar->EnableTool(wxID_COPY, false);
	toolBar->EnableTool(wxID_CUT, false);
	toolBar->EnableTool(wxID_PASTE, false);
}