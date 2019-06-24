/**
 * Main Application Frame
 */
#pragma once

#include "src/widgets/TyroMenu.h"
#include "src/widgets/EditorPane.h"
#include "src/widgets/TabContainer.h"
#include "src/widgets/PrefFrame.h"
#include "src/widgets/FileTreePane.h"

class MainFrame: public wxFrame
{
	public:
		MainFrame(wxFrame *frame, const wxString &title, const wxSize &size);
		~MainFrame() override;
		void EnableEditControls(bool enable=true);
		void OpenFiles(wxArrayString filelist);
		void OnPrefsChanged();
	private:
		PrefFrame *prefFrame = nullptr;
		FileTreePane *fileTreePane = nullptr;
		TabContainer *notebook = nullptr;
		wxAuiManager *manager = nullptr;
		wxAuiToolBar *toolBar = nullptr;
		wxFindReplaceData *findReplaceData = nullptr;
		wxFindReplaceData *findData = nullptr;
		wxFindReplaceDialog *findDlg = nullptr;
		wxFindReplaceDialog *replaceDlg = nullptr;
		wxAuiToolBar* SetupToolbar();
		void BindEvents();
		void MainLayout();
		
		// Main Menu Event handlers
		void OnNew(wxCommandEvent &event);
		void OnOpen(wxCommandEvent &event);
		void OnOpenFolder(wxCommandEvent &event);
		void OnCloseAll(wxCommandEvent &event);
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
