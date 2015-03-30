/***************************************************************
 * Name:      TyroApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Timothy J Warren (tim@timshomepage.net)
 * Created:   2015-03-30
 * Copyright: Timothy J Warren (https://timshomepage.net)
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#include "TyroApp.h"
#include "TyroMain.h"

IMPLEMENT_APP(TyroApp);

bool TyroApp::OnInit()
{
    TyroFrame* frame = new TyroFrame(0L, _("wxWidgets Application Template"));

    frame->Show(true);

    return true;
}

int TyroApp::OnExit(wxCommandEvent &WXUNUSED(event))
{
    close(true);
    return 1;
}
