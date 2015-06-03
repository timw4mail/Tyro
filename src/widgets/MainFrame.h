/**
 * Main Application Frame
 */

#ifndef TYROMAIN_H
#define TYROMAIN_H

#include "TabContainer.h"

class MainFrame: public wxFrame
{
	public:
		MainFrame(wxFrame *frame, const wxString &title);
		~MainFrame();
		void EnableEditControls(bool enable=true);
		void OpenFiles(wxArrayString filelist);
		void OnPrefsChanged(wxCommandEvent &event);
	private:
		wxAuiManager *manager;
		wxAuiToolBar *toolBar;
		wxFindReplaceData *findReplaceData = nullptr;
		wxFindReplaceData *findData = nullptr;
		wxFindReplaceDialog *findDlg = nullptr;
		wxFindReplaceDialog *replaceDlg = nullptr;
		void SetupToolbar();
		void SetupStatusBar();
		void BindEvents();
		void DoLayout();
		
		// Main Menu Event handlers
		void OnNew(wxCommandEvent &event);
		void OnOpen(wxCommandEvent &event);
		void OnCloseAll(wxCommandEvent &event);
		void OnFileClose(wxCommandEvent &event);
		void OnSave(wxCommandEvent &event);
		void OnSaveAs(wxCommandEvent &event);
		
		void OnEditFind(wxCommandEvent &event);
		void OnEditReplace(wxCommandEvent &event);
		void OnFindDialog(wxFindDialogEvent &event);
		
		void OnToggleWhitespace(wxCommandEvent &event);
		void OnToggleLineWrap(wxCommandEvent &event);
		void OnToggleLineEndings(wxCommandEvent &event);
		
		void OnLangSelect(wxCommandEvent &event);
		
		void OnCloseTab(wxCommandEvent &event);
		void OnQuit(wxCommandEvent &event);
		void OnAbout(wxCommandEvent &event);
};


#endif // TYROMAIN_H
