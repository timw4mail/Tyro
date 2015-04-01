/***************************************************************
 * Name:	  TyroMain.cpp
 * Purpose:   Code for Application Frame
 * Author:	Timothy J Warren (tim@timshomepage.net)
 * Created:   2015-03-30
 * Copyright: Timothy J Warren (https://timshomepage.net)
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#include "TyroMain.h"


BEGIN_EVENT_TABLE(TyroFrame, wxFrame)
	EVT_CLOSE(TyroFrame::OnClose)
	EVT_MENU(wxID_EXIT, TyroFrame::OnQuit)
	EVT_MENU(wxID_ABOUT, TyroFrame::OnAbout)
END_EVENT_TABLE()

TyroFrame::TyroFrame(wxFrame *frame, const wxString& title)
	: wxFrame(frame, -1, title)
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
	fileMenu->Append(wxID_OPEN, _T("&Open\tCtrl+0"), _T("Opens an existing file"));
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
	mbar->Append(fileMenu, _("&File"));
	mbar->Append(editMenu, _("&Edit"));
	mbar->Append(helpMenu, _("&Help"));

#ifdef __WXMAC__
	wxMenuBar::MacSetCommonMenuBar(mbar);
#endif // __WXMAC__
SetMenuBar(mbar);

	// create a status bar with some information about the used wxWidgets version
	CreateStatusBar(2);
	SetStatusText(_(""), 0);
	SetStatusText(_(""), 1);

	// Set up control layout
	wxBoxSizer *base_sizer = new wxBoxSizer(wxVERTICAL);

	base_sizer->Add(
		CreateNotebook(),
		1,
		wxEXPAND | wxALL,
		5
	);

	base_sizer->SetContainingWindow(this);
	base_sizer->SetMinSize(800,600);

	SetSizerAndFit(base_sizer);
}


TyroFrame::~TyroFrame() {}

wxAuiNotebook *TyroFrame::CreateNotebook()
{
	wxAuiNotebook *ctrl = new wxAuiNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_NB_DEFAULT_STYLE);



	return ctrl;
}

void TyroFrame::OnClose(wxCloseEvent &event)
{
	Destroy();
}

void TyroFrame::OnMenuFileOpen(wxCommandEvent &event)
{

}

void TyroFrame::OnMenuFileSave(wxCommandEvent &event)
{
}

void TyroFrame::OnQuit(wxCommandEvent &event)
{
	Destroy();
}

void TyroFrame::OnAbout(wxCommandEvent &event)
{
	wxMessageBox(_T("Tyro, a text editor for all development\n Copyright 2015, Timothy J. Warren"), wxT("About Tyro"), wxOK| wxICON_INFORMATION, this);
}
