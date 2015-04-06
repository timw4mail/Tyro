/**
 * Main Application Frame
 */

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#include "Main.h"


BEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_CLOSE(MainFrame::OnClose)
	EVT_MENU(wxID_NEW, MainFrame::OnMenuFileNew)
	EVT_MENU(wxID_OPEN, EditPane::OnMenuFileOpen)
	EVT_MENU(wxID_SAVE, EditPane::OnMenuFileSave)
	EVT_MENU(wxID_EXIT, MainFrame::OnQuit)
	EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
END_EVENT_TABLE()

MainFrame::MainFrame(wxFrame *frame, const wxString& title)
	: wxFrame(frame, -1, title)
{
	this->SetupMenu();

	// create a status bar with some information about the used wxWidgets version
	this->SetupStatusBar();

	// create the main toolbar
	this->SetupToolbar();

	// Set up control layout
	wxBoxSizer *base_sizer = new wxBoxSizer(wxVERTICAL);

	notebook = this->CreateTabContainer();

	base_sizer->Add(notebook, 1, wxEXPAND | wxALL, 5);

	base_sizer->SetContainingWindow(this);
	base_sizer->SetMinSize(800,600);

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
#ifndef __WXGTK__
	#include "../resources/xpm/48/file-empty.xpm"
	#include "../resources/xpm/48/folder.xpm"
	#include "../resources/xpm/48/floppy.xpm"
	#include "../resources/xpm/48/wrench-screwdriver.xpm"
#endif // __WXGTK__
#ifdef __WXGTK__
	#include "../resources/xpm/24/file-empty.xpm"
	#include "../resources/xpm/24/folder.xpm"
	#include "../resources/xpm/24/floppy.xpm"
	#include "../resources/xpm/24/wrench-screwdriver.xpm"
#endif

	CreateToolBar(wxNO_BORDER | wxTB_FLAT | wxTB_HORIZONTAL);

	wxToolBar *toolBar = GetToolBar();

	wxBitmap bitmaps[4];

	bitmaps[0] = wxBitmap(file_empty);
	bitmaps[1] = wxBitmap(folder);
	bitmaps[2] = wxBitmap(floppy);
	bitmaps[3] = wxBitmap(wrench_screwdriver);

	toolBar->AddTool(wxID_NEW, "New", bitmaps[0], "New file");
	toolBar->AddTool(wxID_OPEN, "Open", bitmaps[1], "Open file");
	toolBar->AddTool(wxID_SAVE, "Save", bitmaps[2], "Save file");
	toolBar->AddSeparator();
	toolBar->AddTool(wxID_ANY, "Settings", bitmaps[3], "Change Settings");
	toolBar->Realize();
}

void MainFrame::SetupMenu()
{
	// create a menu bar
	wxMenuBar* mbar = new wxMenuBar();

	// Create Base menus
	wxMenu* fileMenu = new wxMenu(_T(""));
	wxMenu* editMenu = new wxMenu(_T(""));
	wxMenu* helpMenu = new wxMenu(_T(""));

	// Add items to top-level menus
	fileMenu->Append(wxID_NEW, _T("&New\tCtrl+N"), _T("Create a new file"));
	fileMenu->AppendSeparator();
	fileMenu->Append(wxID_OPEN, _T("&Open\tCtrl+O"), _T("Opens an existing file"));
	fileMenu->Append(wxID_CLOSE, _T("&Close\tCtrl+W"), _T("Close the current document"));
	fileMenu->Append(wxID_SAVE, _T("&Save\tCtrl+S"), _T("Save the content"));
	fileMenu->AppendSeparator();
	fileMenu->Append(wxID_EXIT, _T("&Quit\tCtrl+Q"), _T("Quit the application"));

	editMenu->Append(wxID_UNDO, _T("&Undo\tCtrl+Z"), _T("Undo last action"));
	editMenu->Append(wxID_REDO, _T("&Redo\tCtrl+Y"), _T("Redo last action"));
	editMenu->AppendSeparator();
	editMenu->Append(wxID_CUT, _T("Cu&t\tCtrl+X"), _T("Cut selected text"));
	editMenu->Append(wxID_COPY, _T("&Copy\tCtrl+C"), _T("Copy selected text"));
	editMenu->Append(wxID_PASTE, _T("&Paste\tCtrl+V"), _T("Paste contents of clipboard"));

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

TabContainer *MainFrame::CreateTabContainer()
{
	TabContainer *notebook = new TabContainer(this);

	return notebook;
}

void MainFrame::OnClose(wxCloseEvent &WXUNUSED(event))
{
	Destroy();
}

void MainFrame::OnMenuFileNew(wxCommandEvent &WXUNUSED(event))
{
	notebook->AddTab();
}

void MainFrame::OnQuit(wxCommandEvent &WXUNUSED(event))
{
	Destroy();
}

void MainFrame::OnAbout(wxCommandEvent &WXUNUSED(event))
{
	wxMessageBox(_T("Tyro, a text editor for all development\n Copyright 2015, Timothy J. Warren"), wxT("About Tyro"), wxOK| wxICON_INFORMATION, this);
}
