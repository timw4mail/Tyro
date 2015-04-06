/**
 * Main Application Frame
 */

#ifndef TYROMAIN_H
#define TYROMAIN_H

#include "common.h"
#include "TyroApp.h"
#include "TabContainer.h"

class MainFrame: public wxFrame
{
	public:
		MainFrame(wxFrame *frame, const wxString &title);
		~MainFrame();
	private:
		TabContainer *notebook;
		enum
		{
			idMenuQuit = 1000,
			idMenuAbout
		};
		void SetupMenu();
		void SetupToolbar();
		void SetupStatusBar();
		void OnMenuFileNew(wxCommandEvent &event);
		void OnClose(wxCloseEvent &event);
		void OnQuit(wxCommandEvent &event);
		void OnAbout(wxCommandEvent &event);
		TabContainer* CreateTabContainer();
		DECLARE_EVENT_TABLE()
};


#endif // TYROMAIN_H
