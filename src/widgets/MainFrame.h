/**
 * Main Application Frame
 */

#ifndef TYROMAIN_H
#define TYROMAIN_H

#include "../wx_common.h"
#include "../TyroApp.h"

#include <wx/aboutdlg.h>
#include <wx/generic/aboutdlgg.h>

#include "TabContainer.h"

class MainFrame: public wxFrame
{
	friend class TabContainer;
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
		void BindEvents();
		void OnNew(wxCommandEvent &event);
		void OnOpen(wxCommandEvent &event);
		void OnFileClose(wxCommandEvent &event);
		void OnSave(wxCommandEvent &event);
		void OnSaveAs(wxCommandEvent &event);
		void OnEditCut(wxCommandEvent &event);
		void OnEditCopy(wxCommandEvent &event);
		void OnEditPaste(wxCommandEvent &event);
		void OnEditSelectAll(wxCommandEvent &event);
		void OnEditUndo(wxCommandEvent &event);
		void OnEditRedo(wxCommandEvent &event);
		void OnClose(wxCloseEvent &event);
		void OnQuit(wxCommandEvent &event);
		void OnAbout(wxCommandEvent &event);
};


#endif // TYROMAIN_H
