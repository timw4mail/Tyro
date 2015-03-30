/***************************************************************
 * Name:      TyroApp.h
 * Purpose:   Defines Application Class
 * Author:    Timothy J Warren (tim@timshomepage.net)
 * Created:   2015-03-30
 * Copyright: Timothy J Warren (https://timshomepage.net)
 * License:
 **************************************************************/

#ifndef TYROAPP_H
#define TYROAPP_H

#include <wx/app.h>

class TyroApp : public wxApp
{
    friend class TyroFrame;
    public:
        virtual bool OnInit();
        virtual int OnExit(wxCommandEvent &WXUNUSED(event));
};

#endif // TYROAPP_H