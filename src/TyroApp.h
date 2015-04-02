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

#include "common.h"

#include <wx/app.h>
#include <wx/debug.h>

class TyroApp : public wxApp
{
	friend class MainFrame;
	public:
		virtual bool OnInit();
		virtual int OnExit();
};

#endif // TYROAPP_H
