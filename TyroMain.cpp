/***************************************************************
 * Name:      TyroMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Timothy J Warren (tim@timshomepage.net)
 * Created:   2015-03-30
 * Copyright: Timothy J Warren (https://timshomepage.net)
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#include "TyroMain.h"

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__WXMAC__)
        wxbuild << _T("-Mac");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

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
    fileMenu->Append(wxID_OPEN, _T("&Open"), _T("Opens an existing file"));
    fileMenu->Append(wxID_SAVE, _T("&Save"), _T("Save the content"));
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT, _("&Quit"), _("Quit the application"));

    helpMenu->Append(wxID_ABOUT, _("&About"), _("Show info about this application"));

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
    SetStatusText(_(""),0);
    SetStatusText(wxbuildinfo(short_f), 1);

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

void TyroFrame::OnQuit(wxCommandEvent &event)
{
    Destroy();
}

void TyroFrame::OnAbout(wxCommandEvent &event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}
