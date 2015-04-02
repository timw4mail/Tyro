/***************************************************************
 * Name:	  TyroApp.h
 * Purpose:   Defines Application Class
 * Author:	Timothy J Warren (tim@timshomepage.net)
 * Created:   2015-03-30
 * Copyright: Timothy J Warren (https://timshomepage.net)
 * License:
 **************************************************************/

#ifndef TYROAPP_H
#define TYROAPP_H

#include "wx/wxprec.h"

#include <wx/app.h>
#include <wx/toolbar.h>
#include <wx/menu.h>
#include <wx/debug.h>
#include <wx/aui/aui.h>

class TyroApp : public wxApp
{
	friend class MainFrame;
	public:
		virtual bool OnInit();
		virtual int OnExit(wxCommandEvent &WXUNUSED(event));
};

#endif // TYROAPP_H
