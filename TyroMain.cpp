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
    wxMenu* fileMenu = new wxMenu(_T(""));

    fileMenu->Append(wxID_EXIT, _("&Quit\tAlt-F4"), _("Quit the application"));
    mbar->Append(fileMenu, _("&File"));

    wxMenu* helpMenu = new wxMenu(_T(""));
    helpMenu->Append(wxID_ABOUT, _("&About\tF1"), _("Show info about this application"));
    mbar->Append(helpMenu, _("&Help"));

#ifdef __WXMAC__
    wxMenuBar::MacSetCommonMenuBar(mbar);
#endif // __WXMAC__
SetMenuBar(mbar);

    // create a status bar with some information about the used wxWidgets version
    CreateStatusBar(2);
    SetStatusText(_(""),0);
    SetStatusText(wxbuildinfo(short_f), 1);
}


TyroFrame::~TyroFrame()
{
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
