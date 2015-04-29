/**
 * Main Application Frame
 */

#ifndef TYROMAIN_H
#define TYROMAIN_H

#include "../wx_common.h"

#include <wx/cmdline.h>
#include <wx/config.h>
#include <wx/aboutdlg.h>

#include "TabContainer.h"

class MainFrame: public wxFrame
{
	friend class TabContainer;
	public:
		MainFrame(wxFrame *frame, const wxString &title);
		~MainFrame();
	private:
		TabContainer *notebook;
		wxToolBar *toolBar;
		wxMenuBar *mbar;
		wxMenu *fileMenu;
		wxMenu *editMenu;
		wxMenu *viewMenu;
		wxMenu *langMenu;
		wxMenu *helpMenu;
		void SetupMenu();
		void SetupToolbar();
		void SetupStatusBar();
		void BindEvents();
		void EnableEditControls(bool enable=true);
		
		// Main Menu Event handlers
		void OnNew(wxCommandEvent &event);
		void OnOpen(wxCommandEvent &event);
		void OnClose(wxAuiNotebookEvent &event);
		void OnCloseAll(wxCommandEvent &event);
		void OnClosed(wxAuiNotebookEvent &event);
		void OnFileClose(wxCommandEvent &event);
		void OnSave(wxCommandEvent &event);
		void OnSaveAs(wxCommandEvent &event);
		
		void OnEditCut(wxCommandEvent &event);
		void OnEditCopy(wxCommandEvent &event);
		void OnEditPaste(wxCommandEvent &event);
		void OnEditSelectAll(wxCommandEvent &event);
		void OnEditUndo(wxCommandEvent &event);
		void OnEditRedo(wxCommandEvent &event);
		
		void OnFind(wxCommandEvent &event);
		void OnFindNext(wxCommandEvent &event);
		void OnReplace(wxCommandEvent &event);
		void OnReplaceNext(wxCommandEvent &event);
		void OnToggleWhitespace(wxCommandEvent &event);
		void OnToggleLineWrap(wxCommandEvent &event);
		
		void OnCloseTab(wxCommandEvent &event);
		void OnQuit(wxCommandEvent &event);
		void OnAbout(wxCommandEvent &event);

		// Tab Context Menu Event handlers
		void OnTabContextMenu(wxAuiNotebookEvent &event);
};


#endif // TYROMAIN_H
