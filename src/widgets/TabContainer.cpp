/**
 * Wrapper around wxAuiNotebook
 */

#include "TabContainer.h"

static unsigned long untitled_document_count = 0;

TabContainer::TabContainer(
		wxWindow* parent,
		wxWindowID id,
		const wxPoint& pos,
		const wxSize& size,
		long style
) : wxAuiNotebook(parent, id, pos, size, style)
{	
}

TabContainer::~TabContainer() {}

void TabContainer::AddTab()
{
	untitled_document_count++;
	
	wxString caption;

	caption.Printf("Untitled %lu", untitled_document_count);

	EditPane *editor = new EditPane(this, wxID_ANY);

	this->AddPage(editor, caption, true);
}

void TabContainer::AddTab(wxString filePath)
{
	wxFileName fileName(filePath);
	
	wxString caption= fileName.GetFullName();
	EditPane *editor = new EditPane(this, wxID_ANY);
	
	bool loaded_file = editor->LoadAndHighlight(filePath);
	
	if (loaded_file)
	{
		this->AddPage(editor, caption, true);
	}
	else
	{
		wxMessageDialog err(
			this, 
			_T("Failed to open the specified file. Do you have permission to open it?"),
			_T("Could not open file."),
			wxOK|wxCENTER|wxICON_WARNING
		);
		err.ShowModal();
	}
}

EditPane *TabContainer::GetCurrentEditor()
{
	return (EditPane *) this->GetCurrentPage();
}