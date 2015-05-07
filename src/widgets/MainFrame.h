/**
 * Main Application Frame
 */

#ifndef TYROMAIN_H
#define TYROMAIN_H

#include <wx/cmdline.h>
#include <wx/config.h>
#include <wx/aboutdlg.h>
#include <wx/fdrepdlg.h>
#include "TabContainer.h"

class MainFrame: public wxFrame
{
	public:
		MainFrame(wxFrame *frame, const wxString &title);
		~MainFrame();
		void EnableEditControls(bool enable=true);
	private:
		wxToolBar *toolBar;
		wxFindReplaceData *findReplaceData;
		wxFindReplaceDialog *findDlg;
		wxFindReplaceDialog *replaceDlg;
		void SetupToolbar();
		void SetupStatusBar();
		void BindEvents();
		
		// Main Menu Event handlers
		void OnNew(wxCommandEvent &event);
		void OnOpen(wxCommandEvent &event);
		void OnCloseAll(wxCommandEvent &event);
		void OnFileClose(wxCommandEvent &event);
		void OnSave(wxCommandEvent &event);
		void OnSaveAs(wxCommandEvent &event);
		
		void OnEditCut(wxCommandEvent &event);
		void OnEditCopy(wxCommandEvent &event);
		void OnEditPaste(wxCommandEvent &event);
		void OnEditSelectAll(wxCommandEvent &event);
		void OnEditUndo(wxCommandEvent &event);
		void OnEditRedo(wxCommandEvent &event);
		
		void OnEditFind(wxCommandEvent &event);
		void OnEditReplace(wxCommandEvent &event);
		void OnFindDialog(wxFindDialogEvent &event);
		void OnToggleWhitespace(wxCommandEvent &event);
		void OnToggleLineWrap(wxCommandEvent &event);
		void OnToggleLineEndings(wxCommandEvent &event);
		
		void OnCloseTab(wxCommandEvent &event);
		void OnQuit(wxCommandEvent &event);
		void OnAbout(wxCommandEvent &event);
};


#endif // TYROMAIN_H
