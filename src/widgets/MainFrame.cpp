/**
 * Main Application Frame
 */

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#include "MainFrame.h"

MainFrame::MainFrame(wxFrame *frame, const wxString& title)
	: wxFrame(frame, -1, title)
{
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
	base_sizer->SetMinSize(600,400);

	SetSizerAndFit(base_sizer);
	
	// Finally, bind events
	this->BindEvents();
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
	#include "../../resources/xpm/48/file-empty.xpm"
	#include "../../resources/xpm/48/folder.xpm"
	#include "../../resources/xpm/48/floppy.xpm"
	#include "../../resources/xpm/48/sign-error.xpm"
	#include "../../resources/xpm/48/wrench-screwdriver.xpm"
#else
	#include "../../resources/xpm/24/file-empty.xpm"
	#include "../../resources/xpm/24/folder.xpm"
	#include "../../resources/xpm/24/floppy.xpm"
	#include "../../resources/xpm/24/sign-error.xpm"
	#include "../../resources/xpm/24/wrench-screwdriver.xpm"
#endif

	CreateToolBar(wxNO_BORDER | wxTB_FLAT | wxTB_HORIZONTAL);

	toolBar = GetToolBar();

	vector<wxBitmap> bitmaps;

	bitmaps.push_back(wxBitmap(file_empty));
	bitmaps.push_back(wxBitmap(folder));
	bitmaps.push_back(wxBitmap(floppy));
	bitmaps.push_back(wxBitmap(sign_error));
	bitmaps.push_back(wxBitmap(wrench_screwdriver));

	toolBar->AddTool(wxID_NEW, "New", bitmaps[0], "New file");
	toolBar->AddTool(wxID_OPEN, "Open", bitmaps[1], "Open file");
	toolBar->AddTool(wxID_SAVE, "Save", bitmaps[2], "Save file");
	//toolBar->AddTool(wxID_CLOSE, "Close", bitmaps[3], "Close file");
	//toolBar->AddSeparator();
	//toolBar->AddTool(wxID_ANY, "Settings", bitmaps[4], "Change Settings");

	toolBar->EnableTool(wxID_SAVE, false);

	toolBar->Realize();
}

void MainFrame::SetupMenu()
{
	// create a menu bar
	mbar = new wxMenuBar();

	// Create Base menus
	fileMenu = new wxMenu(_T(""));
	editMenu = new wxMenu(_T(""));
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
	
	fileMenu->Enable(wxID_SAVE, false);
	fileMenu->Enable(wxID_SAVEAS, false);
	fileMenu->Enable(wxID_CLOSE, false);

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
	
	editMenu->Enable(wxID_UNDO, false);
	editMenu->Enable(wxID_REDO, false);
	editMenu->Enable(wxID_CUT, false);
	editMenu->Enable(wxID_COPY, false);
	editMenu->Enable(wxID_PASTE, false);
	editMenu->Enable(wxID_CLEAR, false);
	editMenu->Enable(wxID_FIND, false);
	editMenu->Enable(wxID_SELECTALL, false);

	helpMenu->Append(wxID_ABOUT, _T("&About...\tF1"), _T("Show info about this application"));

	// Add the menus to the menubar
	mbar->Append(fileMenu, _T("&File"));
	mbar->Append(editMenu, _T("&Edit"));
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
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnFileClose, this, wxID_CLOSE);
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
	notebook->AddTab();
}

void MainFrame::OnOpen(wxCommandEvent &WXUNUSED(event))
{
	wxString filename;
	
	wxFileDialog dlg (this, _T("Open file"), wxEmptyString, wxEmptyString,
		TYRO_FILE_OPEN_WILDCARDS, wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_CHANGE_DIR);

	if (dlg.ShowModal() != wxID_OK) return;
	
	filename = dlg.GetPath();
	
	notebook->AddTab(filename);
}

void MainFrame::OnFileClose(wxCommandEvent &WXUNUSED(event))
{

}

void MainFrame::OnSave(wxCommandEvent &WXUNUSED(event))
{
	
}

void MainFrame::OnSaveAs(wxCommandEvent &WXUNUSED(event))
{
	
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
	
	info.SetName("Tyro");
	info.SetVersion("0.0.1", "Prerelease");
	
	info.AddDeveloper("Tim Warren, Programmer");
	
	info.SetDescription("Tyro, a text editor for all development");
	info.SetCopyright(_T(" (C) 2015, Timothy J Warren"));
	
	wxGenericAboutDialog dlg;
	dlg.Create(info, this);
	dlg.ShowModal();
}
