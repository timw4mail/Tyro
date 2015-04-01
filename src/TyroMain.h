/***************************************************************
 * Name:	  TyroMain.h
 * Purpose:   Defines Application Frame
 * Author:	Timothy J Warren (tim@timshomepage.net)
 * Created:   2015-03-30
 * Copyright: Timothy J Warren (https://timshomepage.net)
 * License:
 **************************************************************/

#ifndef TYROMAIN_H
#define TYROMAIN_H

#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include "TyroApp.h"

class TyroFrame: public wxFrame
{
	public:
		TyroFrame(wxFrame *frame, const wxString& title);
		~TyroFrame();
	private:
		enum
		{
			idMenuQuit = 1000,
			idMenuAbout
		};
void OnMenuFileOpen(wxCommandEvent &event);
void OnMenuFileSave(wxCommandEvent &event);
		void OnClose(wxCloseEvent &event);
		void OnQuit(wxCommandEvent &event);
		void OnAbout(wxCommandEvent &event);
		wxAuiNotebook* CreateNotebook();
		DECLARE_EVENT_TABLE()
};


#endif // TYROMAIN_H
