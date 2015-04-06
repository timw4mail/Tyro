#include "EditPane.h"

BEGIN_EVENT_TABLE(EditPane, wxStyledTextCtrl)
	// common
	/*EVT_SIZE (Edit::OnSize)
	// edit
	EVT_MENU (wxID_CLEAR,		Edit::OnEditClear)
	EVT_MENU (wxID_CUT,			Edit::OnEditCut)
	EVT_MENU (wxID_COPY,		Edit::OnEditCopy)
	EVT_MENU (wxID_PASTE,		Edit::OnEditPaste)
	EVT_MENU (myID_INDENTINC,	Edit::OnEditIndentInc)
	EVT_MENU (myID_INDENTRED,	Edit::OnEditIndentRed)
	EVT_MENU (wxID_SELECTALL,	Edit::OnEditSelectAll)
	EVT_MENU (myID_SELECTLINE,	Edit::OnEditSelectLine)
	EVT_MENU (wxID_REDO,		Edit::OnEditRedo)
	EVT_MENU (wxID_UNDO,		Edit::OnEditUndo)*/
END_EVENT_TABLE()

EditPane::EditPane(
	wxWindow *parent, wxWindowID id, const wxPoint &pos, 
	const wxSize &size, long style
) : wxStyledTextCtrl (parent, id, pos, size, style)
{
	
}

EditPane::~EditPane() {}

void EditPane::OnMenuFileOpen(wxCommandEvent &WXUNUSED(event))
{
	wxFileDialog *OpenDialog = new wxFileDialog(this, _T("Choose a file"), _(""), _(""), _("*.*"), wxFD_OPEN);

	if (OpenDialog->ShowModal() == wxID_OK)
	{
		// Load the file into a new notebook tab and styled text control
	}
	OpenDialog->Close();
}

void EditPane::OnMenuFileSave(wxCommandEvent &WXUNUSED(event))
{
}