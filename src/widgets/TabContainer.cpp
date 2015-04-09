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
	this->AddTab();
}

TabContainer::~TabContainer() {}

void TabContainer::AddTab()
{
	untitled_document_count++;
	wxString caption;
	
	caption.Printf("Untitled %lu", untitled_document_count);
	
	EditPane *editor = new EditPane(this, wxID_ANY);
	
	this->AddPage(editor, caption);
}

void TabContainer::AddTab(wxString filePath)
{
	wxString caption=filePath;
	EditPane *editor = new EditPane(this, wxID_ANY);
	
	editor->LoadFile(filePath);
	
	this->AddPage(editor, caption);
}

EditPane *TabContainer::GetCurrentEditor()
{
	return (EditPane *) this->GetCurrentPage();
}
